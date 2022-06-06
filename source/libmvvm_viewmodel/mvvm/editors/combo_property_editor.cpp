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

#include "mvvm/editors/combo_property_editor.h"

#include "mvvm/model/combo_property.h"

#include <QComboBox>
#include <QVBoxLayout>

namespace
{
QStringList ToList(const std::vector<std::string>& container)
{
  QStringList result;
  for (const auto& str : container)
  {
    result.push_back(QString::fromStdString(str));
  }
  return result;
}
}  // namespace

namespace mvvm
{

ComboPropertyEditor::ComboPropertyEditor(QWidget* parent)
    : CustomEditor(parent), m_box(new QComboBox)
{
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  auto layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_box);
  setLayout(layout);

  SetConnected(true);
}

QSize ComboPropertyEditor::sizeHint() const
{
  return m_box->sizeHint();
}

QSize ComboPropertyEditor::minimumSizeHint() const
{
  return m_box->minimumSizeHint();
}

bool ComboPropertyEditor::IsPersistent() const
{
  return true;
}

void ComboPropertyEditor::OnIndexChanged(int index)
{
  auto comboProperty = GetData().value<ComboProperty>();

  if (comboProperty.GetCurrentIndex() != index)
  {
    comboProperty.SetCurrentIndex(index);
    SetDataIntern(QVariant::fromValue<ComboProperty>(comboProperty));
  }
}

void ComboPropertyEditor::UpdateComponents()
{
  SetConnected(false);

  m_box->clear();
  m_box->insertItems(0, ToList(GetInternLabels()));
  m_box->setCurrentIndex(GetInternIndex());

  SetConnected(true);
}

//! Returns list of labels for QComboBox

std::vector<std::string> ComboPropertyEditor::GetInternLabels()
{
  if (!GetData().canConvert<ComboProperty>())
  {
    return {};
  }
  auto comboProperty = GetData().value<ComboProperty>();
  return comboProperty.GetValues();
}

//! Returns index for QComboBox.

int ComboPropertyEditor::GetInternIndex()
{
  if (!GetData().canConvert<ComboProperty>())
  {
    return 0;
  }
  auto comboProperty = GetData().value<ComboProperty>();
  return comboProperty.GetCurrentIndex();
}

void ComboPropertyEditor::SetConnected(bool isConnected)
{
  if (isConnected)
  {
    connect(m_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ComboPropertyEditor::OnIndexChanged, Qt::UniqueConnection);
  }
  else
  {
    disconnect(m_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
               &ComboPropertyEditor::OnIndexChanged);
  }
}

}  // namespace mvvm
