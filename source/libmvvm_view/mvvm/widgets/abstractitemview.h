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

#ifndef MVVM_WIDGETS_ABSTRACTITEMVIEW_H
#define MVVM_WIDGETS_ABSTRACTITEMVIEW_H

#include "mvvm/widgets/itemviewbase.h"

class QAbstractItemView;

namespace mvvm
{

class ApplicationModel;
class SessionItem;

//! Generic view to show SessionModel in Qt lists, trees and tables.

class MVVM_VIEW_EXPORT AbstractItemView : public ItemViewBase
{
  Q_OBJECT

public:
  explicit AbstractItemView(QWidget* parent = nullptr);

  void SetApplicationModel(ApplicationModel* model);

  void SetItem(SessionItem* item);

private:
  virtual std::unique_ptr<ViewModel> CreateViewModel(ApplicationModel* model) = 0;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ABSTRACTITEMVIEW_H
