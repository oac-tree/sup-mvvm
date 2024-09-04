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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_

#include "mvvm/commands/abstract_command.h"

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief The MockCommand class to test AbstractCommand.
 */
class MockCommand : public AbstractCommand
{
public:
  MOCK_METHOD(void, ExecuteImpl, ());
  MOCK_METHOD(void, UndoImpl, ());
};

/**
 * @brief The CommandDecorator class decorates MockCommand to use together with CommandStack.
 *
 * It prevents passing ownership of a mock  object to the CommandStack, thus resolving googletest
 * warning related to testing::Mock::AllowLeak.
 */
class CommandDecorator : public AbstractCommand
{
public:
  explicit CommandDecorator(MockCommand& decoratee) : m_decoratee(&decoratee) {}

  bool IsObsolete() const override { return m_decoratee->IsObsolete(); }

  void SetMakeObsoleteAfterExecution() { m_make_obsolete = true; }

private:
  void ExecuteImpl() override
  {
    m_decoratee->ExecuteImpl();
    if (m_make_obsolete)
    {
      m_decoratee->SetIsObsolete(true);
    }
  }

  void UndoImpl() override { m_decoratee->UndoImpl(); }

  MockCommand* m_decoratee{nullptr};
  bool m_make_obsolete{false};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_COMMAND_H_
