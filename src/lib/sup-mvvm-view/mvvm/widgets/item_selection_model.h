/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_ITEM_SELECTION_MODEL_H_
#define MVVM_WIDGETS_ITEM_SELECTION_MODEL_H_

#include <mvvm/model/sessionitem.h>
#include <mvvm/view_export.h>

#include <QItemSelectionModel>

namespace mvvm
{

class ViewModel;
class SessionItem;

class MVVM_VIEW_EXPORT ItemSelectionModel : public QItemSelectionModel
{
  Q_OBJECT

public:
  explicit ItemSelectionModel(mvvm::ViewModel* view_model = nullptr, QObject* parent = nullptr);

  void SetViewModel(mvvm::ViewModel* view_model);

  const mvvm::SessionItem* GetSelectedItem() const;

  std::vector<const mvvm::SessionItem*> GetSelectedItems() const;

  void SetSelectedItem(const mvvm::SessionItem* item);

  void SetSelectedItems(std::vector<const mvvm::SessionItem*> items);

signals:
  void SelectedItemChanged(const mvvm::SessionItem*);

private:
  void OnViewModelReset();
  const mvvm::ViewModel* GetViewModel() const;
};

}  // namespace mvvm

Q_DECLARE_METATYPE(const mvvm::SessionItem*)
Q_DECLARE_METATYPE(mvvm::SessionItem*)

#endif  // MVVM_WIDGETS_ITEM_SELECTION_MODEL_H_
