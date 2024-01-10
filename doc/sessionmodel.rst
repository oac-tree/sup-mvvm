SessionModel
============

The ``SessionModel`` is the main class to hold the hierarchy of ``SessionItem``
objects. It contains a single root ``SessionItem`` as an entry point to other
top-level items through the model API. In pseudo-code it it can be expressed as::

  class SessionModel
  {
  public:
    SessionModel() : m_root_item(std::make_unique<SessionItem>()) {}

  private:
    std::unique_ptr<SessionItem> m_root_item;
  };

``SessionModel`` implements ``SessionModelInterface``. It doesn't have any notification
capabilities, do not have any unddo/redo mechanism and implements only basic functionality to add,
remove and access items.

Inserting item
--------------

To insert an item in a model, one has to use the ``InsertItem`` method and provide
a pointer to a parent item and ``TagIndex`` specifying the position in the
parent's containers. When no arguments are provided, appending to a root item is assumed::

  SessionModel model;

  // appends new compound to the root item
  auto parent = model.InsertItem<CompoundItem>();

  // inserting child into parent's tag
  auto child = model.InsertItem<PropertyItem>(parent, {"tag", 0});

In the example above it is assumed that the `parent` item is properly
configured, and can accept items of a given type under the given tag.

Removing item
-------------

Item can be removed from the model by using either `RemoveItem` or `TakeItem`.
The first method will remove item and discard it completely::

  // inserts and removes the item
  auto item = model.InsertItem<CompoundItem>();
  model.RemoveItem(item)

The second item requires the knowledge of item's parent and location::

  // insert a child into a parent
  auto parent = model.InsertItem<CompoundItem>();
  auto child = model.InsertItem<SessionItem>(parent, {"tag", 0});

  // taking a child from parent
  auto taken = model.RemoveItem<SessionItem>(parent, {"tag", 0});

Memory pool
-----------

Every ``SessionItem`` carries a unique identifier that is assigned to it at the moment of
construction. This identifier can be accessed using the `GetIdentifier` method::

  SessionItem item;
  std::cout << item.GetIdentifier() << std::endl;
  >>> "4c281780-1bf6-4d98-8de2-b9775085c755"

When ``SessionItem`` is inserted in the model, this identifier gets registered in
the model's `ItemPool` together with the item's address. When the item is
removed from the model, the record is removed too. During serialization of the
model, and following reconstruction of the model from serialized content, items'
identifiers are preserved.

Knowing the identifier, one can find the ``SessionItem`` address. It allows using
identifiers for cross-linking between model parts, also for the case of
different models::

  SessionModel model;

  auto item = model.InsertItem<SessionItem>();
  auto identifier = item->GetIdentifier();

  auto found_item = model.FindItem(identifier);
  assert(item == found_item);

SessionModel API
----------------

.. doxygenclass:: mvvm::SessionModel
  :members:
