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

#include "mock_project_context.h"

namespace mvvm::test
{

ProjectContext MockProjectContext::CreateContext(const std::string &application_type)
{
  ProjectContext result;
  result.modified_callback = [this]() { OnModified(); };
  result.loaded_callback = [this]() { OnLoaded(); };
  result.saved_callback = [this]() { OnSaved(); };
  result.application_type = application_type;
  return result;
}

}  // namespace mvvm::test
