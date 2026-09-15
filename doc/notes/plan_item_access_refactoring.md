# SessionItem item-access API refactoring

> Working note. **Status: DONE.** The item-access API change and all const-correctness rounds are
> complete in all three packages. sup-mvvm 1369/1369, sup-gui-core 447/447, sequencer-gui 890/890
> tests pass. `const_cast` went 33 → 8, 10 → 0 and 24 → 4; what is left is listed under
> "What remains" and is mostly Qt and ownership boundaries rather than debt.

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
  cannot drop elements. (`StaticCastItems` was deleted in the const rounds below as dead API.)
- const/non-const pairs on `SessionItemContainer`, `TaggedItems` (plus `GetItem`→`FindItem`
  rebuilt on `FindContainer`, `const` dropped from `InsertItem`/`TakeItem`, `GetTotalItemCount`
  added), `SessionItem`, `ISessionModel::GetRootItem`, `item_utils` (`ChildAt`, `TopLevelItems`,
  `SinglePropertyItems`), `model_utils` (`GetTopItems`, `GetTopItem`, `FindItems`, `ItemFromPath`).
- `CompoundItem::Property`/`SetProperty` no longer dereference a possibly-null pointer.
- `GroupItem` split into `GetCurrentItem<T>()` (reference, throws) and `FindCurrentItem<T>()`
  (pointer, nullptr); `SetCurrentType<T>()` returns a reference.
- `GetTotalItemCount` no longer allocates a vector.
- The 5 `IChildrenStrategy` implementations used `utils::RemoveConst` at this point; the const
  rounds below removed that by giving the interface a non-const item.

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

## Boundaries left open by the API change

`IChildrenStrategy::GetChildren` was left as "const item in, mutable children out" here, and
`ItemSelection` / the view-model proxies were left const-only. The const-correctness rounds below
closed all of those. What is still open is listed once, under "What remains" at the end.

One boundary deserves its note here because a fix was already prototyped:
**`UniversalInstructionItem::GetAttributeItems` / `UniversalVariableItem::GetAttributeItems`** keep
a `RemoveConst` and a FIXME pending a separate attribute-system refactor. A const/non-const pair
works (the two consumers divide cleanly: `InitFromDomainImpl` writes, `SetupDomainImpl` reads), but
it needs a templated `Attribute` struct and was judged not worth the machinery yet.

## Verification

```bash
cmake --preset debug-gcc && cmake --build --preset debug-gcc
ctest --test-dir out/build/debug-gcc
grep -rn "(void).*GetItem" src/ test/      # must stay empty - the one silent-change pattern
grep -rn "const_cast" src/ | wc -l         # see "What remains" for the expected count
```

Downstream packages build against the sup-mvvm build tree via the CMake package registry.
There was no version bump, so a stale downstream build fails with template errors at the call
sites rather than a clean `find_package` message.

---

# Const-correctness rounds — COMPLETE

Goal was to reduce `const_cast` / `RemoveConst` / `MakeConst` / `StaticCastItems` and, where the
operand was an item accessor result, `dynamic_cast`.

| package | `const_cast` | `RemoveConst` | `StaticCastItems` | `CastItems` misuse |
|---|---|---|---|---|
| sup-mvvm | 33 → **8** | 18 → **4** | 3 → **0** | 0 |
| sup-gui-core | 10 → **0** | 3 → **1** | 0 | 6 → **0** |
| sequencer-gui | 24 → **4** | 9 → **2** | 0 | 6 → **0** |

Tests green throughout: sup-mvvm 1369/1369, sup-gui-core 447/447, sequencer-gui 890/890.
(sup-mvvm lost one test with the `StaticCastItems` deletion.)

## The principle that emerged — ownership decides whether const propagates

This settled every question in these rounds, and settled them in *both* directions:

- **An object that owns what it hands out must propagate const.** `SessionItem` owns its children
  through `unique_ptr`, so `GetParent()` / `GetModel()` had to become const/non-const pairs — they
  were escape hatches through which a `const SessionItem&` reached the whole mutable tree.
- **An object that owns nothing must not.** `ItemSelection`, `ItemViewComponentProvider`,
  `ViewModel`, `IViewModelController` and the presentation items are non-owning handles over a
  model's tree. Their constness is about the handle, not the items — narrowing there only pushed
  casts onto every consumer.

