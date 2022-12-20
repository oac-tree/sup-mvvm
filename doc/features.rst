List of features
================

Model part
----------

- Application model to store arbitrary data of GUI session `(+)`.
- Serialization of application models to XML `(+)`.
- Depends only on C++17 and libxml `(+)`.
- Unique identifiers for all items and memory pool registration `(+)`.
- Multiple models with the possibility for persistent inter-model links `(+)`.
- Undo/redo based on command pattern `(+)`.

ViewModel part (depends on Qt)
------------------------------

- View model to show parts of application model in Qt widgets `(+)`.
- Property editors `(+)`.
- Automatic generation of widgets from model content `(+/-)`.
- Scientific plotting `(+/-)`.
- Flexible layout of Qt's trees and tables `(+)`.

> `(+)` - feature is implemented, `(+/-)` - feature was prototyped.

Size of the framework
---------------------

- 14 kLOC of libraries
- 14 kLOC of tests (4500 `EXPECT` statements)
