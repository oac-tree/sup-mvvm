/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/remove_item_command.h"

#include <mvvm/interfaces/model_composer_interface.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/path.h>
#include <mvvm/model/sessionitem.h>

#include <sstream>

namespace
{

std::string GenerateDescription(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
{
  (void)parent;
  std::ostringstream ostr;
  ostr << "RemoveItem: " << tag_index.tag << " " << tag_index.index;
  return ostr.str();
}

}  // namespace

namespace mvvm
{

struct RemoveItemCommand::RemoveItemCommandImpl
{
  ModelComposerInterface* m_composer{nullptr};
  Path m_parent_path;
  TagIndex m_tag_index;

  RemoveItemCommandImpl(ModelComposerInterface* composer, SessionItem* parent,
                        const TagIndex& tag_index)
      : m_composer(composer), m_tag_index(tag_index)
  {
  }
};

RemoveItemCommand::RemoveItemCommand(ModelComposerInterface* composer, SessionItem* parent,
                                     const TagIndex& tag_index)
    : p_impl(std::make_unique<RemoveItemCommandImpl>(composer, parent, tag_index))
{
  SetDescription(GenerateDescription(parent, tag_index));
  p_impl->m_parent_path = utils::PathFromItem(parent);
}

RemoveItemCommand::~RemoveItemCommand() = default;

void RemoveItemCommand::ExecuteImpl() {}

void RemoveItemCommand::UndoImpl() {}

}  // namespace mvvm
