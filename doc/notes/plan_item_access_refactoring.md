# SessionItem item-access API refactoring

> Working note. **Status: the API change, const-correctness Rounds A and B, and Round C #13 are
> DONE.** sup-mvvm 1369/1369, sup-gui-core 447/447, sequencer-gui 890/890 tests pass.
> Remaining: #14, #15, and the selection signals postponed at the end of #13.

## Why this was done

`SessionItem`'s item accessors had three defects:

1. **Const-incorrectness.** All five accessors were `const` member functions returning
   **non-const** pointers, so a `const SessionItem&` handed out full mutable access to its
   subtree. The same defect existed in `TaggedItems` and `SessionItemContainer`.
2. **Three different answers to one question.** On a child of the wrong type, `GetItem<T>`
   threw, `GetItems<T>` silently filtered, and `ItemSelection::GetItem<T>` returned nullptr.
3. **No nullable typed accessor**, so downstream hand-rolled
   `HasTag(item, tag) ? GetItem<T>(tag) : nullptr`, and `GroupItem` could not answer
   "is the current item a `T`?" without catching an exception.

## The law

> **`Get…` is strict: it throws and returns a reference.
> `Find…` is total: it never throws and returns a pointer (or a filtered vector).**
> Failure means unregistered tag, out-of-range index, *or* failed cast — all three,
> uniformly, in both families.

This was already the repo's convention (`TaggedItems::GetContainer` throws /
`FindContainer` returns nullptr; `FindItemUp`, `FindNextSibling`, `ISessionModel::FindItem`
are all nullable). `SessionItem` was the outlier.

Reusing the name `GetItem` for the reference-returning form was deliberate: `SessionItem`'s
copy ctor is deleted, so every legacy pattern against the old pointer version
(`auto* p =`, `auto x =`, `if (…)`, `->`, `== nullptr`) fails to compile. The migration was
100% compiler-detected. The only silent-change pattern would have been a discarded
`(void)item.GetItem(ti);` — there were none.

## Final API (`mvvm/model/session_item.h`)

```cpp
std::size_t GetTotalItemCount() const;
std::size_t GetItemCount(std::string_view tag) const;          // throws on unregistered tag

const SessionItem* FindItem(const TagIndex&) const;            // never throws
SessionItem*       FindItem(const TagIndex&);

std::vector<const SessionItem*> GetItems(std::string_view tag) const;   // throws on bad tag
std::vector<SessionItem*>       GetItems(std::string_view tag);
std::vector<const SessionItem*> GetAllItems() const;
std::vector<SessionItem*>       GetAllItems();

template <typename ItemT = SessionItem> const ItemT& GetItem(const TagIndex&) const;   // throws
template <typename ItemT = SessionItem> ItemT&       GetItem(const TagIndex&);
template <typename ItemT = SessionItem> const ItemT& GetItem(std::string_view tag) const;
template <typename ItemT = SessionItem> ItemT&       GetItem(std::string_view tag);

template <typename ItemT = SessionItem> const ItemT* FindItem(const TagIndex&) const;  // nullptr
template <typename ItemT = SessionItem> ItemT*       FindItem(const TagIndex&);
template <typename ItemT = SessionItem> const ItemT* FindItem(std::string_view tag) const;
template <typename ItemT = SessionItem> ItemT*       FindItem(std::string_view tag);

template <typename ItemT = SessionItem> std::vector<const ItemT*> FindItems(std::string_view) const;
template <typename ItemT = SessionItem> std::vector<ItemT*>       FindItems(std::string_view);
```

5 names, 16 declarations, 8 of which are const twins the caller never chooses between.
A private `FindItemsImpl` (built on `TaggedItems::FindContainer`) gives `FindItems` its
total behaviour, since `GetItems` throws on an unregistered tag.

### Design decisions worth not re-litigating

- **No non-template `GetItem(const TagIndex&)`.** It existed briefly only to disambiguate
  `GetItem({"tag", 0})`, where a braced initializer matches both `TagIndex(string_view, size_t)`
  and `string_view(const char*, size_t)`. Removed deliberately: braced-init addressing reads as
  a raw pair rather than an address. Spell it `GetItem(TagIndex("tag", 0))`.
  `FindItem`'s non-template overload stays — there it is structural, the template dispatches to
  it to avoid RTTI on the `SessionItem` case.
