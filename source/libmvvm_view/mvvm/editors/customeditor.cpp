/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/customeditor.h"

namespace mvvm
{

CustomEditor::CustomEditor(QWidget* parent) : QWidget(parent) {}

QVariant CustomEditor::GetData() const
{
  return m_data;
}

//! Returns true if editor should remains alive after editing finished.

bool CustomEditor::IsPersistent() const
{
  return false;
}

//! Sets the data from model to editor.

void CustomEditor::SetData(const QVariant& data)
{
  m_data = data;
  UpdateComponents();
}

//! Saves the data as given by editor's internal components and notifies the model.

void CustomEditor::SetDataIntern(const QVariant& data)
{
  m_data = data;
  emit dataChanged(m_data);
}

}  // namespace mvvm
