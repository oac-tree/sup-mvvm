# Refactor `SessionItem` item-access API (`GetItem` / `GetItems`)

> Working note / implementation plan. Commits are made by the repository owner — this document
> only describes the intended change and its phases.
>
> **Status:** Phases 0-3 are implemented in sup-mvvm; the full suite passes (1370/1370).
> Two deviations from the plan as written, both discovered while implementing:
> 1. A **non-template `GetItem(const TagIndex&)` pair** had to be kept. Without it,
>    `GetItem({"tag", 0})` is ambiguous, because a braced initializer matches both
>    `TagIndex(string_view, size_t)` and `string_view(const char*, size_t)`; the old non-template
>    overload used to break that tie.
> 2. `FindItems<T>` needs a **total** (never-throwing) source of children, so `SessionItem` has a
>    private `FindItemsImpl` built on `TaggedItems::FindContainer`, since `GetItems` throws on an
>    unregistered tag.
>
> The downstream migration (sup-gui-core, sequencer-gui) has **not** been done.

## Context

`SessionItem`'s item accessors have accumulated three defects that a survey of sup-mvvm,
sup-gui-core and sequencer-gui confirmed are real and actively harmful:

1. **Const-incorrectness.** All five accessors are `const` member functions returning
   **non-const** pointers (`src/lib/sup-mvvm-model/mvvm/model/session_item.h:185-217`).
   A `const SessionItem&` therefore hands out full mutable access to its subtree. This is not a
   theoretical leak: `ViewportItem::SetViewportToContent() const`
   (`standarditems/viewport_item.h:84-99`) and `LineSeriesItem::SetNamedColor() const`
   (`standarditems/line_series_item.h:92`) are `const` methods that mutate through the hole, and
   downstream there are ~15 more. The same defect exists one layer down in `TaggedItems` and
   `SessionItemContainer`.

2. **Three different answers to one question.** On a child of the wrong type,
   `GetItem<T>` **throws**, `GetItems<T>` **silently filters**, and `ItemSelection::GetItem<T>`
   returns **nullptr**. `test/testsup-mvvm-model/session_item_tests.cpp:932` already carries the
   comment `// current behavior of item<> method, consider to change`. The throw path has **zero**
   production catchers anywhere in the three packages.

3. **No nullable typed accessor.** Because there is no "try-get", downstream hand-rolls
   `HasTag(item, tag) ? GetItem<T>(tag) : nullptr` in at least 5 places, and `GroupItem` — whose
   entire purpose is heterogeneous children — cannot express "is the current item a `T`?" without
   catching an exception (`test/testsup-mvvm-model/group_item_tests.cpp:287`).

Separately, `CompoundItem::Property`/`SetProperty` (`model/compound_item.h:133-143`) dereference
`GetItem()` with no null check — latent UB that this change fixes for free.

**Intended outcome:** one coherent, const-correct item-access API on `SessionItem`, with the
throw/nullptr split made explicit and uniform, landed in sup-mvvm first.

### Decisions taken

- **`Get…` is strict, `Find…` is total.** `Get…` throws and returns a **reference**;
  `Find…` never throws and returns a **pointer** (or a filtered vector). "Failure" means
  unregistered tag, out-of-range index, *or* failed cast — all three, uniformly, for both
  families. This law is already in the repo: `TaggedItems::GetContainer` throws while
  `FindContainer` returns nullptr (`model/tagged_items.h:197-205`), and `FindItemUp` /
  `FindNextSibling` / `ISessionModel::FindItem` are all nullable.
- **Reuse the name `GetItem` for the reference-returning form** rather than adding `GetItemRef`.
  `SessionItem`'s copy ctor is deleted (`session_item.h:59`), so *every* legacy pattern against the
  old pointer version fails to compile — `auto* p =`, `auto x =`, `if (…)`, `->`, `== nullptr`.
  This buys 100% compiler detection. A new name would leave `GetItem<T>` alive with silently
  drifting semantics.
