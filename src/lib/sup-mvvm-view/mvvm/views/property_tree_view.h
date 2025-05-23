/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef MVVM_VIEWS_PROPERTY_TREE_VIEW_H_
#define MVVM_VIEWS_PROPERTY_TREE_VIEW_H_

#include "abstract_item_view.h"

class QTreeView;

namespace mvvm
{

//! Widget holding standard QTreeView and intended for displaying all properties of given
//! SessionItem.

class MVVM_VIEW_EXPORT PropertyTreeView : public AbstractItemView
{
  Q_OBJECT

public:
  explicit PropertyTreeView(QWidget* parent_widget = nullptr);

  QTreeView* GetTreeView() const;

private:
  void UpdateView() override;

  QTreeView* m_tree_view{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_PROPERTY_TREE_VIEW_H_
