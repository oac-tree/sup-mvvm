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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H

#include <QWidget>
#include <memory>

class QBoxLayout;
class GraphModel;
class QBoxLayout;

namespace mvvm
{
class AllItemsTreeView;
}  // namespace mvvm

//! Shows model content in standard tree view.

class GraphPropertyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GraphPropertyWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);
  ~GraphPropertyWidget();

  void setModel(GraphModel* model);

private:
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H
