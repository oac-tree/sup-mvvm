// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef DRAGANDMOVECORE_CONTAINEREDITORWIDGET_H
#define DRAGANDMOVECORE_CONTAINEREDITORWIDGET_H

#include <QWidget>
#include <memory>
#include <vector>

class QTreeView;
class QBoxLayout;
class QItemSelectionModel;

namespace mvvm {
class ViewModel;
class ViewModelDelegate;
class SessionItem;
} // namespace ModelView

namespace DragAndMove {

class SampleModel;

//! Shows content of container and provide functionality to add, copy and move items.

class ContainerEditorWidget : public QWidget {
    Q_OBJECT

public:
    explicit ContainerEditorWidget(QWidget* parent = nullptr);
    ~ContainerEditorWidget();

    void setModel(SampleModel* model, mvvm::SessionItem* root_item = nullptr);

private:
    void onAdd();
    void onCopy();
    void onRemove();
    void onMoveDown();
    void onMoveUp();

    std::vector<mvvm::SessionItem*> selected_items() const;
    QBoxLayout* create_button_layout();

    QTreeView* m_treeView{nullptr};
    std::unique_ptr<mvvm::ViewModel> m_viewModel;
    std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
    mvvm::SessionItem* m_container{nullptr};
    SampleModel* m_model{nullptr};
};

} // namespace DragAndMove

#endif // DRAGANDMOVECORE_CONTAINEREDITORWIDGET_H
