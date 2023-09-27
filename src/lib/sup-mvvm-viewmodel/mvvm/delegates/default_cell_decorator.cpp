/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/delegates/default_cell_decorator.h"

#include <mvvm/editors/scientific_spinbox.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>

#include <QModelIndex>
#include <QStyleOptionViewItem>

namespace mvvm
{

bool DefaultCellDecorator::HasCustomDecoration(const QModelIndex& index) const
{
  return GetCellText(index).has_value();
}

std::optional<std::string> DefaultCellDecorator::GetCellText(const QModelIndex& index) const
{
  auto variant = index.data();
  if (!variant.isValid())
  {
    return {};
  }

  if (utils::IsComboPropertyVariant(variant))
  {
    return std::optional<std::string>{variant.value<ComboProperty>().GetLabel()};
  }

  if (utils::IsBoolVariant(variant))
  {
    return variant.value<bool>() ? std::optional<std::string>{"True"}
                                 : std::optional<std::string>{"False"};
  }

  if (utils::IsExternalPropertyVariant(variant))
  {
    return std::optional<std::string>{variant.value<ExternalProperty>().GetText()};
  }

  if (utils::IsDoubleVariant(variant))
  {
    auto str = ScientificSpinBox::toString(index.data(Qt::EditRole).value<double>(),
                                           constants::kDefaultDoubleDecimals);
    return std::optional<std::string>{str.toStdString()};
  }

  if (utils::IsDoubleVariant(variant))
  {
    auto str = ScientificSpinBox::toString(index.data(Qt::EditRole).value<double>(),
                                           constants::kDefaultDoubleDecimals);
    return std::optional<std::string>{str.toStdString()};
  }

  if (utils::GetQtVariantName(variant) == constants::kInt8QtTypeName
      || utils::GetQtVariantName(variant) == constants::kUInt8QtTypeName)
  {
    // Default decoration for int8 and uint8 types in Qt cells are some weired ASCII characters.
    // We force it here to a string, so int8{127} would look like "127".
    const int num(variant.value<int>());
    return std::optional<std::string>{std::to_string(num)};
  }

  return {};
}

void DefaultCellDecorator::InitStyleOption(QStyleOptionViewItem* option, const QModelIndex& index)
{
  if (!HasCustomDecoration(index))
  {
    return;
  }

  auto value = GetCellText(index).value();
  option->text = QString::fromStdString(value);
  if (value.empty())
  {
    option->features &= ~QStyleOptionViewItem::HasDisplay;
  }
}

}  // namespace mvvm
