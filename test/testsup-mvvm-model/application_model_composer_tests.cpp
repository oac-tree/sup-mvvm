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

#include "mvvm/model/application_model_composer.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_event_handler.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests of ApplicationModelComposer class.
 */
class ApplicationModelComposerTests : public ::testing::Test
{
public:
  std::unique_ptr<ApplicationModelComposer> CreateComposer()
  {
    return std::make_unique<ApplicationModelComposer>(&m_model, &m_event_handler, &m_commands);
  }

  SessionModel m_model;
  ModelEventHandler m_event_handler;
  CommandStack m_commands;
};

TEST_F(ApplicationModelComposerTests, InitialState) {}
