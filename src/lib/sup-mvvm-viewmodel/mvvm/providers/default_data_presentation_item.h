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

#ifndef MVVM_PROVIDERS_DEFAULT_DATA_PRESENTATION_ITEM_H_
#define MVVM_PROVIDERS_DEFAULT_DATA_PRESENTATION_ITEM_H_

#include <mvvm/providers/abstract_presentation_item.h>

#include <QMap>

namespace mvvm
{

class SessionItem;

/**
 * @brief The DefaultDataPresentationItem class presents a data role of SessionItem.
 *
 * In addition to translating SessionItem roles to their Qt equivalents, it can carry a small set of
 * presentation-only Qt roles (currently Qt::BackgroundRole, Qt::ForegroundRole and
 * Qt::DecorationRole). These roles are
 * set via the view model, are stored on board of the presentation item, and are never propagated to
 * the underlying SessionItem (i.e. they do not end up on disk). They affect only how the item is
 * shown in Qt trees and tables, and can be used, for example, to highlight changed values in cells.
 */
class DefaultDataPresentationItem : public AbstractPresentationItem
{
public:
  DefaultDataPresentationItem(SessionItem* item, role_t main_data_role);

  ~DefaultDataPresentationItem() override = default;

  DefaultDataPresentationItem(const DefaultDataPresentationItem&) = delete;
  DefaultDataPresentationItem& operator=(const DefaultDataPresentationItem&) = delete;
  DefaultDataPresentationItem(DefaultDataPresentationItem&&) noexcept = delete;
  DefaultDataPresentationItem& operator=(DefaultDataPresentationItem&&) noexcept = delete;

  bool IsCheckable() const override;

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& data, role_t qt_role) override;

  bool IsPresentationOnlyRole(role_t qt_role) const override;

  QVector<role_t> GetQtRolesToEmit(role_t data_role) const override;

  role_t GetMainDataRole() const;

  /**
   * @brief Returns variant to represent Qt::DisplayRole of underlying main data role.
   *
   * The returned QVariant will contain QString. We perform our own conversion to QString to provide
   * specific formatting for some types. A nullable item without data shows placeholder text.
   */
  QVariant GetDisplayRole() const;

private:
  /**
   * @brief Returns true if the item is nullable and has no main data.
   */
  bool IsPlaceholderShown() const;

  role_t m_main_data_role{0};

  //!< presentation-only Qt roles, not propagated to the underlying SessionItem
  QMap<role_t, QVariant> m_qt_data;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_DEFAULT_DATA_PRESENTATION_ITEM_H_
