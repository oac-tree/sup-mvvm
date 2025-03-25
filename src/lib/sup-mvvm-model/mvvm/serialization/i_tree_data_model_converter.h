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

#ifndef MVVM_SERIALIZATION_I_TREE_DATA_MODEL_CONVERTER_H_
#define MVVM_SERIALIZATION_I_TREE_DATA_MODEL_CONVERTER_H_

#include <mvvm/model_export.h>
#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The ITreeDataModelConverter class is a base for all converters of SessionModel to/from
 * TreeData object.
 */
class MVVM_MODEL_EXPORT ITreeDataModelConverter
{
public:
  ITreeDataModelConverter() = default;
  virtual ~ITreeDataModelConverter() = default;

  ITreeDataModelConverter(const ITreeDataModelConverter&) = delete;
  ITreeDataModelConverter& operator=(const ITreeDataModelConverter&) = delete;
  ITreeDataModelConverter(ITreeDataModelConverter&&) = delete;
  ITreeDataModelConverter& operator=(ITreeDataModelConverter&&) = delete;

  /**
   * @brief Creates TreeData from SessionModel.
   */
  virtual std::unique_ptr<tree_data_t> ToTreeData(const ISessionModel& item) const = 0;

  /**
   * @brief Populates empty SessionModel from TreeData.
   */
  virtual void PopulateSessionModel(const tree_data_t&, ISessionModel&) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_I_TREE_DATA_MODEL_CONVERTER_H_
