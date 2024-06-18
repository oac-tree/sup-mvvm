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

#ifndef MVVM_VIEWMODEL_PROPERTY_TABLE_VIEWMODEL_H_
#define MVVM_VIEWMODEL_PROPERTY_TABLE_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class ISessionModel;

//! View model to show content of SessionModel in Qt widgets: all item properties as a table row.
//! Intended to show registered properties of items in table-like view.
//! Registered properties will form columns of the table, top level items will form table rows.

class MVVM_VIEWMODEL_EXPORT PropertyTableViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit PropertyTableViewModel(ISessionModel* model, QObject* parent = nullptr);

  void insertRow(ViewItem* parent, int row, std::vector<std::unique_ptr<ViewItem>> items) override;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_PROPERTY_TABLE_VIEWMODEL_H_
