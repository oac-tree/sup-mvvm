/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "main_window.h"

#include "model_editor_widget.h"
#include "sample_model.h"

#include <QCoreApplication>
#include <QSettings>

namespace
{
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
}  // namespace

namespace flatview
{

MainWindow::MainWindow() : m_model(std::make_unique<SampleModel>())
{
  PopulateModel();
  InitApplication();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::InitApplication()
{
  QCoreApplication::setApplicationName("flatview");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setOrganizationName("sup-mvvm");

  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(400, 400)).toSize());
    move(settings.value(pos_key, QPoint(200, 200)).toPoint());
    settings.endGroup();
  }

  m_editor_widget = new ModelEditorWidget(m_model.get());
  setCentralWidget(m_editor_widget);
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

void MainWindow::PopulateModel()
{
  m_model->InsertItem<DemoItem>();
  m_model->InsertItem<DemoItem>();
  m_model->InsertItem<DemoItem>();
  m_model->InsertItem<DemoItem>();
}

}  // namespace flatview
