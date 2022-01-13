/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODELBASE_VIEWITEM_H
#define MVVM_VIEWMODELBASE_VIEWITEM_H

#include "mvvm/viewmodel_export.h"

#include <QVariant>
#include <memory>
#include <vector>

namespace mvvm
{
class ViewItemDataInterface;

//! Represents a single editable/displayable entity in cells of ViewModelBase.

class MVVM_VIEWMODEL_EXPORT ViewItem
{
public:
  ViewItem();
  ViewItem(std::unique_ptr<ViewItemDataInterface> view_item_data);
  virtual ~ViewItem();

  int rowCount() const;

  int columnCount() const;

  void appendRow(std::vector<std::unique_ptr<ViewItem>> items);

  void insertRow(int row, std::vector<std::unique_ptr<ViewItem>> items);

  void removeRow(int row);

  void clear();

  ViewItem* parent() const;

  ViewItem* child(int row, int column) const;

  ViewItemDataInterface* item();

  const ViewItemDataInterface* item() const;

  int row() const;

  int column() const;

  virtual QVariant data(int qt_role) const;

  virtual bool setData(const QVariant& value, int qt_role);

  virtual Qt::ItemFlags flags() const;

  std::vector<ViewItem*> children() const;

protected:
  void setParent(ViewItem* parent);

private:
  struct ViewItemImpl;
  std::unique_ptr<ViewItemImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODELBASE_VIEWITEM_H
