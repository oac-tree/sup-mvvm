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

#include "string_completer_combo_editor.h"

#include <QComboBox>
#include <QVBoxLayout>

namespace mvvm
{

StringCompleterComboEditor::StringCompleterComboEditor(QWidget *parent_widget)
    : QWidget(parent_widget)
{
  setAutoFillBackground(true);

  m_box->setEditable(true);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_box);
  setLayout(layout);
}

}  // namespace mvvm
