// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H
#define CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H

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
  mvvm::AllItemsTreeView* m_treeView{nullptr};
};

#endif  // CONCURRENTPLOTCORE_GRAPHPROPERTYWIDGET_H
