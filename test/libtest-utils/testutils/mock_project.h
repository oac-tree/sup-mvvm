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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_PROJECT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_PROJECT_H_

#include <mvvm/project/i_project.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockProject class mocks IProject interface and intended for IProjectManager testing.
 */
class MockProject : public mvvm::IProject
{
public:
  MOCK_METHOD(mvvm::ProjectType, GetProjectType, (), (const, override));
  MOCK_METHOD(std::string, GetApplicationType, (), (const, override));
  MOCK_METHOD(std::string, GetPath, (), (const, override));
  MOCK_METHOD(bool, HasPath, (), (const, override));
  MOCK_METHOD(bool, Save, (const std::string&), (override));
  MOCK_METHOD(bool, Load, (const std::string&), (override));
  MOCK_METHOD(bool, IsModified, (), (const, override));
  MOCK_METHOD(bool, CreateEmpty, (), (override));
  MOCK_METHOD(bool, Close, (), (override));
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_PROJECT_H_