- **`TagIndex` stays `explicit`.** Making it implicit achieves nothing on its own
  (`const char*` → `string_view` → `TagIndex` is two user-defined conversions); adding
  `const char*`/`std::string` ctors to fix that splits the overload set so `std::string_view`
  arguments resolve differently from `const char*` ones — and sup-mvvm uses
  `constexpr std::string_view` tag constants while sequencer-gui uses `constexpr auto = "..."`.
- **The `std::string_view` overloads earn their keep**: ~60 call sites address the first item
  under a named tag (`GetItem(VectorItem::kX)`); without them all become
  `GetItem(TagIndex(VectorItem::kX))`.
- **No strict `GetItems<T>`.** "Throw if any sibling is the wrong type" is not wanted; plural is
  inherently filtering, so typed-plural lives in the `Find` family.

## What shipped

**sup-mvvm** (commits `4398a43f` … `23d89257`)
- `utils::ConvertItems<T>` added — implicit-conversion-only (upcast and/or add const), no RTTI,
  cannot drop elements. `CastItems`/`StaticCastItems`/`MakeConst`/`RemoveConst` kept.
- const/non-const pairs on `SessionItemContainer`, `TaggedItems` (plus `GetItem`→`FindItem`
  rebuilt on `FindContainer`, `const` dropped from `InsertItem`/`TakeItem`, `GetTotalItemCount`
  added), `SessionItem`, `ISessionModel::GetRootItem`, `item_utils` (`ChildAt`, `TopLevelItems`,
  `SinglePropertyItems`), `model_utils` (`GetTopItems`, `GetTopItem`, `FindItems`, `ItemFromPath`).
- `CompoundItem::Property`/`SetProperty` no longer dereference a possibly-null pointer.
- `GroupItem` split into `GetCurrentItem<T>()` (reference, throws) and `FindCurrentItem<T>()`
  (pointer, nullptr); `SetCurrentType<T>()` returns a reference.
- `GetTotalItemCount` no longer allocates a vector.
- The 5 `IChildrenStrategy` implementations use `utils::RemoveConst` with a comment naming the
  interface contract (see "Accepted boundaries").

**sup-gui-core** — migrated; model accessors turned into pairs; `AnyValueItem::GetChildren()`
made const-correct (const virtual returning `vector<const AnyValueItem*>` + non-virtual non-const
overload + `using` in the two overriding classes). That change broke **exactly one** of its ~119
call sites, and that site was a real bug.

**sequencer-gui** — migrated; ~12 leaky accessors turned into pairs; the
`ProcedurePreambleItem` const-mutator cluster de-consted; `DomainWorkspaceBuilder` maps now hold
`const VariableItem*`; `ExclusiveCheckStatePresentationItem::GetInstructionContainer` no longer
narrows to const.

## Latent bugs this exposed (all fixed)

- `ViewportItem::SetViewportArea` / `SetViewportToContent` and `LineSeriesItem::SetNamedColor`
  were `const` methods that mutated children.
- `ProcedurePreambleItem::AddTypeRegistration` / `AddPluginPath` / `SetPluginPaths` likewise.
- `SetBreakpointsFromInfo` looked instructions up through a **const** model path and then wrote
  breakpoints through them.
- `UpdateArrayElementNames(const mvvm::SessionItem&)` called `SetDisplayName`.
- `SetAttribute(const mvvm::CompoundItem&, …)`, `UpdateShowCollapsedPropertyAppearance`,
  `UpdateIsRootPropertyAppearance`, `UpdatePluginNames`, `PopulateProcedurePreambleItem`.
- `settings_helper.cpp` shared one `Node { SessionItem* }` between a read-only writer and a
  mutating reader; now `template <typename ItemT> struct Node`.
- ~40 test fixtures declared `const` and then mutated.

## Workarounds the new API retired

- `HasTag(x, t) ? GetItem<T>(t) : nullptr` → `FindItem<T>(t)` (5 sites; `FindItem` is total).
- Hand-rolled get-or-throw wrappers → `GetItem<T>` (`GetTextEditItem`, 8 call sites; also took
  a `const_cast` and two includes with it).
