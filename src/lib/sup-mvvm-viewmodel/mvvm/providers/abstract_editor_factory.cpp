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

#include "abstract_editor_factory.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{

void AbstractEditorFactory::RegisterBuilder(const std::string& name, editorbuilder_t builder)
{
  auto it = m_name_to_builder.find(name);
  if (it != m_name_to_builder.end())
  {
    throw RuntimeException("AbstractEditorFactory: editor name exists");
  }

  m_name_to_builder.insert(it, {name, std::move(builder)});
}

editorbuilder_t AbstractEditorFactory::FindBuilder(const std::string& name) const
{
  auto it = m_name_to_builder.find(name);
  return it != m_name_to_builder.end() ? it->second : editorbuilder_t();
}

}  // namespace mvvm
