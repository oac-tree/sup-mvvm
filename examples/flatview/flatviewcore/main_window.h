/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef FLATVIEW_FLATVIEWCORE_MAIN_WINDOW_H_
#define FLATVIEW_FLATVIEWCORE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

namespace mvvm {
class ApplicationModel;
}

namespace flatview
{

class ModelEditorWidget;

/**
 * @brief The MainWindow class is a main window of this application.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

  MainWindow(const MainWindow& other) = delete;
  MainWindow(MainWindow&& other) = delete;
  MainWindow& operator=(const MainWindow& other) noexcept = delete;
  MainWindow& operator=(MainWindow&&) noexcept = delete;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void WriteSettings();
  void PopulateModel();

  ModelEditorWidget* m_editor_widget{nullptr};
  std::unique_ptr<mvvm::ApplicationModel> m_model;
};

}  // namespace flatview

#endif  // FLATVIEW_FLATVIEWCORE_MAIN_WINDOW_H_
