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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "combo_property_editor.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/viewmodel/qtcore_helper.h>

#include <QComboBox>
#include <QVBoxLayout>

namespace mvvm
{

ComboPropertyEditor::ComboPropertyEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget), m_combo_box(new QComboBox)
{
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_combo_box);
  setLayout(layout);

  SetConnected(true);
}

QSize ComboPropertyEditor::sizeHint() const
{
  return m_combo_box->sizeHint();
}

QSize ComboPropertyEditor::minimumSizeHint() const
{
  return m_combo_box->minimumSizeHint();
}

QComboBox* ComboPropertyEditor::GetComboBox()
{
  return m_combo_box;
}

void ComboPropertyEditor::OnIndexChanged(int index)
{
  auto combo_property = GetData().value<ComboProperty>();

  if (combo_property.GetCurrentIndex() != index)
  {
    combo_property.SetCurrentIndex(index);
    SetDataIntern(QVariant::fromValue<ComboProperty>(combo_property));
  }
}

void ComboPropertyEditor::UpdateComponents()
{
  if (!GetData().canConvert<ComboProperty>())
  {
    return;
  }

  SetConnected(false);

  m_combo_box->clear();

  auto combo_property = GetData().value<ComboProperty>();

  m_combo_box->insertItems(0, utils::GetStringList(combo_property.GetValues()));
  m_combo_box->setCurrentIndex(GetInternIndex());
  int index{0};
  for (const auto& tooltip : combo_property.GetToolTips())
  {
    if (index < m_combo_box->count())
    {
      m_combo_box->setItemData(index++, QString::fromStdString(tooltip), Qt::ToolTipRole);
    }
  }

  SetConnected(true);
}

int ComboPropertyEditor::GetInternIndex()
{
  if (!GetData().canConvert<ComboProperty>())
  {
    return 0;
  }

  return GetData().value<ComboProperty>().GetCurrentIndex();
}

void ComboPropertyEditor::SetConnected(bool is_connected)
{
  if (is_connected)
  {
    connect(m_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ComboPropertyEditor::OnIndexChanged, Qt::UniqueConnection);
  }
  else
  {
    disconnect(m_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &ComboPropertyEditor::OnIndexChanged);
  }
}

}  // namespace mvvm