- **Depth:** `SessionItem` + `TaggedItems` + `SessionItemContainer` + `ISessionModel::GetRootItem`.
  `IChildrenStrategy` is deliberately **left alone**.
- **`container_utils`:** add `ConvertItems` only. Keep `StaticCastItems`, `MakeConst`, `RemoveConst`.
- **Rollout:** sup-mvvm only. sup-gui-core and sequencer-gui are left for a later session and
  **will not compile** until then (see "Downstream follow-up").
- **No project version bump** as part of this work.

### Two things verified by compilation, worth not re-litigating

- **Do not make `TagIndex` implicitly constructible.** Removing `explicit` alone achieves nothing
  (`const char*` → `string_view` → `TagIndex` is two user-defined conversions, ill-formed). Adding
  `const char*`/`std::string` ctors to fix that splits the overload set: `"tag"` and `std::string`
  bind the non-template while `std::string_view` binds the template. sup-mvvm uses
  `constexpr std::string_view` tag constants and sequencer-gui uses `constexpr auto = "..."`
  (`const char*`), so the same logical call would resolve differently per package. Keep
  `explicit TagIndex(const std::string_view&)` and supply ergonomics via explicit `string_view`
  overloads.
- **The only silent change** in the whole refactor is a bare discarded `(void)item.GetItem(ti);`.
  Grep for it before landing; there are currently none.

---

## Target API

### `mvvm/model/session_item.h`

Take `std::string_view` **by value** everywhere (today it is `const std::string_view&` here and
`const std::string&` in `TaggedItems`). By-value `string_view` accepts `const char*`, `std::string`
and `std::string_view` uniformly, so overload viability no longer depends on how a caller declared
its tag constant.

```cpp
// ---- counts: value-returning, no pair needed ----
std::size_t GetTotalItemCount() const;
std::size_t GetItemCount(std::string_view tag) const;   // throws on unregistered tag

// ---- untyped ----
const SessionItem* FindItem(const TagIndex& tag_index) const;   // never throws
SessionItem*       FindItem(const TagIndex& tag_index);

std::vector<const SessionItem*> GetItems(std::string_view tag) const;  // throws on bad tag
std::vector<SessionItem*>       GetItems(std::string_view tag);

std::vector<const SessionItem*> GetAllItems() const;
std::vector<SessionItem*>       GetAllItems();

// ---- typed, strict (throws on bad tag, bad index, or bad cast) ----
template <typename ItemT = SessionItem> const ItemT& GetItem(const TagIndex&) const;
template <typename ItemT = SessionItem> ItemT&       GetItem(const TagIndex&);
template <typename ItemT = SessionItem> const ItemT& GetItem(std::string_view tag) const;
template <typename ItemT = SessionItem> ItemT&       GetItem(std::string_view tag);

// ---- typed, total (nullptr on bad tag, bad index, or bad cast) ----
template <typename ItemT = SessionItem> const ItemT* FindItem(const TagIndex&) const;
template <typename ItemT = SessionItem> ItemT*       FindItem(const TagIndex&);
template <typename ItemT = SessionItem> const ItemT* FindItem(std::string_view tag) const;
template <typename ItemT = SessionItem> ItemT*       FindItem(std::string_view tag);

// ---- plural typed, filtering, never throws ----
template <typename ItemT> std::vector<const ItemT*> FindItems(std::string_view tag) const;
template <typename ItemT> std::vector<ItemT*>       FindItems(std::string_view tag);
```

Implementations follow the existing idiom — const version holds the logic, non-const derives via
`const_cast` + `std::as_const`, exactly as `session_item.cpp:319,334` and `item_utils.h:296-299`
already do:

