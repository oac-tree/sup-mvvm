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

#ifndef MVVM_SERIALIZATION_TREE_DATA_ITEM_CONVERTER_H_
#define MVVM_SERIALIZATION_TREE_DATA_ITEM_CONVERTER_H_

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/i_tree_data_item_converter.h>

namespace mvvm
{

/**
 * @brief The TreeDataItemConverter class is a main converter from SessionItem to TreeData and back.
 *
 * On the way from SessionItem to TreeData creates exact copy of the data. On the way back,
 * can create either a full clone or a deep copy (new identifiers).
 */
class TreeDataItemConverter : public ITreeDataItemConverter
{
public:
  TreeDataItemConverter(const IItemFactory* factory, ConverterMode mode,
                        const item_filter_t& filter_func = {});

  std::unique_ptr<SessionItem> ToSessionItem(const tree_data_t& tree_data) const override;

  std::unique_ptr<tree_data_t> ToTreeData(const SessionItem& item) const override;

private:
  const IItemFactory* m_factory{nullptr};
  ConverterMode m_converter_mode;
  item_filter_t m_item_filter_func;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_ITEM_CONVERTER_H_
