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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
#define CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class StringCompleterComboEditor;
}

namespace customeditors
{

class EditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit EditorWidget(QWidget* parent_widget = nullptr);

private:
  mvvm::StringCompleterComboEditor* m_combo_editor{nullptr};
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
