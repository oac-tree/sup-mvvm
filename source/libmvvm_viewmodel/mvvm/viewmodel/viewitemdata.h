/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
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

#ifndef MVVM_VIEWMODEL_VIEWITEMDATA_H
#define MVVM_VIEWMODEL_VIEWITEMDATA_H

#include "viewitemdatainterface.h"

#include <QMap>

namespace ModelView
{
//! Carries the data for ViewItem.

class ViewItemData : public ViewItemDataInterface
{
public:
  ViewItemData();

  virtual ~ViewItemData() = default;

  virtual QVariant data(int role = Qt::EditRole) const override;

  virtual bool setData(const QVariant& data, int role = Qt::EditRole) override;

  bool isEnabled() const override;

  void setEnabled(bool value);

  bool isEditable() const override;

  void setEditable(bool value);

private:
  bool m_is_enabled;
  bool m_is_editable;
  QMap<int, QVariant> m_data;
};

}  // namespace ModelView

#endif // MVVM_VIEWMODEL_VIEWITEMDATA_H
