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

#include "mock_user_interactor.h"

namespace mvvm::test
{

UserInteractionContext MockUserInteractor::CreateContext()
{
  UserInteractionContext result;

  result.existing_path_callback = [this]() { return GetExistingProjectPath(); };
  result.new_path_callback = [this]() { return OnGetNewProjectPath(); };
  result.answer_callback = [this]() { return OnSaveChangesRequest(); };
  result.message_callback = [this](const std::string& message) { OnMessage(message); };

  return result;
}

}  // namespace mvvm::test