```cpp
template <typename ItemT>
inline const ItemT* SessionItem::FindItem(const TagIndex& tag_index) const
{
  if constexpr (std::is_same_v<ItemT, SessionItem>)
  {
    return FindItem(tag_index);                     // non-template primitive, no RTTI
  }
  else
  {
    return dynamic_cast<const ItemT*>(FindItem(tag_index));
  }
}

template <typename ItemT>
inline ItemT* SessionItem::FindItem(const TagIndex& tag_index)
{
  return const_cast<ItemT*>(std::as_const(*this).FindItem<ItemT>(tag_index));
}

template <typename ItemT>
inline const ItemT& SessionItem::GetItem(const TagIndex& tag_index) const
{
  const auto* result = FindItem<ItemT>(tag_index);
  if (result == nullptr)
  {
    throw RuntimeException("SessionItem::GetItem: no item of the requested type at tag '"
                           + tag_index.GetTag() + "'");
  }
  return *result;
}
```

**Deliberately absent: a strict `GetItems<T>`.** No caller wants "throw if any sibling has the
wrong type"; adding it would create new runtime failure modes at the existing `GetItems<T>` sites.
`FindItems<T>` (filtering) is the plural analogue of `FindItem<T>` (nullptr).

### Resolution table (verified with `g++ -std=c++17`, zero ambiguities)

| call | non-const receiver | const receiver |
|---|---|---|
| `FindItem(ti)` | `SessionItem*` | `const SessionItem*` |
| `FindItem<Foo>(ti)` | `Foo*` | `const Foo*` |
| `FindItem("tag")` / `FindItem(std::string)` / `FindItem(string_view)` | `SessionItem*` | `const SessionItem*` |
| `GetItem(ti)` | `SessionItem&` | `const SessionItem&` |
| `GetItem<Foo>("tag")` | `Foo&` | `const Foo&` |
| `GetItems("tag")` | `vector<SessionItem*>` | `vector<const SessionItem*>` |
| `FindItems<Foo>("tag")` | `vector<Foo*>` | `vector<const Foo*>` |

The non-template `FindItem(const TagIndex&)` and the template with `ItemT = SessionItem` have
identical signatures; `[over.match.best]` prefers the non-template. This mirrors today's
structure, so it adds no new risk.

### `TaggedItems`, `SessionItemContainer`, `ISessionModel`

- `TaggedItems`: mirror the same split; **build `FindItem` on `FindContainer`, not `GetContainer`** —
  that one-line change is what makes it total. Drop the lying `const` from `InsertItem`/`TakeItem`
  (`tagged_items.h:119,129`). Pair up `GetContainer` / `FindContainer` / `ContainerAt`.
- `SessionItemContainer`: pair `ItemAt` and `GetItems`. **Known residual leak, accepted:**
  `begin()/end()` expose `const_iterator` over `vector<unique_ptr<SessionItem>>`, so
  `(*it)->SetData(...)` still mutates through a const container. Document it, mark as internal,
  do not fix here — closing it needs an iterator adaptor.
- `ISessionModel::GetRootItem`: make it a **pure-virtual pair**. There is exactly one override in
  all three packages (`session_model.h:57`). Do *not* add a non-virtual const_casting helper — a
  derived `const … GetRootItem() const override` would hide the base's non-const overload and
  silently return const.

### `container_utils.h` — add `ConvertItems`

The existing helpers conflate two operations: `CastItems` is a `dynamic_cast` downcast-and-filter,
but it is widely abused merely to upcast `vector<Derived*>` → `vector<Base*>` or to add const,
paying RTTI and silently dropping mismatches. Add the missing primitive:

```cpp
/**
 * @brief Returns a vector of pointers converted to the given type.
 *
 * Only implicit pointer conversions are allowed (upcast to a base, adding const, or both).
 * No RTTI is involved, and no element can be lost.
 */
template <typename T, typename C>
std::vector<T*> ConvertItems(const C& container)
{
  std::vector<T*> result;
  result.reserve(container.size());
  (void)std::transform(container.begin(), container.end(), std::back_inserter(result),
                       [](auto* element) -> T* { return element; });
  return result;
}
```

