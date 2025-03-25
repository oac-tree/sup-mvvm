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

#ifndef MVVM_SERIALIZATION_I_TREE_DATA_ITEM_CONVERTER_H_
#define MVVM_SERIALIZATION_I_TREE_DATA_ITEM_CONVERTER_H_

#include <mvvm/model_export.h>
#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>

namespace mvvm
{

class SessionItem;

/**
 * @brief The ITreeDataItemConverter class is a base for all converters of SessionItem to/from
 * TreeData object.
 */
class MVVM_MODEL_EXPORT ITreeDataItemConverter
{
public:
  ITreeDataItemConverter() = default;
  virtual ~ITreeDataItemConverter() = default;

  ITreeDataItemConverter(const ITreeDataItemConverter&) = delete;
  ITreeDataItemConverter& operator=(const ITreeDataItemConverter&) = delete;
  ITreeDataItemConverter(ITreeDataItemConverter&&) = delete;
  ITreeDataItemConverter& operator=(ITreeDataItemConverter&&) = delete;

  /**
   * @brief Creates TreeData from SessionItem.
   */
  virtual std::unique_ptr<tree_data_t> ToTreeData(const SessionItem& item) const = 0;

  /**
   * @brief Creates SessionItem from TreeData.
   */
  virtual std::unique_ptr<SessionItem> ToSessionItem(const tree_data_t& tree_data) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_I_TREE_DATA_ITEM_CONVERTER_H_
