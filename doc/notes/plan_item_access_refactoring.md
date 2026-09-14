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
| sup-mvvm | 33 → **19** | 18 → **4** | 0 | 0 | 3 → **0** |
| sup-gui-core | 10 → **0** | 3 → **1** | 0 | 6 → **0** | 0 |
| sequencer-gui | 24 → **4** | 9 → **2** | 2 (legit) | 6 → **0** | 0 |

sup-mvvm's `const_cast` count barely moved, but its composition changed completely — see
"What remains" at the end. sup-gui-core is at **zero**.

`RemoveConst` is now down to its legitimate uses: the one inside `ItemSelection` (#13), the
definition in `container_utils.h` plus its unit test, and the two in sequencer-gui's
`universal_instruction_item` / `universal_variable_item` — the documented attribute-system boundary
that waits on its own refactor.

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

#13 and #14 are done. #15 remains: the largest, and it unblocks the leftover noted in #8.
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

### Selection signals carrying bare item pointers

Signals that report a selection should carry `ItemSelection`: one already-registered metatype
replaces a per-type, per-constness one, and consumers get mutable items without a cast.

This is not a new convention: four signals already carried `ItemSelection` before this work —
`procedureSelectionChanged` (in two widgets), `focusWidgetProcedureSelectionChanged` and
`focusWidgetJobSelectionChanged`.

**Done:**

- `InstructionEditorWidget::InstructionSelected` now carries `const mvvm::ItemSelection&`, and
  `GetSelectedInstruction()` widened to `InstructionItem*` with it, so the cast at the
  `SetInstruction` call is gone. The signal had no connected consumers, which made it a safe first
  example of the pattern.
- `JobListWidget::JobSelected` likewise, together with the slot it feeds,
  `OperationJobPanel::OnJobSelectedIntern`, which now takes the job out of the selection. That
  retired the cast at `operation_job_panel.cpp:113`. `JobListWidget::GetSelectedJob()` is exactly
  `GetSelection().GetItem<JobItem>()`, so forwarding the selection preserved semantics precisely.
  `OperationJobPanel::JobSelected` still emits `const JobItem*` and was left alone.

**Postponed:** `OperationJobPanel::JobSelected` -> `OperationMonitorView::OnJobSelected`
(`operation_monitor_view.cpp:314`), the second hop of the same chain.

**Leave as they are — commands carrying a subject:** `SubmitProcedureRequest(const ProcedureItem*)`
is "submit *this* one", not a selection. Do not force `ItemSelection` onto these.

### The `ToggleBreakpointRequest` inconsistency — DONE, and a simpler fix than `ItemSelection`

`ToggleBreakpointRequest` is declared four times along one relay chain:
`realtime_instruction_tree_widget.h:73` -> `operation_realtime_widget.h:67` ->
`operation_tab_widget.h:95` -> `operation_splittable_widget.h:128`, gaining the job as it relays
upward. The last two are declared
`ToggleBreakpointRequest(JobItem* job, const InstructionItem* instruction)` — **mutable job, const
instruction, in the same signal**. Every sibling signal in those same headers (`RunRequest`,
`PauseRequest`, `StepRequest`, `StopRequest`, `ResetRequest`, `ChangeDelayRequest`) takes a plain
`JobItem*`, and the terminal consumer,
`AbstractJobHandler::OnToggleBreakpointRequest(InstructionItem*)`, takes a **non-const**
instruction. So the instruction is const only in transit, and the cast at
`operation_monitor_view.cpp:293` exists purely to undo it.

The stray `const` was dropped across all four declarations, which made the signal match its own
neighbours and retired the cast at `operation_monitor_view.cpp:293` — no `ItemSelection` involved,
which is right: `ToggleBreakpointRequest` is a command with two arguments in different roles, and a
flat selection cannot express that.

No metatype work was needed. The `Q_DECLARE_METATYPE(const InstructionItem*)` in
`abstract_job_handler.h` belongs to the job system, not to this chain; nothing spies on
`ToggleBreakpointRequest` with `QSignalSpy` and every connection in the relay is direct, so no
registration of a non-const form was required. Worth re-checking that if one of these connections
ever becomes queued.

## 14. `IChildrenStrategy::GetChildren` — DONE

All **13** strategy `RemoveConst` cleared (5 sup-mvvm, 7 sequencer-gui, 1 sup-gui-core).

**The fix was not the one this plan assumed.** No const/non-const split, no shared-implementation
helper. The contract was never "const item in, mutable children out" by necessity — it was simply
over-promising on the *input*. Mutable children cannot honestly be derived from a const parent, so
the parameter was made to match the return type:

```cpp
virtual std::vector<SessionItem*> GetChildren(SessionItem* item) const = 0;
```

Every implementation then just calls `item->GetAllItems()` / `TopLevelItems` /
`SinglePropertyItems` and the casts evaporate. All 10 implementations got *simpler*, rather than
doubling into overload pairs.

The feasibility question was the whole job, and it came out clean: every caller that needs mutable
children already holds a non-const item — `ItemInsertedEvent::item` is `SessionItem*`,
`CreateTreeOfRows(SessionItem&)`, `RebuildGroupBranch(SessionItem&)`,
`ConnectableViewModelController::m_root_item`. Only two read-only lookup helpers took const parents
(`ViewModelController::GetInsertViewIndexOfChild`,
`ConnectableViewModelController::IsChildNeedsView`), and both are called with non-const items
anyway, so de-consting their parameters cost nothing.

Five `const SessionItem item;` test locals had to be de-consted — the same signal seen throughout
these rounds: fixtures declaring const and then handing the object to something that legitimately
needs mutability.

**Generalisable lesson for #15:** before building machinery to preserve a const parameter, check
whether the parameter should have been const at all. An accessor that must return mutable state is
often telling you its input was mistyped, not that it needs an overload pair.

## 15. `SessionItem::GetParent()` / `GetModel()` — DONE

Both are now const/non-const pairs. The reasoning that settled #13 the *other* way applies here:
`ItemSelection` and the providers own nothing, so their constness is not the items'. A
`SessionItem` **owns its children**, and these two were the escape hatches — from a
`const SessionItem&` you could reach the parent and from there the entire mutable tree. Const has
to propagate through them or it means nothing anywhere.

**The blast radius was far smaller than the 221 call sites suggested: 6 sites in sup-mvvm, 4 in
sup-gui-core, 4 in sequencer-gui.** Nearly every caller already held a non-const item, because the
earlier rounds had done that work — #13 made selections yield mutable items, #14 made the children
strategies take them. Sequencing this last was right, but for the opposite reason to the one
assumed: not because it is the largest change, but because the earlier rounds shrank it.

### Latent bugs it exposed — the point of the item

- **`utils::RemoveItem(const SessionItem&)`** took a const item and then did
  `item.GetParent()->TakeItem(...)` — it *destroys* the item. **`MoveUp`** and **`MoveDown`**
  likewise moved items through a const reference. All three de-consted; `MoveTo`'s `const_cast` and
  its FIXME went with them.
- Note for the record: earlier in these rounds a cast in `anyvalue_editor_widget.cpp` was removed
  on the grounds that "`RemoveItem` already takes `const&`". That was true, but the const was
  itself the bug — the cast was redundant only because the function was lying about what it does.

### Judgment call worth keeping

`ISessionModel::RemoveItem(const SessionItem*)` **keeps** its const-taking signature, with one
documented conversion inside `SessionModel::RemoveItem`. Asking the owner to remove something
should not require write access to it — that is `erase(const_iterator)`. De-consting the virtual
would have pushed casts back out to ~15 call sites, which is exactly the pattern these rounds
exist to remove. Same principle as #13: concentrate the conversion at the owning boundary.

Two smaller shifts fell out: `sup::gui::FilterOutChildren` now preserves mutability (both callers
filter items they are about to delete, so it removed a conversion rather than adding one), and
`ItemViewComponentProvider::GetItemFromViewIndex` returns non-const — #13's non-owning-proxy
argument applied to the one accessor left alone at the time to keep that diff minimal.

### Correction to this plan's earlier claim

#15 does **not** unblock `SequencerSettingsModel::Get<T>()` from #8. That site is blocked on
`mvvm::utils::FindItemDown` being const-only (and `iterate_if` under it), which is unrelated to
`GetParent`/`GetModel`. See the remaining-work section below.

Note the distinction this round settled: **ownership decides whether const should propagate.**
`SessionItem` owns its children through `unique_ptr`, so const propagation is meaningful there -
that is why #15 is a real fix. `ItemSelection`, the providers and the view models own nothing, so
const propagation through them was never protecting anything — that is why #13 is a relabelling
rather than a weakening.

Also still worth doing: **`ISessionModel::FindItem<T>(const std::string& id)`**. It is the one
lookup with no typed form, so `dynamic_cast<T*>(model->FindItem(id))` remains the idiom at ~4 sites
(including `LinkedItem::Get<T>()`).

---

# What remains

All of #13, #14 and #15 are done. 23 `const_cast` are left across the three packages (sup-gui-core
is at zero). Classified, because the composition matters more than the count:

