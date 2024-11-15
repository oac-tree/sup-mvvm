/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_CONTAINER_EDITOR_WIDGET_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_CONTAINER_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>
#include <vector>

class QTreeView;
class QBoxLayout;
class QItemSelectionModel;

namespace mvvm
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
}  // namespace mvvm

namespace saveloadproject
{

class SampleModel;

//! Shows content of container and provide functionality to add, copy and move items.

class ContainerEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ContainerEditorWidget(QWidget* parent = nullptr);
  ~ContainerEditorWidget() override;

  void SetModel(SampleModel* model, mvvm::SessionItem* root_item = nullptr);

private:
  void OnAdd();
  void OnCopy();
  void OnRemove();
  void OnMoveDown();
  void OnMoveUp();

  std::vector<mvvm::SessionItem*> GetSelectedItems() const;
  QBoxLayout* CreateButtonLayout() const;

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ViewModel> m_view_model;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  mvvm::SessionItem* m_container{nullptr};
  SampleModel* m_model{nullptr};
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_CONTAINER_EDITOR_WIDGET_H_