Corollary worth remembering: before building machinery to preserve a const parameter, check whether
the parameter should have been const at all. An accessor that must return mutable state is often
telling you its *input* was mistyped. `IChildrenStrategy::GetChildren` was the clean example — it
needed a non-const item, not an overload pair.

## The shared-implementation pattern

Used throughout to give a const and a non-const overload one body with no cast: a file-local (or
private static) template where the type parameter carries the constness.

- Constness arriving through a **reference or value** (`SessionItem&`, `ISessionModel&`, `*this`) —
  plain deduction plus `decltype` suffices: `template <typename SelfT> auto Impl(SelfT& self)`.
- Constness arriving through a **`unique_ptr`** — deduction silently gives you the non-const
  overload, because `unique_ptr` does not propagate const. Select explicitly with
  `std::conditional_t` (`TaggedItems::CollectAllItems`), or the const path quietly calls non-const
  code.
- A `dynamic_cast` whose **target type** must pick up the constness also needs `std::conditional_t`
  (`SessionItem::FindItemImpl`, `utils::FindItemUpImpl`).

Prefer this over both `const_cast` and copy-pasted bodies; prefer a plain duplicated one-liner over
machinery when the body is a single expression.

## Findings worth keeping

- **`CastItems` silently drops `nullptr`** (`dynamic_cast<T*>(nullptr)` fails its `if` check);
  `ConvertItems` preserves it by design. Migrating between them surfaces **tests that fabricate
  inputs production cannot produce**, not real bugs. It made one test segfault; the fix was the
  test.
- **Latent bugs the rounds exposed, all fixed:** `selectable_combobox_editor` wrote through the
  caller's `const QStyleOptionViewItem&` inside a `paint()` override, and
  `AbstractItemView::GetSelected<T>()` was ill-formed if ever instantiated (it `dynamic_cast`-ed a
  `const SessionItem*` to a non-const type; it compiled only because nothing instantiated it).
  `utils::RemoveItem` / `MoveUp` / `MoveDown` were also de-consted during the rounds, but whether
  that was the right call is the open question below.
- **Qt registers metatypes per type *and per constness*.** Widening a bare-pointer signal from
  `const JobItem*` to `JobItem*` fails to build (`redefinition of QMetaTypeId<JobItem*>`) because
  sequencer-gui declares `const InstructionItem*`, `const ProcedureItem*` and `JobItem*` in three
  separate headers. Carrying `ItemSelection` costs **one** metatype regardless of item type or
  constness — this is the concrete argument for the class, beyond the const question.

## Decisions not to re-litigate

- **`ItemSelection` keeps its storage and constructors const, and its four getters return mutable
  items**, with one documented conversion inside. It is a non-owning bundle: constructors accept
  const items (selecting only reads one), getters hand out mutable ones (consumers act on what the
  user picked). One cast behind a named abstraction, against ~20 scattered over call sites.
  - **Rejected, prototyped, do not retry:** storing `vector<SessionItem*>` and adding
    `SetSelection(const SessionItem*)` overloads to the provider. It removes every cast but gives
    the provider a **parallel API on raw item pointers alongside `ItemSelection`**, defeating both
    reasons the class exists — one uniform type, and one Qt metatype. Measured cost was also worse
    (3 + 8 downstream break sites versus 1 + 3).
- **`ISessionModel::RemoveItem(const SessionItem*)` keeps its const-taking signature**, with one
  documented conversion inside `SessionModel::RemoveItem`. Asking the owner to remove something
  should not require write access to it — that is `erase(const_iterator)`. De-consting the virtual
  would push casts back onto ~15 call sites.
- **Signals that report a selection carry `ItemSelection`**; signals that issue a command carrying
  a subject keep a plain pointer. `SubmitProcedureRequest` is "submit *this* one", not a selection;
  `ToggleBreakpointRequest(JobItem*, InstructionItem*)` carries two arguments in different roles,
  which a flat selection cannot express.

---

# What remains

8 / 0 / 4 `const_cast`. Most are boundaries rather than debt.

## Open design question — const on parent-side operations

**Should an operation that reorders or removes a child take a const item?**

These rounds answered it inconsistently, and the inconsistency sits on one interface:

```cpp
virtual void RemoveItem(const SessionItem* item) = 0;                                   // const
virtual void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex&) = 0; // not
```

Same interface, same kind of operation, different constness. Correspondingly
`utils::RemoveItem` / `MoveUp` / `MoveDown` were changed to take `SessionItem&`, while
`ISessionModel::RemoveItem` kept its const parameter with one documented recovery inside
`SessionModel::RemoveItem`.