Keep `CastItems`, `StaticCastItems`, `MakeConst`, `RemoveConst` as they are. `RemoveConst` is the
tool for the `IChildrenStrategy` escape hatch below.

---

## Execution phases (all in sup-mvvm)

### Phase 0 — `ConvertItems` — non-breaking
Add `ConvertItems` to `mvvm/utils/container_utils.h` plus unit tests alongside the existing ones in
`test/testsup-mvvm-model/container_utils_tests.cpp`. Self-contained.

### Phase 1 — inner layers — non-breaking, invisible outside `session_item.cpp`
Give `SessionItemContainer` and `TaggedItems` their const/non-const pairs; drop the false `const`
on `TaggedItems::InsertItem`/`TakeItem`; rebuild `TaggedItems::FindItem` on `FindContainer`.

Keep `SessionItem`'s public signatures **unchanged** for now, absorbing the difference with three
clearly-marked temporary `const_cast`s:

```cpp
SessionItem* SessionItem::GetItem(const TagIndex& ti) const
{
  return const_cast<SessionItem*>(GetTaggedItems()->FindItem(ti));  // TEMPORARY — removed in Phase 2
}
```

The tree stays green at the end of this phase.

### Phase 2 — `SessionItem` public API — the breaking change
Introduce the target API; remove the five old accessors and the Phase-1 `const_cast`s. Then update:

- `CompoundItem::Property` / `SetProperty` → `GetItem(TagIndex(tag)).Data<T>()` /
  `.SetData(value)`. Fixes the unchecked dereference; a missing property now throws instead of UB.
- `GroupItem` → split into `GetCurrentItem<T>()` (reference, throws) and `FindCurrentItem<T>()`
  (pointer, nullptr). `group_item_tests.cpp:287` stays valid; "is the current item a `T`?" becomes
  `FindCurrentItem<T>() != nullptr`.
- `ISessionModel` / `SessionModel` → `GetRootItem` pair.
- `item_utils` → const/non-const pairs for `ChildAt`, `TopLevelItems`, `SinglePropertyItems`;
  `FindNextSibling` / `FindPreviousSibling` lose their reliance on the const hole.
  The shared `IterateImpl` template (`item_utils.cpp:112-140`) starts working *correctly* with the
  pair — delete its now-obsolete comment about `GetAllItems()` returning non-const regardless of
  constness.
- `model_utils` → pairs for `GetTopItems<T>`, `GetTopItem<T>`, `FindItems<T>` (`model_utils.h:44-60`).
- `GetTotalItemCount` → reimplement as `GetTaggedItems()->GetTotalItemCount()` instead of
  `GetAllItems().size()` (`session_item.cpp:174-177`), so it stops allocating a vector and stops
  depending on overload selection.
- **The 5 `IChildrenStrategy` implementations** in
  `src/lib/sup-mvvm-viewmodel/mvvm/providers/standard_children_strategies.cpp` — all in one file —
  now receive `vector<const SessionItem*>` from a `const SessionItem*`. Wrap with
  `utils::RemoveConst(...)` and add a one-line comment pointing at the
  `IChildrenStrategy::GetChildren(const SessionItem*) const → vector<SessionItem*>` contract
  (`providers/i_children_strategy.h:54`) as the reason. This is the accepted cost of leaving that
  interface alone.
- Remaining ~34 call sites across `src/lib` and `examples/`.

### Phase 3 — sup-mvvm tests (~344 sites)
Mechanical, high volume. Expect a handful of `EXPECT_EQ(…, nullptr)` → `EXPECT_THROW` conversions
where a test asserted the old nullptr-for-absent behavior on what is now a strict `GetItem`.
Update the three tests that pin the old inconsistency:
`session_item_tests.cpp:931-936` (and delete the `// consider to change` comment),
`compound_item_tests.cpp:182-201`, `group_item_tests.cpp:287,291`.

