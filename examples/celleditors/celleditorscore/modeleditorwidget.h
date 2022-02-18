// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CELLEDITORSCORE_MODELEDITORWIDGET_H
#define CELLEDITORSCORE_MODELEDITORWIDGET_H

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
  virtual ~ModelEditorWidget() override;

  void setModel(SampleModel* model);

private:
  QBoxLayout* createLeftLayout();
  QBoxLayout* createRightLayout();

  QTreeView* m_verticalTree{nullptr};
  QTreeView* m_horizontalTree{nullptr};
  QTableView* m_tableView{nullptr};

  std::unique_ptr<mvvm::ViewModel> m_verticalViewModel;
  std::unique_ptr<mvvm::ViewModel> m_horizontalViewModel;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
};

}  // namespace celleditors

#endif  // CELLEDITORSCORE_MODELEDITORWIDGET_H