- `dynamic_cast<T*>(x->FindItem(tag))` → `x->FindItem<T>(tag)` (13 sites).

## Accepted boundaries (deliberately NOT fixed)

- **`IChildrenStrategy::GetChildren(const SessionItem*) const` → `std::vector<SessionItem*>`**
  (`mvvm/providers/i_children_strategy.h:54`). const item in, mutable children out, by contract.
  Fixing it drags in the whole ViewItem/viewmodel construction path. Its implementations (5 in
  sup-mvvm, 1 in sup-gui-core, 4 in sequencer-gui) use `utils::RemoveConst` with a comment.
- **`UniversalInstructionItem::GetAttributeItems` / `UniversalVariableItem::GetAttributeItems`**
  keep a `RemoveConst` and a FIXME, pending a separate attribute-system refactor. A const/non-const
  pair was prototyped and works (the two consumers divide cleanly: `InitFromDomainImpl` writes,
  `SetupDomainImpl` reads), but it needs a templated `Attribute` struct and was judged not worth
  the machinery yet.
- **`LinkedItem::Get<T>() const`** still returns a non-const `T*`; the link points outside the
  subtree, which is a different question from child ownership.
- **`SessionItemContainer::begin()/end()`** expose `const_iterator` over
  `vector<unique_ptr<SessionItem>>`, so `(*it)->SetData(...)` still mutates through a const
  container. Documented as internal; closing it needs an iterator adaptor.
- **`ISessionModel::FindItem(identifier)`** has no templated form, so
  `dynamic_cast<T*>(model->FindItem(id))` remains the idiom at those sites.

## Verification

```bash
cmake --preset debug-gcc && cmake --build --preset debug-gcc
ctest --test-dir out/build/debug-gcc
grep -rn "(void).*GetItem" src/ test/      # must stay empty - the one silent-change pattern
grep -rn "const_cast" src/ | wc -l         # only as_const forwarding + documented boundaries
```

Downstream packages build against the sup-mvvm build tree via the CMake package registry.
There was no version bump, so a stale downstream build fails with template errors at the call
sites rather than a clean `find_package` message.

---


# Const-correctness backlog

Goal of these rounds: **reduce `const_cast` / `std::as_const` / `RemoveConst` / `MakeConst` /
`StaticCastItems`, and replace `dynamic_cast` with `GetItem<T>` / `FindItem<T>` where the operand
is an item accessor result.**

Totals (`src/` + `test/`), start of Round A → now:

| package | `const_cast` | `RemoveConst` | `MakeConst` | `CastItems` misuse | `StaticCastItems` |
|---|---|---|---|---|---|
| sup-mvvm | 33 → **19** | 18 → **9** | 0 | 0 | 3 → **0** |
| sup-gui-core | 10 → **0** | 3 → **2** | 0 | 6 → **0** | 0 |
| sequencer-gui | 24 → **7** | 9 | 2 (legit) | 6 → **0** | 0 |

