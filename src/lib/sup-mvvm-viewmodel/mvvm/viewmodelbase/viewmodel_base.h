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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODELBASE_VIEWMODEL_BASE_H_
#define MVVM_VIEWMODELBASE_VIEWMODEL_BASE_H_

#include <mvvm/viewmodel_export.h>

#include <QAbstractItemModel>
#include <memory>

namespace mvvm
{

class ViewItem;

/**
 * @brief The ViewModelBase class implements Qt's abstract view model to form trees with fixed
 * number of colunmns.
 *
 * ViewModelBase is made of ViewItems, where each item can carry data, i.e. a collection of
 * data/role pairs. The number of columns in a tree is fixed and stays the same from row to row. The
 * first item in a row might have a children's rows attached.
 *
 * Here the model has invisible root item, which contains a single row of items with 3 columns
 * (item00, item01, item02). The first item00 in this row, is a parent for 6 other children, forming
 * (rowxcol) = (2x3) table.
 *
 * item00      item01  item02
 *    child00  child01 child02
 *    child00  child11 child12
 */
class MVVM_VIEWMODEL_EXPORT ViewModelBase : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit ViewModelBase(QObject* parent_object = nullptr);
  ~ViewModelBase() override;

  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

  QModelIndex parent(const QModelIndex& child) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  /**
   * @brief Returns a pointer to invisible root item (const version).
   */
  const ViewItem* rootItem() const;

  /**
   * @brief Returns a pointer to invisible root item (non-const version).
   */
  ViewItem* rootItem();

  /**
   * @brief Returns view item associated with the given index (const version).
   *
   * If index is invalid, returns nullptr.
   */
  const ViewItem* itemFromIndex(const QModelIndex& index) const;

  /**
   * @brief Returns view item associated with the given index (non-const version).
   *
   * If index is invalid, returns nullptr.
   */
  ViewItem* itemFromIndex(const QModelIndex& index);

  /**
   * @brief Returns the QModelIndex associated with the given item.
   */
  QModelIndex indexFromItem(const ViewItem* item) const;

  /**
   * @brief Removes row of items at given position.
   * Items will be deleted.
   *
   * @param row Row index.
   */
  void removeRow(ViewItem* parent, int row);

  /**
   * @brief Clears all children fof given parent.
   */
  void clearRows(ViewItem* parent);

  /**
   * @brief Inserts a row of items at index 'row' to given parent.
   */
  virtual void insertRow(ViewItem* parent, int row, std::vector<std::unique_ptr<ViewItem>> items);

  /**
   * @brief Appends a row of items at the end of vector of rows.
   */
  void appendRow(ViewItem* parent, std::vector<std::unique_ptr<ViewItem>> items);

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief  Sets new root item.
   *
   * Previous item will be deleted, model will be reset.
   *
   * @param root_item A new root item.
   * @param notify Will notify external views about model reset, if true.
   */
  void ResetRootViewItem(std::unique_ptr<ViewItem> root_item, bool notify = true);

  /**
   * @brief Allows to call interntal notification method from outside.
   */
  void BeginResetModelNotify();

  /**
   * @brief Allows to call interntal notification method from outside.
   */
  void EndResetModelNotify();

private:
  struct ViewModelBaseImpl;
  std::unique_ptr<ViewModelBaseImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODELBASE_VIEWMODEL_BASE_H_
