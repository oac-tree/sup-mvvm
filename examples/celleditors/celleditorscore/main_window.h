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

#ifndef CELLEDITORS_CELLEDITORSCORE_MAIN_WINDOW_H_
#define CELLEDITORS_CELLEDITORSCORE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

namespace celleditors
{

class SampleModel;
class ModelEditorWidget;

/**
 * @brief The MainWindow represents main window of this application.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow(MainWindow&&) noexcept = delete;
  MainWindow& operator=(MainWindow&&) noexcept = delete;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void WriteSettings();

  ModelEditorWidget* m_editor_widget{nullptr};
  std::unique_ptr<SampleModel> m_model;
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_MAIN_WINDOW_H_
