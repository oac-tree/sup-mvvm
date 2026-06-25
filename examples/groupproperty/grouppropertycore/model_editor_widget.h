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

#ifndef GROUPPROPERTY_GROUPPROPERTYCORE_MODEL_EDITOR_WIDGET_H_
#define GROUPPROPERTY_GROUPPROPERTYCORE_MODEL_EDITOR_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class AllItemsTreeView;
class PropertyTreeView;
}  // namespace mvvm

namespace groupproperty
{

class SampleModel;

//! Shows the whole model content as a tree on the left, and the properties of a single ParticleItem
//! on the right, where unused shapes and their parameters are hidden.

class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(SampleModel* model = nullptr, QWidget* parent = nullptr);

  void SetModel(SampleModel* model);

private:
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  mvvm::PropertyTreeView* m_property_view{nullptr};
};

}  // namespace groupproperty

#endif  // GROUPPROPERTY_GROUPPROPERTYCORE_MODEL_EDITOR_WIDGET_H_