**The case for const** (and the position this note now leans to):

- Moving a child does not modify the child. It modifies the **parent's** ordering; the item
  argument only says *which* child. That is `erase(const_iterator)` — the authority comes from the
  container, not the element.
- `SessionModel::MoveItem` never mutates through the pointer: it validates with
  `utils::GetMoveOperationInfo(const SessionItem&, const SessionItem&, ...)`, which is already
  const, then calls `TakeItem` / `InsertItem` on the model.
- The original code carried `// FIXME: modify MoveItem API so it accepts const item to move`. The
  rounds removed that FIXME by de-consting the *caller* — the opposite of what it asked for.
- The counter-argument that `GetTagIndex()` is observable item state and changes does not survive:
  `RemoveItem` destroys the item outright and still takes a const handle.

**What changing it would involve:** `ISessionModel::MoveItem(const SessionItem* item,
SessionItem* new_parent, const TagIndex&)` — `new_parent` stays mutable, since it is the container
being modified, so the signature says which argument is subject and which is identifier. Then
`utils::MoveTo` / `MoveUp` / `MoveDown` / `RemoveItem` go back to `const SessionItem&`. The
model path in `MoveTo` needs no cast; the parent-direct paths (used when the item has no model)
need one documented recovery each, the same shape as the one already in `SessionModel::RemoveItem`.
Roughly cast-neutral, but the API becomes consistent.

## Actionable, if a further round is ever wanted

- **`LineSeriesDataController::m_active_point`** (`line_series_data_controller.cpp:204`) — the
  controller observes a **const** data item but stores a mutable `PointItem*` because dragging must
  modify it. The only remaining sup-mvvm cast that is neither idiom nor boundary: either the
  controller should observe a mutable data item, or dragging should go through the model.
- **`AnyValueItem::GetChildren()`** (sup-gui-core, `anyvalue_item.cpp:83`) — the last
  const-overload forwarding site anywhere, `RemoveConst(std::as_const(*this).GetChildren())`. The
  shared-implementation pattern above applies directly; the equivalent 11 sites in sup-mvvm were
  cleared that way.
- **Non-const `FindItemDown` / `iterate_if`** in sup-mvvm — the only blocker for
  `SequencerSettingsModel::Get<T>()` (`sequencer_settings_model.h:78`). `FindItemDown` has exactly
  one production user across all three packages, so this is worth doing only alongside something
  else.
- **Two sequencer-gui signals** (`operation_monitor_view.cpp:268,314`) — the second hop of the
  `JobSelected` chain (`ItemSelection` applies, as done for the first hop and for
  `InstructionSelected`), and `SubmitProcedureRequest`, which wants a non-const parameter rather
  than `ItemSelection`.

## Accepted boundaries — leave alone

- **Qt (3, sup-mvvm):** `viewmodel_base.cpp:139` (`createIndex` takes `void*`),
  `viewmodel_utils.cpp:162,186` (`QModelIndex::model()` returns const by Qt's design). Removable
  only by collapsing `ViewModel`'s item accessors to const-methods-returning-mutable — a separate
  API decision.
- **Deliberate (2, sup-mvvm):** inside `ItemSelection` and `SessionModel::RemoveItem`, per the
  decisions above.
- **Infrastructure:** the `RemoveConst` definition in `container_utils.h` and its unit test; one
  test helper (`test_viewmodel_helper.cpp:33`).
- **External API (1, sequencer-gui):** `domain_procedure_builder.cpp:142` —
  `procedure->GetWorkspace()` belongs to oac-tree.
- **Attribute system (2, sequencer-gui):** the `RemoveConst` in `universal_instruction_item` /
  `universal_variable_item`, waiting on that refactor; their FIXME still stands.
- **`MakeConst` (2, sequencer-gui):** `drag_and_drop_helper` — legitimate narrowing.

## Still worth doing at some point

**`ISessionModel::FindItem<T>(const std::string& id)`** — the one lookup with no typed form, so
`dynamic_cast<T*>(model->FindItem(id))` remains the idiom at ~4 sites (including
`LinkedItem::Get<T>()`).

Also unchanged from the original API work: `LinkedItem::Get<T>() const` still returns a non-const
`T*` (the link points outside the subtree, a different question from child ownership), and
`SessionItemContainer::begin()/end()` expose `const_iterator` over
`vector<unique_ptr<SessionItem>>`, so `(*it)->SetData(...)` still mutates through a const container.

