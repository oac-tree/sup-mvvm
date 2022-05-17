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

#ifndef MVVM_WIDGETS_ABSTRACTITEMVIEW_H
#define MVVM_WIDGETS_ABSTRACTITEMVIEW_H

#include "mvvm/view_export.h"
#include <QWidget>

class QAbstractItemView;

namespace mvvm
{

class ApplicationModel;
class SessionItem;
class ItemViewComponentProvider;

//! Generic view to show SessionModel in Qt lists, trees and tables.

class MVVM_VIEW_EXPORT AbstractItemView : public QWidget
{
  Q_OBJECT

public:
  explicit AbstractItemView(QWidget* parent = nullptr);
  ~AbstractItemView() override;

  void SetComponentProvider(std::unique_ptr<ItemViewComponentProvider> provider);

  void SetApplicationModel(ApplicationModel* model);

  virtual void SetItem(SessionItem* item);

  ItemViewComponentProvider* GetComponentProvider();

  mvvm::SessionItem* GetSelectedItem() const;
  void SetSelectedItem(mvvm::SessionItem* item);

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  virtual void UpdateView();
  std::unique_ptr<ItemViewComponentProvider> m_provider;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ABSTRACTITEMVIEW_H
