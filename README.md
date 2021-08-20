# Operational Applications UI foundation <!-- omit in toc -->

Collection of C++/Qt common components for Operational Application User Interfaces.

- [Requirements](#requirements)
- [Installation](#installation)
- [Quick start](#quick-start)
- [The context](#the-context)
- [List of features](#list-of-features)
- [Main components](#main-components)

## Requirements

- C++17
- CMake 3.14
- Qt 5.12
- libxml2

## Installation

```bash
cmake <source> && make -j4 && ctest
```

## Quick start

```C++
// Create an application model holding all the data of running application.
SessionModel model;

// Populate model with content.
// - The model contains a single item representing a Gaussian distribution.
auto compound = model.InserItem<CompoundItem>;
compound->SetDisplayName("Gaussian");
compound->AddProperty("Mean", 0.0);
compound->AddProperty("StdDev", 1.0);

// Creates ViewModel which can be shown in Qt widgets.
ViewModel view_model(&model);

// Open the view model in standard Qt's tree view.
QTreeView view;
view.SetModel(&view_model);
view.show();

// After editing is complete, save the content in the XML file.
XmlDocument document({&model});
document.save("filename.xml");
```

## The context

This model-view-viewmodel (MVVM) framework is intended for large Qt-based
scientific applications written in C++. The definition of `large` is quite
arbitrary and means something in a line "can be easilly above 50K of LOC".

### The Model  <!-- omit in toc -->

The first part of the framework, the model, consists of a group of classes to
build a tree-like structure, named `SessionModel`, to handle any data of the
GUI session. The data in this model is originated, at least partly, from the
persistent storage (database, XML files), and then leaves in the memory of the
running application. The model is populated with elements of different
complexities: from compound items representing complex domain objects (classes),
to elementary items, representing editable properties.

This part of the framework is intentionally made independent of any graphics
library. The idea behind is the following:

In large GUIs, the business logic gets quickly spoiled with presentation logic.
Graphics library classes (like `QModelIndex` from Qt library) start to appear
everywhere, even in places that have nothing to do with graphics. Removing
graphics library from dependencies allows focusing more on common needs (i.e.
objects construction, property editing, etc) of GUI applications rather than on
presentation details. Thus, the intention here is to build an application model
to handle the data and logic of GUI while being independent on any particular
GUI library.

### The ViewModel  <!-- omit in toc -->

The second part defines `ViewModel` and serves as a thin counterpart of
`SessionModel` in the Qt world. `ViewModel` doesn't own the data but simply acts
as a proxy to different parts of `SessionModel`. It is derived from
`QAbstractItemModel` and intended to work together with Qt's trees and tables.
The layout of `ViewModel` (i.e. parent/child relationships) doesn't follow the
layout of the original `SessionModel`. It is generated on the fly by using
strategy who-is-my-next-child provided by the user. In practice, it allows
generating Qt tables and trees with arbitrary layouts, based on a common data
source, without diving into the nightmare of `QAbstractProxyModel`.
Particularly, the aforementioned machinery allows having something in the line
of the ancient [Qt property browser framework](https://doc.qt.io/archives/qq/qq18-propertybrowser.html).

### The View  <!-- omit in toc -->

The third part, the View, contains few convenience widgets for property editing.
In the future this part can be extended with widget library for scientific plotting.

### Further reading <!-- omit in toc -->

+ [GUI architecture, Martin Fowler](https://martinfowler.com/eaaDev/uiArchs.html)

## List of features

### Model part <!-- omit in toc -->

- Application model to store arbitrary data of GUI session `(+)`.
- Serialization of application models to XML `(+)`.
- Depends only on C++17 and libxml `(+)`.
- Unique identifiers for all items and memory pool registration `(+)`.
- Multiple models with the possibility for persistent inter-model links `(+)`.
- Undo/redo based on command pattern `(+/-)`.

### ViewModel part (depends on Qt5) <!-- omit in toc -->

- View model to show parts of application model in Qt widgets `(+/-)`.
- Property editors `(+/-)`.
- Automatic generation of widgets from model content `(+/-)`.
- Scientific plotting `(+/-)`.
- Flexible layout of Qt's trees and tables `(+/-)`.

> `(+)` - feature is implemented, `(+/-)` - porting is required from the original `qt-mvvm`.

### Size of the framework <!-- omit in toc -->

- 4000 LOC of libraries
- 4000 LOC of tests (1500 `EXPECT` statements)

## Main components

- [SessionItem](docs/mvvm/model/sessionitem.md)
- [CompoundItem](docs/mvvm/model/compounditem.md)
- [SessionModel](docs/mvvm/model/sessionmodel.md)
- [Serialization](docs/mvvm/model/serialization.md)
- to be continued ... 
