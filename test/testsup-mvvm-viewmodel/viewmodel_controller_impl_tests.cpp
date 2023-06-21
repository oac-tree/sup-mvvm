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

#include "mvvm/viewmodel/viewmodel_controller_impl.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/viewmodelbase/viewmodel_base.h>

#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

using namespace mvvm;

//! Tests for ViewModelControllerImpl class.

class ViewModelControllerImplTests : public ::testing::Test
{
public:
  std::unique_ptr<ViewModelControllerImpl> CreateController(SessionModelInterface* model,
                                                            ViewModelBase* view_model)
  {
    auto result = std::make_unique<ViewModelControllerImpl>(model, view_model);
    result->SetChildrenStrategy(std::make_unique<AllChildrenStrategy>());
    result->SetRowStrategy(std::make_unique<LabelDataRowStrategy>());
    return result;
  }

  std::unique_ptr<ViewModelControllerImpl> CreateController()
  {
    return CreateController(&m_model, &m_viewmodel);
  }

  ApplicationModel m_model;
  ViewModelBase m_viewmodel;
};

//! Empty model.

TEST_F(ViewModelControllerImplTests, EmptyProcedure) {}
