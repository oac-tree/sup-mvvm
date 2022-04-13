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

#ifndef TESTS_TESTMVVM_VIEW_ABSTRACTITEMVIEW_TEST_H
#define TESTS_TESTMVVM_VIEW_ABSTRACTITEMVIEW_TEST_H

#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/widgets/abstractitemview.h"

#include <QTreeView>
#include <memory>

namespace testutils
{

class TestView : public mvvm::AbstractItemView
{
  Q_OBJECT

public:
  explicit TestView(mvvm::ApplicationModel* model) : AbstractItemView(model)
  {
    SetView(new QTreeView);
  }

private:
  std::unique_ptr<mvvm::ViewModel> CreateViewModel(mvvm::ApplicationModel* model) override
  {
    return std::make_unique<mvvm::TopItemsViewModel>(model);
  }
};

}  // namespace testutils

#endif  // TESTS_TESTMVVM_VIEW_ABSTRACTITEMVIEW_TEST_H
