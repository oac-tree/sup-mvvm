/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef DRAGANDMOVE_DRAGANDMOVECORE_MAIN_WINDOW_H_
#define DRAGANDMOVE_DRAGANDMOVECORE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

namespace dragandmove
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

}  // namespace dragandmove

#endif  // DRAGANDMOVE_DRAGANDMOVECORE_MAIN_WINDOW_H_
