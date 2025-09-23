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

#include "tree_data_item_converter.h"

#include "session_item_composer.h"
#include "tree_data.h"  // IWYU pragma: export
#include "tree_data_composer.h"

#include <mvvm/model/session_item.h>

namespace mvvm
{

TreeDataItemConverter::TreeDataItemConverter(const IItemFactory* factory, ConverterMode mode,
                                             const item_filter_t& filter_func)
    : m_factory(factory), m_converter_mode(mode), m_item_filter_func(filter_func)
{
}

std::unique_ptr<SessionItem> TreeDataItemConverter::ToSessionItem(
    const tree_data_t& tree_data) const
{
  const SessionItemComposer composer({m_factory, m_converter_mode});
  return composer.CreateSessionItem(tree_data);
}

std::unique_ptr<tree_data_t> TreeDataItemConverter::ToTreeData(const SessionItem& item) const
{
  const TreeDataComposer composer(m_item_filter_func);
  return composer.CreateTreeData(item);
}

}  // namespace mvvm
