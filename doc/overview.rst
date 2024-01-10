Overview
========

The context
-----------

This model-view-viewmodel **mvvm** framework is intended for large Qt-based
scientific applications written in C++. The definition of `large` is quite
arbitrary and means something in a line "can be easilly above 50K of LOC".

All classes in the framework are build around the concept of model-view-viewmodel design pattern for
graphical user interface creation, and thus fails in one of these three categories.

The Model
^^^^^^^^^

The first part of the framework, the model, consists of a group of classes to
build a tree-like structure, named ``SessionModel``, to handle any data of the
GUI session. The data in this model is originated, at least partly, from the
persistent storage (database, XML files), and then leaves in the memory of the
running application. The model is populated with elements of different
complexities: from compound items representing complex domain objects,
to elementary items, representing editable properties.

This part of the framework is intentionally made independent of any graphics
library. The idea behind is the following:

In large GUIs, the business logic gets quickly spoiled with presentation logic.
Graphics library classes (like ``QModelIndex`` from Qt library) start to appear
everywhere, even in places that have nothing to do with graphics. Removing
graphics library from dependencies allows focusing more on common needs (i.e.
objects construction, property editing, etc) of GUI applications rather than on
presentation details. Thus, the intention here is to build an application model
to handle the data and logic of GUI while being independent on any particular
GUI library.

The ViewModel
^^^^^^^^^^^^^

The second part defines ``ViewModel`` and serves as a thin counterpart of
``SessionModel`` in the Qt world. ``ViewModel`` doesn't own the data but simply acts
as a proxy to different parts of ``SessionModel``. It is derived from
``QAbstractItemModel`` and intended to work together with Qt's trees and tables.
The layout of ``ViewModel`` (i.e. parent/child relationships) doesn't follow the
layout of the original ``SessionModel``. It is generated on the fly by using
strategy who-is-my-next-child provided by the user. In practice, it allows
generating Qt tables and trees with arbitrary layouts, based on a common data
source, without diving into the nightmare of ``QAbstractProxyModel``.
Particularly, the aforementioned machinery allows having something in the line
of the ancient `Qt property browser framework <https://doc.qt.io/archives/qq/qq18-propertybrowser.html>`_.

The View
^^^^^^^^^

The third part, the View, contains few convenience widgets for property editing.
In the future this part can be extended with widget library for scientific plotting.

List of features
----------------

**Model part**

- Application model to store arbitrary data of GUI session.
- Support of all SUP data types.
- Serialization of application models to XML.
- Depends only on C++17 and libxml.
- Unique identifiers for all items and memory pool registration.
- Multiple models with the possibility for persistent inter-model links.
- Notification machinery.
- Undo/redo based on command pattern.

**ViewModel part (depends on Qt)**

- View model to show parts of application model in Qt widgets.
- Property editors.
- Automatic generation of widgets from model content.
- Scientific plotting.
- Flexible layout of Qt's trees and tables.