sup-mvvm gains one on purpose: the single conversion now concentrated inside `ItemSelection`
(#13), which bought back 18 downstream.

Tests green throughout: sup-mvvm 1369/1369, sup-gui-core 447/447, sequencer-gui 890/890.
(sup-mvvm lost one test with the `StaticCastItems` deletion.)

## Round A — DONE

sup-mvvm `0e7cbf29`, `a395e40e`; sup-gui-core `cfc97df2`, `6cabe57d`; sequencer-gui `ef709aeb`,
`db326962`.

1. **10 owner-side non-const accessors written directly.** Six were true one-liners
   (`SessionItemImpl` already exposes non-const accessors, so `SessionItem::GetItemData()` is just
   `p_impl->GetItemData()`). The four in `tagged_items.cpp` would have duplicated an 11-line search
   loop and two throw-message literals, so they share file-local helpers instead
   (`FindContainerByName<ContainerT>`, `ThrowNoSuchContainer`, `ThrowWrongContainerIndex`).
   Knock-on: `TaggedItems::GetItems(tag)` collapsed to `GetContainer(tag)->GetItems()`.
2. **7 owner-side `RemoveConst`** removed. `SessionItemContainer::GetItems()` became
   `GetVectorOfPtrs(m_items)`; `item_utils`, `graph_viewport_item` and `TaggedItems::GetAllItems`
   collapsed onto shared template helpers (see "The shared-implementation pattern" below).
3. **Dead casts deleted** — all four were genuinely no-ops, plus two redundant `dynamic_cast` in
   tests.
4. **`ConvertItems` adopted at the 12 `CastItems` upcast sites.**
   **Finding worth remembering:** `CastItems` silently drops `nullptr`
   (`dynamic_cast<T*>(nullptr)` fails its `if (casted)` check); `ConvertItems` preserves it by
   design. This made `AnyValueEditorActionHandlerTest.RemoveItemWhenNothingIsSelected` segfault.
   The cause was the **test**, which fabricated `{nullptr}` as "nothing is selected" — an input the
   production wiring cannot produce, since `ItemViewComponentProvider::GetSelection()` skips
   nullptr and `ItemSelection::GetItems<T>()` filters again. Fixed the test (`{}`, as
   `InitialState` already spelled it), not production. Expect the same when migrating any
   remaining `CastItems`: it surfaces tests that fabricate impossible inputs, not real bugs.
5. **Small local fixes.** `selectable_combobox_editor.cpp:61` was a real (if mild) bug: it
   `const_cast`-ed the caller's `const QStyleOptionViewItem&` and wrote into it from inside a
   `paint()` override. Now copies.
6. **`StaticCastItems` deleted** from `container_utils.h` together with its only test.

## Round B — DONE

sup-mvvm `c1adabf3`; sequencer-gui `db326962`, `e3881fcb`, `97e573f2`; sup-gui-core `6cabe57d`.

7. **`AbstractItemView::GetSelected<T>()`** was indeed ill-formed if instantiated, and has zero
   users in any of the three packages. Now `const T*`, forwarding to `GetSelectedItem()` — no new
   includes needed. (Revisit when #13 lands: it can return `T*` then.)
9. **`ProcedureItemJobInfoBuilder`** — `InstructionTree::indexes` and both `PopulateWorkspaceItem`
   overloads create the items they return, so the `const` was gratuitous. Dropped.
11. **`NodePort`** now stores `vector<INodeConnection*>`. A port mutates its connections
    (`ForgetPort`, `UpdateConnectionShape`), so storing them const and laundering was the dishonest
    part. `AppendConnection`/`RemoveConnection` take non-const; `GetConnections() const` narrows via
    `ConvertItems`; a non-const overload lets `ConnectableShape` drop its cast.

### Round B deviations — read before re-planning

8. **Three of four local pairs done.** `SequencerSettingsModel::Get<T>()` was **not** done: it is
   built on `mvvm::utils::FindItemDown`, which is const-only, as is the `iterate_if` traversal
   beneath it. `FindItemDown` has **exactly one production user across all three packages** — this
   call site. Adding a parallel non-const traversal to sup-mvvm to retire one `const_cast` is
   disproportionate; fold it into #15.
10. **Deferred wholesale to #13.** Only `anyvalue_editor_widget.cpp:180` was fixed, and it turned
    out to be a dead cast — `mvvm::utils::RemoveItem` already takes `const SessionItem&`. The
    "launder through the owned model" idea (`model->FindItem(item->GetIdentifier())`) was
    implemented and then **reverted by decision**: looking an item up by its own identifier to
    obtain a non-const pointer to that same item is indirection that hides the real boundary and
    costs a lookup, and for `dto_compare_view`/`dto_composer_view` it additionally needs
    `dynamic_cast<ConfigurationItem*>` on the result — trading a `const_cast` for RTTI, which does
    not serve this round's goal. All five sites are the same boundary; they are #13's job.
12. **Mostly already done, and partly not desirable.** The clean
    `dynamic_cast<T*>(x->FindItem(tag))` → `x->FindItem<T>(tag)` drop-ins were retired by the
    original API migration — **zero remain**. What is left in the test trees is: Qt widget/model
    casts (`QLabel*`, `QTransposeProxyModel*`), casts on `unique_ptr::get()` results from clone and
    mime reconstruction (not item accessors at all), and the `standard_variable_items_tests`
    cases — which are named `...PropertyAppearance` and deliberately assert both the set **and the
    order** of registered properties. Making those "order-independent" would weaken real coverage;
    left alone deliberately. Seven positional downcasts in `workspace_editor_action_handler_tests`
    and `procedure_list_action_handler_tests` were converted to
    `GetItem<T>(mvvm::TagIndex::Default(n))`, which also deleted seven `ASSERT_NE(..., nullptr)`
    lines.

## The shared-implementation pattern (reusable for #14 and #15)

Five places now share one body between a const and a non-const overload with **no cast**, using a
file-local template in an anonymous namespace where the type parameter carries the constness:
`CollectVisibleChildren` (item_utils), `CollectVisibleGraphItems` (graph_viewport_item),
`CollectAllItems` (tagged_items), `GetDropTargetImpl` (two sequencer-gui view models),
`FindInstructionsImpl` (sequencer-gui test utils).

Rule of thumb:

- Constness reaching you through a **reference or value** (`SessionItem&`, `ISessionModel&`,
  `*this`) — plain template deduction plus `decltype` is enough:
  `template <typename SelfT> auto Impl(SelfT& self)`.
- Constness reaching you through a **`unique_ptr`** — deduction silently gives you the non-const
  overload, because `unique_ptr` does not propagate const. Select explicitly with
  `std::conditional_t`, as `CollectAllItems` does, or the const path will quietly call non-const
  code.

Prefer this over both `const_cast` and copy-pasted bodies. Prefer a plain duplicated 2-line body
over machinery when the body is trivial.

---

# Round C — the three root causes (do in this order)

#13 is done. #14 and #15 remain, in that order; #15 is the largest and unblocks the leftovers
noted in #8 and #14.
## 13. `ItemSelection` const boundary — DONE

Implemented as decided below. Result: **one** `const_cast` added inside `ItemSelection`,
**18 removed** downstream (sup-gui-core 7 → 0, sequencer-gui 18 → 7). `ItemSelection`'s storage,
constructors, `ItemViewComponentProvider` and the `SelectionChanged` signature are untouched, so
the single Qt metatype is unchanged and every `SetSelected...(const T*)` signature kept its const.

### Diagnosis

`ItemSelection` was used in **two opposite directions with one type**:

- **Inbound** (`SetSelection`, 12 construction sites): selecting an item only *reads* it. Backed by
  ~14 `SetSelected...(const T*)` entry points and by `notify_request`-style callbacks carrying
  `const SessionItem*`. That const is deliberate and correct.
- **Outbound** (`GetSelection()`, the `SelectionChanged(const ItemSelection&)` signal): consumers
  *edit* what the user picked. Every one of the ~22 casts lived here.

Nothing in the chain owns the items: `ItemSelection` owns nothing, `ItemViewComponentProvider`
holds `std::unique_ptr<ViewModel>`, `ViewModel` is a proxy owning no data, the model owns the
items. Neither `unique_ptr` nor raw pointers propagate const, so the declared types were simply
over-promising.

### What was done

The cast is concentrated inside `ItemSelection`, the class invented to absorb exactly this:

- storage and constructors **unchanged** (`std::vector<const SessionItem*>`, accepting const items);
- the four accessors now return mutable items — `SessionItem* GetItem() const`,
  `std::vector<SessionItem*> GetItems() const`, and the two templated forms — carrying one
  documented conversion (`const_cast` in `GetItem`, `utils::RemoveConst` in `GetItems`);
- `AbstractItemView::GetSelectedItem`/`GetSelectedItems`/`GetSelected<T>` widened, since they only
  forward;
- downstream: selection-reporting accessors and the 4 context structs widened
  (`selected_items_callback`, `selected_instructions`, `selected_job`), and the casts deleted.
  Read-only consumers that legitimately want const (`CreateInstructionTreeCopyMimeData`,
  `GetBottomLevelSelection`, ...) take `utils::ConvertItems<const T>(...)` at the call site.

Accepted cost: this is laundering and it does not compose — wrapping a genuinely const item in an
`ItemSelection` gets mutable access back. In practice items live in non-const models; the same
reasoning already backs `ISessionModel::FindItem(const std::string&) const`.

### Rejected alternative (prototyped, then reverted — do not retry)

Making `ItemSelection` store `std::vector<SessionItem*>` and adding
`SetSelection(const SessionItem*)` / `SetSelection(const std::vector<const SessionItem*>&)`
overloads to the provider. It removes every cast, but gives the provider a **parallel API on raw
`SessionItem` pointers alongside `ItemSelection`**, defeating the two reasons the class exists: one
uniform type for the const/non-const question, and one Qt metatype so no `Q_DECLARE_METATYPE` is
needed per item type. Measured cost of that variant was 3 break sites in sup-gui-core and 8 in
sequencer-gui, against 1 and 3 for the design that shipped.

### The metatype finding — the strongest argument for ItemSelection-as-currency

Widening the remaining bare-pointer signals (`JobSelected(const JobItem*)` and friends) to
`JobItem*` **fails to build**: `redefinition of QMetaTypeId<oac_tree_gui::JobItem*>`, plus
"specialization after instantiation". Metatypes are registered per *type and per constness*, and
sequencer-gui has three separate ones in three headers —
`Q_DECLARE_METATYPE(const InstructionItem*)` in `abstract_job_handler.h`,
`Q_DECLARE_METATYPE(const ProcedureItem*)` in two view .cpp files, and
`Q_DECLARE_METATYPE(JobItem*)` in `operation_splittable_widget.h`. Changing a signal's pointer
constness therefore needs a new registration and runs into include-order collisions.

Carrying `ItemSelection` instead costs **one** metatype regardless of item type or constness. This
is concrete evidence for the design, and the reason the attempt above was reverted rather than the
metatype declarations restructured.

### Postponed: selection signals still carrying bare item pointers

5 casts remain in sequencer-gui behind Qt signals. They split by what the signal *means*:

- **Genuine selection notifications — switch to `ItemSelection`** (3 casts):
  `InstructionEditorWidget::InstructionSelected(const InstructionItem*)`
  (`instruction_editor_widget.cpp:241`), `JobListWidget::JobSelected(const JobItem*)`
  (`operation_job_panel.cpp:113`), `OperationJobPanel::JobSelected`
  (`operation_monitor_view.cpp:314`). These say "the user picked something", which is what
  `ItemSelection` is for, and one already-registered metatype replaces three per-type ones.
- **Commands carrying a subject — leave them** (2 casts): `SubmitProcedureRequest(const
  ProcedureItem*)` is "submit *this* one", not a selection; `ToggleBreakpointRequest(JobItem* job,
  const InstructionItem* instruction)` carries two arguments in *different roles*, which a flat
  selection cannot express. Give these non-const parameters if the casts are worth removing, but
  do not force `ItemSelection` onto them.

## 14. `IChildrenStrategy::GetChildren(const SessionItem*) const → vector<SessionItem*>`

Still the last bucket-C cluster inside our own code: **5 `RemoveConst` in sup-mvvm, 7 in
sequencer-gui, 1 in sup-gui-core**. Splitting it into const/non-const would clear all 13; the
shared-implementation pattern above is the tool for it.

## 15. `SessionItem::GetParent() const` and `GetModel() const` return mutable

44 and 49 src call sites. The root enabler: much "can't fix locally" debt becomes fixable once a
const item yields a const parent — including the command classes' `FindItem/FindParent() const`
accessors, parts of #14, and `SequencerSettingsModel::Get<T>()` from #8 (which needs a non-const
`FindItemDown`, and under it a non-const `iterate_if`). Largest single change; sequence it last.

Note the distinction this round settled: **ownership decides whether const should propagate.**
`SessionItem` owns its children through `unique_ptr`, so const propagation is meaningful there -
that is why #15 is a real fix. `ItemSelection`, the providers and the view models own nothing, so
const propagation through them was never protecting anything — that is why #13 is a relabelling
rather than a weakening.

Also still worth doing: **`ISessionModel::FindItem<T>(const std::string& id)`**. It is the one
lookup with no typed form, so `dynamic_cast<T*>(model->FindItem(id))` remains the idiom at ~4 sites
(including `LinkedItem::Get<T>()`).
