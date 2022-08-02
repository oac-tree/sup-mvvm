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
