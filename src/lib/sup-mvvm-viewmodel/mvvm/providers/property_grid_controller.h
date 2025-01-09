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

#ifndef MVVM_PROVIDERS_PROPERTY_GRID_CONTROLLER_H_
#define MVVM_PROVIDERS_PROPERTY_GRID_CONTROLLER_H_

#include <mvvm/view_export.h>

#include <QObject>
#include <memory>
#include <vector>

class QAbstractItemModel;
class QDataWidgetMapper;

namespace mvvm
{

class ViewModelDelegate;

/**
 * @brief The PropertyGridController class assists PropertyFlatView in the generation of a 2D array
 * of editors to populate the grid layout.
 *
 * It takes as an input viewmodel representing a table and generates a 2D array of widgets, where
 * each widget is intended for editing/displaying the cell of the model. It is the responsibility of
 * the user to populate the grid layout.
 *
 * Controller also holds a collection of delegates/and mappers to propagate editing activity back to
 * the model.
 */
class MVVM_VIEW_EXPORT PropertyGridController : public QObject
{
  Q_OBJECT

public:
  using widgets_t = std::unique_ptr<QWidget>;
  using widget_row_t = std::vector<widgets_t>;

  explicit PropertyGridController(QAbstractItemModel* model, QObject* parent_object = nullptr);
  ~PropertyGridController() override;

  /**
   * @brief Creates a 2D array of widgets for editing model cells.
   *
   * Additionally, creates all necessary mappers to propagate edited values from widgets back to
   * the model. Ownership for widgets goes to the user, while mappers remain here.
   */
  std::vector<widget_row_t> CreateWidgetGrid();

  /**
   * @brief Submits all changes from mapped widgets back to the model.
   */
  bool Submit();

signals:
  /**
   * @brief The signal is emitted when the model changes its layout, meaning some rows/columns have
   * been inserted or removed. In the current simplified approach, we must re-populate the grid
   * layout from scratch.
   */
  void GridChanged();

private:
  /**
   * @brief Create widget for a given model cell.
   */
  std::unique_ptr<QWidget> CreateWidget(const QModelIndex& index);

  /**
   * @brief Clear all mappers and delegates.
   */
  void ClearContent();

  /**
   * @brief Checks if current cell has to be represented by the label.
   */
  bool IsLabel(const QModelIndex& index) const;

  /**
   * @brief Creates label for representing given model cell.
   */
  std::unique_ptr<QWidget> CreateLabel(const QModelIndex& index);

  /**
   * @brief Creates editor for given model cell.
   */
  std::unique_ptr<QWidget> CreateEditor(const QModelIndex& index);

  /**
   * @brief Process model layout change.
   */
  void OnLayoutChange();

  /**
   * @brief Regenerates all mappers.
   */
  void UpdateMappers();

  /**
   * @brief Start listening the model.
   */
  void SetupConnections(QAbstractItemModel* model);

  QAbstractItemModel* m_view_model{nullptr};
  std::vector<std::unique_ptr<ViewModelDelegate>> m_delegates;
  std::vector<std::unique_ptr<QDataWidgetMapper>> m_widget_mappers;
  std::vector<std::vector<QWidget*>> m_widgets;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_PROPERTY_GRID_CONTROLLER_H_
