// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CELLEDITORS_CELLEDITORSCORE_MODELEDITORWIDGET_H
#define CELLEDITORS_CELLEDITORSCORE_MODELEDITORWIDGET_H

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

#endif  // CELLEDITORS_CELLEDITORSCORE_MODELEDITORWIDGET_H
