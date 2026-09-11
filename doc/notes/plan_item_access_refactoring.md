# SessionItem item-access API refactoring

> Working note. **Status: the API change is DONE and committed in all three packages.**
> sup-mvvm 1370/1370, sup-gui-core 447/447, sequencer-gui 890/890 tests pass.
> Remaining work is the follow-up const-correctness rounds at the end of this document.

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

# Next rounds: const-correctness backlog

Survey taken after the API change landed. Goal of these rounds: **reduce `const_cast` /
`std::as_const` / `RemoveConst` / `MakeConst` / `StaticCastItems`, and replace `dynamic_cast`
with `GetItem<T>` / `FindItem<T>` where the operand is an item accessor result.**

Current totals (`src/` + `test/`):

| package | `const_cast` | `RemoveConst` | `MakeConst` | `CastItems` misuse |
|---|---|---|---|---|
| sup-mvvm | 33 | 14 | 0 | 0 |
| sup-gui-core | 10 | 3 | 0 | 6 |
| sequencer-gui | 24 | 8 | 2 (legit) | 6 |

`StaticCastItems`: **0 production users anywhere** — dead API.
`ConvertItems`: **0 production users anywhere** — added but never adopted; the 12 `CastItems`
misuses below are exactly the gap it was created to fill.

## Round A — free (zero caller churn). Removes ~35 hacks.

1. **sup-mvvm: write the 10 owner-side non-const accessors directly.** `session_item.cpp:341,356`;
   `session_item_container.cpp:135`; `tagged_items.cpp:126,205,236,241`; `viewmodel_base.cpp:123,128`;
   `viewitem.cpp:216`. These classes own their storage through `unique_ptr`/`p_impl`, so the
   non-const overload can return it directly instead of `const_cast`-ing the const overload's
   result. **−10 `const_cast`.** (Introduced by this refactor — worth undoing now.)
2. **sup-mvvm: same for the 7 owner-side `RemoveConst`s.** `session_item_container.cpp:60`
   (→ `GetVectorOfPtrs(m_items)`), `tagged_items.cpp:136,153`, `session_item.h:598` (add a
   non-const private `FindItemsImpl`), `item_utils.cpp:268,286`, `graph_viewport_item.cpp:77`.
   **−7 `RemoveConst`.**
3. **Delete the dead casts.** sup-mvvm `abstract_item_view.cpp:68`, `viewmodel_controller.cpp:70`;
   sup-gui-core `waveform_editor_widget.cpp:146`, `domain_anyvalue_builder.cpp:185` (a no-op
   `const_cast<const AnyValueItem*>` left over from the `GetChildren` change);
   sup-gui-core `dto_waveform_action_handler_tests.cpp:144,186` (redundant `dynamic_cast`).
   **−4 `const_cast`, −2 `dynamic_cast`.**
4. **Adopt `ConvertItems` at the 12 `CastItems` upcast/add-const sites.** sup-gui-core:
   `anyvalue_item_copy_helper.cpp:51` (**and delete its stale FIXME** — the `MakeConst` half is
   already gone and no downcast is ever needed there), `anyvalue_editor_action_handler.cpp:140`,
   `configuration_splittable_widget.cpp:113`, 3 test copies. sequencer-gui:
   `copy_and_paste_helper.cpp:56,77`, `workspace_editor_action_handler.cpp:201`,
   `instruction_editor_action_handler.cpp:181`, `procedure_splittable_editor_widget.cpp:257`,
   `operation_splittable_widget.cpp:206`. Buys back the `reserve`+`transform` fast path and the
   "no element silently dropped" guarantee. **−12 RTTI abuses.**
5. **Small local fixes.** sup-mvvm `item_view_component_provider.cpp:180` (make the local
   `vector<const SessionItem*>`), `selectable_combobox_editor.cpp:61` (copy the Qt option instead
   of casting); sup-gui-core `custom_row_waveform_strategies.cpp:41,77` (the `const` in the
   `dynamic_cast` is gratuitous, then stripped again). **−3 `const_cast`, −1 `RemoveConst`.**
6. **Delete `StaticCastItems`** from `container_utils.h` — zero production users; it exists only
   to be unit-tested.

## Round B — cheap, small blast radius

7. **sup-mvvm `AbstractItemView::GetSelected<T>()`** (`abstract_item_view.h:75-78`) is **ill-formed
   if ever instantiated** — it `dynamic_cast<T*>`s a `const SessionItem*`. Compiles today only
   because nothing instantiates it. Make it `const T* GetSelected() const` forwarding to
   `ItemSelection::GetItem<T>()`.
8. **sequencer-gui, 4 local pairs.** `FlatListViewModel`/`FavoriteInstructionsViewModel::GetDropTarget`
   → return non-const (both private, 4 call sites in the same 2 files); `SequencerSettingsModel::Get<T>()`
   → const/non-const pair; `InstructionTaskMonitor::GetTaskWidgetBuilder()` → pair;
   `test::FindInstructions` → non-const overload. **−5 `const_cast`.**
9. **sequencer-gui `ProcedureItemJobInfoBuilder`** — `InstructionTree::indexes` and
   `PopulateWorkspaceItem` return `const` vectors of items *the functions themselves just created*.
   Drop the `const`. **−2 `const_cast`.**
10. **sup-gui-core, 5 selection sites** (`settings_editor.cpp:67`, `dto_compare_view.cpp:118`,
    `dto_composer_view.cpp:56`, `anyvalue_editor_widget.cpp:180`,
    `anyvalue_editor_action_handler.cpp:394`) can each be laundered legally through the owned
    non-const model: `model->FindItem(selection->GetIdentifier())`. **−5 `const_cast`** without
    touching any signature.
11. **sup-mvvm `NodePort::m_connections`** → non-const, `GetConnections() const` returns const via
    `ConvertItems`. **−2 `RemoveConst`, −1 `const_cast`.**
12. **Test sweep: `dynamic_cast` → `GetItem<T>`/`FindItem<T>`**, ~15 sites in sequencer-gui tests
    plus a few in sup-gui-core. Also deletes the now-redundant `ASSERT_NE(…, nullptr)` lines and
    makes `standard_variable_items_tests` order-independent.

## Round C — the three root causes (do last, in this order)

13. **`ItemSelection` / `ItemViewComponentProvider` are const-only** while every downstream
    `Set…`/context callback takes non-const. This single boundary drives **12 `const_cast` in
    sequencer-gui and 5 in sup-gui-core**. Fix upstream first (non-const `GetSelection()` /
    `GetItemFromViewIndex()` overloads on the provider, non-const accessors on `ItemSelection`),
    then flip sequencer-gui's 4 context structs. Doing the downstream half alone only *migrates*
    the casts into the 6 providers.
14. **`IChildrenStrategy::GetChildren(const SessionItem*) const → vector<SessionItem*>`** — the
    last bucket-C cluster inside our own code: **5 `RemoveConst` in sup-mvvm, 7 in sequencer-gui,
    1 in sup-gui-core**. Splitting it into const/non-const would clear all 13.
15. **`SessionItem::GetParent() const` and `GetModel() const` return mutable** (44 and 49 src call
    sites). This is the root enabler: much "can't fix locally" debt — including the command
    classes' `FindItem/FindParent() const` accessors and parts of #14 — becomes fixable once a
    const item yields a const parent. Largest single change; sequence it last.

Also worth doing at some point: **`ISessionModel::FindItem<T>(const std::string& id)`**. It is the
one lookup with no typed form, so `dynamic_cast<T*>(model->FindItem(id))` remains the idiom at
~4 sites (including `LinkedItem::Get<T>()`).
