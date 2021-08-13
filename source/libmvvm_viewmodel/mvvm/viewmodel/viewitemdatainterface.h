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

#ifndef MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H
#define MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H

#include <QVariant>

namespace ModelView
{
//! Defines interface for ViewItem's data access.

class ViewItemDataInterface
{
public:
  virtual QVariant data(int role = Qt::EditRole) const = 0;

  virtual bool setData(const QVariant& data, int role = Qt::EditRole) = 0;

  virtual bool isEnabled() const = 0;

  virtual bool isEditable() const = 0;
};

}  // namespace ModelView

#endif // MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H
