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

#ifndef FLATVIEW_FLATVIEWCORE_FLAT_WIDGET_H_
#define FLATVIEW_FLATVIEWCORE_FLAT_WIDGET_H_

#include <QWidget>
#include <functional>
#include <memory>

namespace mvvm
{
class SessionItem;
class SessionModelInterface;
class ViewModel;
class PropertyFlatView;
}  // namespace mvvm

namespace flatview
{

class FlatWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FlatWidget(QWidget* parent = nullptr);
  ~FlatWidget() override;

  void SetViewModel(std::unique_ptr<mvvm::ViewModel> view_model);

  void SetItem(mvvm::SessionItem* item);

private:
  std::unique_ptr<mvvm::ViewModel> m_view_model;
  mvvm::PropertyFlatView* m_flat_view{nullptr};
};

}  // namespace flatview

#endif  // FLATVIEW_FLATVIEWCORE_FLAT_WIDGET_H_
