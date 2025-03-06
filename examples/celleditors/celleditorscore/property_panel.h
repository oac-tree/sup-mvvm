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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef CELLEDITORS_CELLEDITORSCORE_PROPERTY_PANEL_H_
#define CELLEDITORS_CELLEDITORSCORE_PROPERTY_PANEL_H_

#include <QWidget>
#include <memory>

class QBoxLayout;
class QTreeView;
class QTableView;

namespace mvvm
{
class SessionItem;
}

namespace mvvm
{
class ViewModel;
class ViewModelDelegate;
}  // namespace mvvm

namespace celleditors
{

class SampleModel;

//! Shows content of the model as vertical tree, horizontal tree and table.

class PropertyPanel : public QWidget
{
  Q_OBJECT

public:
  explicit PropertyPanel(QWidget* parent = nullptr);
  ~PropertyPanel() override;

  void SetItem(mvvm::SessionItem* item);

private:
  QBoxLayout* CreateLeftLayout();
  QBoxLayout* CreateRightLayout();

  QTreeView* m_vertical_tree{nullptr};
  QTreeView* m_horizontal_tree{nullptr};
  QTableView* m_table_view{nullptr};

  std::unique_ptr<mvvm::ViewModel> m_vertical_view_model;
  std::unique_ptr<mvvm::ViewModel> m_horizontal_view_model;

  // Qt likes that every view has its own delegates
  std::unique_ptr<mvvm::ViewModelDelegate> m_vertical_delegate;
  std::unique_ptr<mvvm::ViewModelDelegate> m_horizontal_delegate;
  std::unique_ptr<mvvm::ViewModelDelegate> m_table_delegate;
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_PROPERTY_PANEL_H_
