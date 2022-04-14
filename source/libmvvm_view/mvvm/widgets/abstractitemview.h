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

#include <functional>

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
  using create_viewmodel_t = std::function<std::unique_ptr<ViewModel>(ApplicationModel*)>;

  explicit AbstractItemView(create_viewmodel_t func, QAbstractItemView* view,
                            ApplicationModel* model = nullptr, QWidget* parent = nullptr);
  ~AbstractItemView() override;

  void SetApplicationModel(ApplicationModel* model);

  void SetItem(SessionItem* item);

private:
  create_viewmodel_t m_create_viewmodel;
};

template <typename T>
std::unique_ptr<T> CreateViewModel(ApplicationModel* model)
{
  return std::make_unique<T>(model);
}

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ABSTRACTITEMVIEW_H