## Actionable — one coherent cluster left

**The const-overload forwarding idiom: 11 sites in sup-mvvm.** Every one is a non-const overload
delegating to its const twin via `const_cast<T*>(std::as_const(*this).F())`:

- `viewmodel.cpp:105,122,132` — `GetRootSessionItem`, `GetSessionItemFromIndex`,
  `GetViewItemFromIndex`
- `session_item.h:550,580` — `FindItem<ItemT>`, `GetItem<ItemT>`
- `group_item.h:148,167` — `FindCurrentItem<T>`, `GetCurrentItem<T>`
- `item_utils.cpp:249` (`ChildAt`), `item_utils.h:313` (`FindItemUp<T>`),
  `model_utils.cpp:68` (`ItemFromPath`), `line_series_item.cpp:73` (`GetDataItem`)

This is the same shape Round A items 1-2 cleared for the owner-side accessors, and the tools are
already proven: write the non-const body directly where the const version does no real work, and
use the shared-implementation template where it does (see "The shared-implementation pattern").
Note `FindItemUp<T>` and the `session_item.h` / `group_item.h` ones are templates, so the
self-deducing `template <typename SelfT>` form applies directly.

**Non-const `FindItemDown` / `iterate_if` in sup-mvvm.** The one remaining blocker for
`SequencerSettingsModel::Get<T>()` (`sequencer_settings_model.h:78`, the leftover from #8).
`FindItemDown` has exactly one production user across all three packages, so this is only worth
doing as part of the cluster above, not on its own.

**`LineSeriesDataController::m_active_point`** (`line_series_data_controller.cpp:204`). The
controller observes a **const** data item but stores a mutable `PointItem*` because dragging has to
modify it. A genuine narrow-then-widen: either the controller should observe a mutable data item,
or dragging should go through the model. Small, self-contained, and the only remaining cast in
sup-mvvm that is neither idiom nor boundary.

**Two postponed signals in sequencer-gui** (`operation_monitor_view.cpp:268,314`) — the second hop
of the `JobSelected` chain (`ItemSelection` applies, as done for the first hop), and
`SubmitProcedureRequest`, which is a command carrying a subject and should get a non-const
parameter rather than `ItemSelection`. See the #13 signal section.

## Accepted boundaries — not defects

- **Deliberate and documented (2):** `item_selection.cpp:47` (#13) and `session_model.cpp:171`
  (#15). Each is one conversion concentrated at an owning boundary, bought against ~20 scattered
  casts. Do not "clean these up".
- **Qt boundaries (3):** `viewmodel_base.cpp:139` (`createIndex` takes `void*`),
  `viewmodel_utils.cpp:162,186` (`QModelIndex::model()` returns const by Qt's design). Only
  removable by collapsing `ViewModel`'s item accessors to const-methods-returning-mutable, which is
  a separate API decision.
- **The `RemoveConst` definition itself** (`container_utils.h:175`).
- **External domain API (1):** `domain_procedure_builder.cpp:142` — `procedure->GetWorkspace()`
  belongs to oac-tree, not to us.
- **One test helper:** `test_viewmodel_helper.cpp:33`.

## Other counters

- `RemoveConst`: 4 / 1 / 2. Only legitimate uses left — the definition plus its test, the one
  inside `ItemSelection`, and the two in sequencer-gui's `universal_instruction_item` /
  `universal_variable_item`, which wait on the attribute-system refactor (their FIXME still
  stands).
- `MakeConst`: definition and test only in sup-mvvm; the 2 in sequencer-gui are legitimate.
- `StaticCastItems`: gone everywhere.
- `std::as_const`: 10 / 3 / 0 — most of the sup-mvvm ones are inside the 11 forwarding sites above
  and will go with them.

