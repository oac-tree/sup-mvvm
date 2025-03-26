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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSCORE_MAIN_WINDOW_H_
#define CUSTOMEDITORS_CUSTOMEDITORSCORE_MAIN_WINDOW_H_

#include <QMainWindow>

namespace customeditors
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void WriteSettings();
};

}  // namespace celleditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSCORE_MAIN_WINDOW_H_
