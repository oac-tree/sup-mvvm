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
}  // namespace mvvm

namespace flatview
{

class SampleModel;

class FlatWidget : public QWidget
{
  Q_OBJECT

public:
  using create_viewmodel_t =
      std::function<std::unique_ptr<mvvm::ViewModel>(mvvm::SessionModelInterface*)>;

  explicit FlatWidget(QWidget* parent = nullptr);

  void SetApplicationModel(mvvm::SessionModelInterface* model);

  void SetItem(mvvm::SessionItem* item);

private:
};

template <typename T>
std::unique_ptr<T> CreateViewModel(mvvm::SessionModelInterface* model)
{
  return std::make_unique<T>(model);
}

}  // namespace flatview

#endif  // FLATVIEW_FLATVIEWCORE_FLAT_WIDGET_H_
