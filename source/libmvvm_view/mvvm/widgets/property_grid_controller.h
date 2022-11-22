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

#ifndef MVVM_WIDGETS_PROPERTY_GRID_CONTROLLER_H_
#define MVVM_WIDGETS_PROPERTY_GRID_CONTROLLER_H_

#include <mvvm/view_export.h>

#include <QObject>
#include <memory>
#include <vector>

class QAbstractItemModel;
class QDataWidgetMapper;

namespace mvvm
{

class ViewModelDelegate;

//!

class MVVM_VIEW_EXPORT PropertyGridController : public QObject
{
  Q_OBJECT

public:
  using widgets_t = std::unique_ptr<QWidget>;
  using widget_row_t = std::vector<widgets_t>;

  explicit PropertyGridController(QAbstractItemModel* model, QObject* parent = nullptr);
  ~PropertyGridController() override;

  std::vector<widget_row_t> CreateWidgetGrid();

  bool Submit();

signals:
  void GridChanged();

private:
  std::unique_ptr<QWidget> CreateWidget(const QModelIndex& index);
  void ClearContent();
  bool IsLabel(const QModelIndex& index) const;
  std::unique_ptr<QWidget> CreateLabel(const QModelIndex& index);
  std::unique_ptr<QWidget> CreateEditor(const QModelIndex& index);
  void OnLayoutChange();
  void UpdateMappers();
  void SetupConnections(QAbstractItemModel* model);

  QAbstractItemModel* m_view_model{nullptr};

  std::vector<std::unique_ptr<ViewModelDelegate>> m_delegates;
  std::vector<std::unique_ptr<QDataWidgetMapper>> m_widget_mappers;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_PROPERTY_GRID_CONTROLLER_H_
