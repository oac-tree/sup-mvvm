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

#include "role_dependent_editor_factory.h"

#include "editor_builders.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QWidget>

namespace mvvm
{

RoleDependentEditorFactory::RoleDependentEditorFactory()
{
  // registering all existing builders under their names

  RegisterBuilder(constants::kBoolEditorType, BoolEditorBuilder());
  RegisterBuilder(constants::kComboPropertyEditorType, ComboPropertyEditorBuilder());
  RegisterBuilder(constants::kSelectableComboPropertyEditorType,
                  SelectableComboPropertyEditorBuilder());
  RegisterBuilder(constants::kColorEditorType, ColorEditorBuilder());
  RegisterBuilder(constants::kExternalPropertyEditorType, ExternalPropertyEditorBuilder());
  RegisterBuilder(constants::kIntegerEditorType, IntegerEditorBuilder());
  RegisterBuilder(constants::kScientificSpinboxEditorType, ScientificSpinBoxEditorBuilder());
  RegisterBuilder(constants::kDoubleEditorType, DoubleEditorBuilder());
  RegisterBuilder(constants::kAllIntSpinBoxEditorType, AllIntSpinBoxEditorBuilder());
  RegisterBuilder(constants::kFloatSpinBoxEditorType, FloatEditorBuilder());
}

editor_t RoleDependentEditorFactory::CreateEditor(const QModelIndex& index) const
{
  auto item = utils::ItemFromProxyIndex(index);
  return item ? CreateEditor(item) : editor_t{};
}

editor_t RoleDependentEditorFactory::CreateEditor(const SessionItem* item) const
{
  auto builder = FindBuilder(item->GetEditorType());
  return builder ? builder(item) : editor_t{};
}

}  // namespace mvvm
