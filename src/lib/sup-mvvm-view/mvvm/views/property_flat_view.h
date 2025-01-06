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

#ifndef MVVM_VIEWS_PROPERTY_FLAT_VIEW_H_
#define MVVM_VIEWS_PROPERTY_FLAT_VIEW_H_

#include <mvvm/view_export.h>

#include <QWidget>
#include <memory>

class QGridLayout;
class QAbstractItemModel;

namespace mvvm
{

class PropertyGridController;

//! Widget holding grid layout with editors and intended for displaying all properties of given
//! SessionItem.

class MVVM_VIEW_EXPORT PropertyFlatView : public QWidget
{
  Q_OBJECT

public:
  explicit PropertyFlatView(QWidget* parent = nullptr);
  ~PropertyFlatView() override;

  void SetViewModel(QAbstractItemModel* view_model);

private:
  void UpdateGridLayout();

  std::unique_ptr<PropertyGridController> m_grid_controller;
  QGridLayout* m_grid_layout{nullptr};
  QAbstractItemModel* m_view_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_PROPERTY_FLAT_VIEW_H_
