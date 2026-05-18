/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PROVIDERS_STANDARD_PRESENTATION_ITEMS_H_
#define MVVM_PROVIDERS_STANDARD_PRESENTATION_ITEMS_H_

//! @file
//! Defines standard presentations for SessionItem.

#include <mvvm/providers/abstract_presentation_item.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The LabelPresentationItem class shows the label associated with the given item.
 *
 * Item is used by ViewModelController to keep track on which cells belongs to which item. Item's
 * data is not used. The label is non-editable, and it is not possible to set any other data to it.
 */
class LabelPresentationItem : public AbstractPresentationItem
{
public:
  explicit LabelPresentationItem(SessionItem* item, const std::string& label = {});

  bool SetData(const QVariant& data, role_t qt_role) override;

  QVariant Data(role_t qt_role) const override;

  bool IsEditable() const override;

  QVector<role_t> GetQtRolesToEmit(role_t data_role) const override;

private:
  std::string m_label;
};

/**
 * @brief The FixedDataPresentationItem class shows any type of QVariant-based data associated with
 * given qt_role and given item.
 *
 * Ideologically, it is similar to LabelPresentationItem, where the label has no connection with
 * item itself, and item pointer are used only by the controller to associate items and presentation
 * items. This class goes a step further and allows one to add any Qt role that has no direct
 * counterpart among SessionItem's roles.
 */
class FixedDataPresentationItem : public AbstractPresentationItem
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param item Reference item.
   * @param fixed_data Map of qt roles associated with the data.
   */
  FixedDataPresentationItem(SessionItem* item, std::map<role_t, QVariant> fixed_data);

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& data, role_t qt_role) override;

  QVector<role_t> GetQtRolesToEmit(role_t data_role) const override;

  bool IsEditable() const override;

private:
  std::map<role_t, QVariant> m_data;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_STANDARD_PRESENTATION_ITEMS_H_
