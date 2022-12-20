Quick start
===========

Minimal example to create a model, populate it with the content, and then edit in Qt tree::

  // Create an application model holding all the data of running application.
  SessionModel model;

  // Populate model with the content.
  // The model contains a single item representing a Gaussian distribution.

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

.. contents::
   :local:
