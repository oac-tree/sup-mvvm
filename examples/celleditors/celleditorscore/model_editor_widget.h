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

#ifndef CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
#define CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QBoxLayout;
class QTreeView;
class QTableView;

namespace mvvm
{
class ViewModel;
class ViewModelDelegate;
}  // namespace mvvm

namespace celleditors
{

class SampleModel;

//! Shows content of the model as vertical tree, horizontal tree and table.

class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(SampleModel* model = nullptr, QWidget* parent = nullptr);
  ~ModelEditorWidget() override;

  void SetModel(SampleModel* model);

private:
  QBoxLayout* CreateLeftLayout();
  QBoxLayout* CreateRightLayout();

  QTreeView* m_vertical_tree{nullptr};
  QTreeView* m_horizontal_tree{nullptr};
  QTableView* m_table_view{nullptr};

  std::unique_ptr<mvvm::ViewModel> m_vertical_view_model;
  std::unique_ptr<mvvm::ViewModel> m_horizontal_view_model;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
