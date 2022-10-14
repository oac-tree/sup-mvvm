/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_PROPERTY_FLAT_VIEW_H_
#define MVVM_WIDGETS_PROPERTY_FLAT_VIEW_H_

#include <mvvm/view_export.h>

#include <QWidget>

namespace mvvm
{

//! Widget holding grid layout with editors and intended for displaying all properties of given
//! SessionItem.

class MVVM_VIEW_EXPORT PropertyFlatView : public QWidget
{
  Q_OBJECT

public:
  explicit PropertyFlatView(QWidget* parent = nullptr);

};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_PROPERTY_FLAT_VIEW_H_
