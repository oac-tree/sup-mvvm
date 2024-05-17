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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_USER_INTERACTOR_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_USER_INTERACTOR_H_

#include <mvvm/project/project_context.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockUserInteractor class helps to test ProjectManagerDecorator and its callbacks from
 * UserInteractionContext.
 */
class MockUserInteractor
{
public:
  MOCK_METHOD(std::string, GetExistingProjectPath, (), (const));
  MOCK_METHOD(std::string, OnGetNewProjectPath, (), (const));
  MOCK_METHOD(mvvm::SaveChangesAnswer, OnSaveChangesRequest, (), (const));

  UserInteractionContext CreateContext();
};
}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_USER_INTERACTOR_H_
