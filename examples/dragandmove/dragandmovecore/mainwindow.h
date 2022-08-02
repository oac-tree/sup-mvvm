/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef DRAGANDMOVECORE_MAINWINDOW_H
#define DRAGANDMOVECORE_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace DragAndMove
{

class SampleModel;

//! A main window. Contain two sample models and tabs with two model editors.

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void WriteSettings();
  void InitApplication();

  std::unique_ptr<SampleModel> m_model;
};

}  // namespace DragAndMove

#endif  // DRAGANDMOVECORE_MAINWINDOW_H
