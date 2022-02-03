// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/delegates/defaultcelldecorator.h"

#include "mvvm/viewmodel/customvariants.h"

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

  if (utils::IsComboPropertyVariant(variant))
  {
    return std::optional<std::string>{variant.value<ComboProperty>().GetLabel()};
  }

  else if (utils::IsBoolVariant(variant))
  {
    return variant.value<bool>() ? std::optional<std::string>{"True"}
                                 : std::optional<std::string>{"False"};
  }

  //! FIXME uncomment after ExternalProperty implementation
  //  else if (Utils::IsExtPropertyVariant(variant))
  //    return std::optional<std::string>{variant.value<ExternalProperty>().text()};

  //  else if (Utils::IsColorVariant(variant))
  //    return std::optional<std::string>{std::string()};

  //  else if (Utils::IsDoubleVariant(variant))
  //    return std::optional<std::string>{
  //        ScientificSpinBox::toString(index.data(Qt::EditRole).value<double>(),
  //                                    Constants::default_double_decimals)
  //            .toStdString()};

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
