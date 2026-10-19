/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
#define CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace mvvm
{
class ContainerItem;
}

namespace celleditors
{

/**
 * @brief The BasicPropertyItem is used to create demo items populated with basic properties.
 */
class BasicPropertyItem : public mvvm::CompoundItem
{
public:
  BasicPropertyItem();
};

/**
 * @brief The AllIntPropertyItem is used to create demo items containing various integer properties.
 *
 * We investigate the behavior of the original QSpinBox vs our AllIntSpinBox.
 */
class AllIntPropertyItem : public mvvm::CompoundItem
{
public:
  AllIntPropertyItem();
};

/**
 * @brief The OptionalIntPropertyItem is used to create demo items containing integer properties
 * which can have no value.
 *
 * No value is an empty cell. The first step in the editor sets the default value. Clearing the
 * text removes the value.
 */
class OptionalIntPropertyItem : public mvvm::CompoundItem
{
public:
  OptionalIntPropertyItem();
};

/**
 * @brief The FloatPropertyItem is used to create demo items containing various float properties.
 *
 * We investigate the behavior of the original QDoubleSpinBox vs our FloatSpinBoxEditor.
 */
class FloatPropertyItem : public mvvm::CompoundItem
{
public:
  FloatPropertyItem();
};

/**
 * @brief The CheckboxItem is used to create demo items with boolean properties to investigate the
 * behavior of the CheckBox editor.
 */
class CheckboxItem : public mvvm::CompoundItem
{
public:
  CheckboxItem();
};

/**
 * @brief The SampleModel is the main model of this application, it contains all demo items.
 */
class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();

  void PopulateModel();

  std::uint32_t GetContainerCount() const;

  mvvm::ContainerItem* GetContainer(std::uint32_t index);
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
