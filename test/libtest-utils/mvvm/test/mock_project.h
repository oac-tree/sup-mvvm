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
  MOCK_METHOD(std::string, GetProjectPath, (), (const, override));
  MOCK_METHOD(bool, Save, (const std::string&), (override));
  MOCK_METHOD(bool, Load, (const std::string&), (override));
  MOCK_METHOD(bool, IsModified, (), (const, override));
};

/**
 * @brief The ProjectDecorator class is a decorator that simply forwards all calls to underlying
 * project.
 *
 * It is used when unique_ptr is required, but we don't want to pass ownership to real object. For
 * example, gtest doesn't like gmock objects to be moved from the test to somewhere else.
 */
class ProjectDecorator : public mvvm::IProject
{
public:
  explicit ProjectDecorator(mvvm::IProject* project);

  ProjectType GetProjectType() const override;
  std::string GetProjectPath() const override;
  bool Save(const std::string& path) override;
  bool Load(const std::string& path) override;
  bool IsModified() const override;

private:
  mvvm::IProject* m_project{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_PROJECT_H_
