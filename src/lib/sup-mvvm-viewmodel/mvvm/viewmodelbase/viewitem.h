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

#ifndef MVVM_VIEWMODELBASE_VIEWITEM_H_
#define MVVM_VIEWMODELBASE_VIEWITEM_H_

#include <mvvm/viewmodel_export.h>

#include <QVariant>
#include <memory>
#include <vector>

namespace mvvm
{

class ViewItemDataInterface;

/**
 * @brief The ViewItem class represents a single editable/displayable entity in cells of ViewModel.
 */
class MVVM_VIEWMODEL_EXPORT ViewItem
{
public:
  ViewItem();
  explicit ViewItem(std::unique_ptr<ViewItemDataInterface> view_item_data);
  virtual ~ViewItem();

  /**
   * @brief Returns the number of child item rows that the item has.
   */
  int GetRowCount() const;

  /**
   * @brief Returns the number of child item columns that the item has.
   */
  int GetColumnCount() const;

  /**
   * @brief Appends a row containing items.
   *
   * Number of items should be the same as columnCount() (if there are already some rows). If it is
   * a first row, then items can be of any size.
   */
  void AppendRow(std::vector<std::unique_ptr<ViewItem>> items);

  /**
   * @brief Inserts a row of items at given position.
   *
   * @param row Row index.
   * @param items The row of items to insert.
   */
  void InsertRow(int row, std::vector<std::unique_ptr<ViewItem>> items);

  /**
   * @brief Removes row of items at given position.
   * Items will be deleted.
   *
   * @param row Row index.
   */
  void RemoveRow(int row);

  /**
   * @brief Clears all children.
   */
  void Clear();

  /**
   * @brief Returns GetParent view.
   */
  ViewItem* GetParent() const;

  /**
   * @brief Returns a GetChild at given position.
   *
   * @param row Row index of a GetChild.
   * @param column Column index of a GetChild.
   *
   * @return A GetChild.
   */
  ViewItem* GetChild(int row, int column) const;

  /**
   * @brief Returns underlying data GetItemData (non-const version).
   */
  ViewItemDataInterface* GetItemData();

  /**
   * @brief Returns underlying data item (const version).
   */
  const ViewItemDataInterface* GetItemData() const;

  /**
   * @brief Returns the row where the item is located in its parent's child table, or -1 if the item
   * has no parent.
   */
  int row() const;

  /**
   * @brief Returns the column where the item is located in its parent's child table, or -1 if the
   * item has no parent.
   */
  int column() const;

  /**
   * @brief Returns the data for given role according to Qt::ItemDataRole namespace definitions.
   *
   * @param qt_role Standard Qt role.
   *
   * @return QVariant representing given role.
   */
  virtual QVariant data(int qt_role) const;

  /**
   * @brief Sets the data for given role.
   *
   * Internally sets the data to the underlying presentation item.
   */
  virtual bool setData(const QVariant& value, int qt_role);

  /**
   * @brief Returns Qt's item flags.
   *
   * Converts internal SessionItem's status enable/disabled/readonly to what Qt expects.
   */
  virtual Qt::ItemFlags flags() const;

  /**
   * @brief Gets buffer vector of children.
   */
  std::vector<ViewItem*> children() const;

protected:
  void SetParent(ViewItem* parent);

  /**
   * @brief Update cached values of row and col.
   */
  void UpdatePositionCache(int row, int col);

private:
  struct ViewItemImpl;
  std::unique_ptr<ViewItemImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODELBASE_VIEWITEM_H_
