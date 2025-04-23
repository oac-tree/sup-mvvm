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

#ifndef MVVM_TEST_MOCK_PROJECT_CONTEXT_H_
#define MVVM_TEST_MOCK_PROJECT_CONTEXT_H_

#include <mvvm/project/project_context.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockProjectContext class helps to test notifications triggered by the AbstractProject.
 */
class MockProjectContext
{
public:
  MOCK_METHOD(void, OnModified, (), (const));
  MOCK_METHOD(void, OnLoaded, (), (const));
  MOCK_METHOD(void, OnSaved, (), (const));

  ProjectContext CreateContext(const std::string& application_type = {});
};

}  // namespace mvvm::test

#endif  // MVVM_TEST_MOCK_PROJECT_CONTEXT_H_