### Rewrite recipes

```
HasTag(x, t) ? x.GetItem<T>(t) : nullptr   ->  x.FindItem<T>(t)
*x.GetItem<T>(t)                           ->  x.GetItem<T>(t)       // throw semantics preserved
x.GetItem<T>(t)->M()                       ->  x.GetItem<T>(t).M()   // throw semantics preserved
auto* p = x.GetItem<T>(t); if (p) …        ->  auto* p = x.FindItem<T>(t); if (p) …
x.GetItem(tag_index)                       ->  x.FindItem(tag_index)
x.GetItems<T>(t)                           ->  x.FindItems<T>(t)
```

Two cautions:
- **Never blanket-rewrite `GetItem<T>` → `FindItem<T>`.** Where the result is immediately
  dereferenced that converts today's exception into undefined behaviour. Use the `*`/`->` recipes.
- In the viewmodel controllers (`viewmodel_controller.cpp:67,81`,
  `chart_viewport_controller.cpp:150,169`, `connectable_view_model_controller.cpp:110`,
  `graph_viewport_plot_controller.cpp:92,115`) prefer `GetItem<>(ti)` over `FindItem(ti)` where the
  event contract guarantees the item exists — that turns a latent null-deref into a diagnosable
  exception. Note `connectable_view_model_controller.cpp:97` currently instantiates the throwing
  template with `T = SessionItem` (a cast that can never fail) and derefs unchecked at line 99.

---

## Verification

```bash
cmake --preset debug-gcc
cmake --build --preset debug-gcc
ctest --test-dir out/build/debug-gcc
```

Per phase:
- **Phase 0/1** must leave the full suite green with no call-site changes outside the touched files.
- **Phase 2/3**: full suite green. Then check the const-correctness actually landed rather than
  being papered over — count `const_cast` in `src/` before and after; the only *new* ones permitted
  are the `std::as_const` forwarding pairs and the 5 `RemoveConst` calls in
  `standard_children_strategies.cpp`:
  ```bash
  grep -rn "const_cast" src/ | wc -l
  grep -rn "(void).*GetItem" src/ test/     # must stay empty — the one silent-change pattern
  ```
- Targeted suites for the semantic changes:
  ```bash
  ctest --test-dir out/build/debug-gcc -R "Model\."
  ./out/build/debug-gcc/test/testsup-mvvm-model/testsup-mvvm-model \
      --gtest_filter='SessionItemTest*:CompoundItemTest*:GroupItemTest*:TaggedItemsTest*'
  ```
- Add new tests for the behaviors that did not previously exist: `FindItem<T>` returning nullptr on
  a wrong-typed child, `GetItem<T>` throwing on a missing tag, and `FindItems<T>` filtering.
- Build the examples (`examples/`) — they are part of the call-site count and are easy to forget.

## Downstream follow-up (deferred, not in this plan's scope)

sup-gui-core and sequencer-gui **will not compile** against the new sup-mvvm until migrated. Since
there is no version bump, a stale downstream build will fail with template errors at the call sites
rather than a single clear `find_package` message — worth knowing when picking the work up.

Measured scope, for when that happens: ~29 function signatures across ~19 files, ~250–350 mostly
mechanical lines. 93% of the ~537 downstream call sites hold a non-const object and are untouched;
only ~39 have a const receiver and only **8 of those mutate** — those 8 are genuine latent bugs.
The widest single ripple is `SetAttribute(const mvvm::CompoundItem&, …)`
(`attribute_item_transform_helper.h:148`, 22 callers but **zero** const-receiver callers, so it is a
one-line signature change); its `extern template` instantiations at lines 126-137 must move in
lockstep. The deepest chain is the `ProcedurePreambleItem` const-mutator cluster (depth 3, but it
terminates). The 4 downstream `IChildrenStrategy` overrides need the same `RemoveConst` treatment as
sup-mvvm's own.
