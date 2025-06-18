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

#ifndef MVVM_SERIALIZATION_TREE_DATA_COMPOSER_H_
#define MVVM_SERIALIZATION_TREE_DATA_COMPOSER_H_

#include <mvvm/serialization/converter_types.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The TreeDataComposer class constructs TreeData structure representing SessionItem.
 */
class TreeDataComposer
{
public:
  explicit TreeDataComposer(item_filter_t item_filter);
  ~TreeDataComposer() = default;

  TreeDataComposer(const TreeDataComposer& other) = delete;
  TreeDataComposer(TreeDataComposer&& other) = delete;
  TreeDataComposer& operator=(const TreeDataComposer& other) = delete;
  TreeDataComposer& operator=(TreeDataComposer&& other) = delete;

  std::unique_ptr<tree_data_t> CreateTreeData(const SessionItem &item) const;

private:
  item_filter_t m_item_filter;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_COMPOSER_H_
