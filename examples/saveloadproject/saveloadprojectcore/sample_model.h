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

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_SAMPLE_MODEL_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace saveloadproject
{

class DemoItem : public mvvm::CompoundItem
{
public:
  DemoItem();
};

class DemoContainerItem : public mvvm::CompoundItem
{
public:
  DemoContainerItem();
};

/**
 * @brief The SampleModel is a main application model.
 */
class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();

  void AppendRandomItem(mvvm::SessionItem* container);

private:
  void PopulateModel();
};

}  // namespace saveloadproject

template <>
struct mvvm::item_traits<saveloadproject::DemoItem>
{
  static constexpr std::string_view type_name() noexcept { return "Demo"; }
};

template <>
struct mvvm::item_traits<saveloadproject::DemoContainerItem>
{
  static constexpr std::string_view type_name() noexcept { return "DemoContainer"; }
};

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_SAMPLE_MODEL_H_
