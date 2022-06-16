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

#ifndef MVVM_WIDGETS_PROPERTY_TREE_VIEW_H_
#define MVVM_WIDGETS_PROPERTY_TREE_VIEW_H_

#include <mvvm/widgets/abstract_item_view.h>

class QTreeView;

namespace mvvm
{

//! Widget holding standard QTreeView and intended for displaying all properties of given
//! SessionItem.

class MVVM_VIEW_EXPORT PropertyTreeView : public AbstractItemView
{
  Q_OBJECT

public:
  explicit PropertyTreeView(QWidget* parent = nullptr);

private:
  void UpdateView() override;

  QTreeView* m_tree_view{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_PROPERTY_TREE_VIEW_H_
