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

#ifndef MVVM_VIEWMODELBASE_VIEWITEMDATA_H
#define MVVM_VIEWMODELBASE_VIEWITEMDATA_H

#include "viewitemdatainterface.h"

#include <QMap>

namespace mvvm
{
//! Carries the data for ViewItem.

class MVVM_VIEWMODEL_EXPORT ViewItemData : public ViewItemDataInterface
{
public:
  ViewItemData();

  virtual ~ViewItemData() = default;

  QVariant Data(int role = Qt::EditRole) const override;

  bool SetData(const QVariant& data, int role = Qt::EditRole) override;

  bool IsEnabled() const override;

  void SetEnabled(bool value);

  bool IsEditable() const override;

  void SetEditable(bool value);

private:
  bool m_is_enabled;
  bool m_is_editable;
  QMap<int, QVariant> m_data;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODELBASE_VIEWITEMDATA_H
