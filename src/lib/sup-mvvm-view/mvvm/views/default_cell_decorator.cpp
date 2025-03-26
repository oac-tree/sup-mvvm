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

#include "default_cell_decorator.h"

#include <mvvm/editors/scientific_spinbox.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/custom_variants.h>

#include <QDebug>
#include <QModelIndex>
#include <QStyleOptionViewItem>

namespace mvvm
{

namespace
{

/**
 * @brief Checks if given variant contains one of int8 like types.
 */
bool IsInt8Special(const QVariant& variant)
{
  return utils::GetQtVariantName(variant) == constants::kInt8QtTypeName
         || utils::GetQtVariantName(variant) == constants::kUInt8QtTypeName
         || utils::GetQtVariantName(variant) == constants::kChar8QtTypeName;
}

std::string GetBoolAsAstring(bool value)
{
  static const std::string kTrueStr("True");
  static const std::string kFalseStr("False");
  return value ? kTrueStr : kFalseStr;
}

}  // namespace

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
    return variant.value<ComboProperty>().GetLabel();
  }

  if (utils::IsBoolVariant(variant))
  {
    return GetBoolAsAstring(variant.value<bool>());
  }

  if (utils::IsExternalPropertyVariant(variant))
  {
    return variant.value<ExternalProperty>().GetText();
  }

  if (utils::IsDoubleVariant(variant))
  {
    return ScientificSpinBox::toString(variant.value<double>(), constants::kDefaultDoubleDecimals)
        .toStdString();
  }

  if (utils::IsDoubleVariant(variant))
  {
    return ScientificSpinBox::toString(variant.value<double>(), constants::kDefaultDoubleDecimals)
        .toStdString();
  }

  if (IsInt8Special(variant))
  {
    // Default decoration for int8 and uint8 types in Qt cells looks like  some weired ASCII
    // characters. We force it here to a string, so int8{127} would look like "127".
    return std::to_string(variant.value<int>());
  }

  return {};
}

void DefaultCellDecorator::InitStyleOption(const QModelIndex& index, QStyleOptionViewItem* option)
{
  // Special corner case of ExternalProperty without text. It means that the user probably whants to
  // see a pixmap only. For better alignment have to remove text option from the style.
  if (auto optional_value = GetCellText(index); optional_value.has_value())
  {
    option->text = QString::fromStdString(optional_value.value());
    if (optional_value.value().empty())
    {
      option->features.setFlag(QStyleOptionViewItem::HasDisplay, false);
    }
  }
}

}  // namespace mvvm
