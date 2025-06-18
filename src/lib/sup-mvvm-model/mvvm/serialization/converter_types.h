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

#ifndef MVVM_SERIALIZATION_CONVERTER_TYPES_H_
#define MVVM_SERIALIZATION_CONVERTER_TYPES_H_

//! @file
//! Collection of custom types involved into SessionItem to TreeData serialization.

#include <cstdint>
#include <functional>

namespace mvvm
{

class SessionItem;
class IItemFactory;

//! Defines a function to filter out item during conversion to TreeData.
using item_filter_t = std::function<bool(const SessionItem&)>;

/**
 * @brief The ConverterMode enum defines flags to steer converter behavior from SessionItem to
 * TreeData and back.
 */
enum class ConverterMode : std::uint8_t
{
  kNone,   //!< undefined converter mode
  kClone,  //!< full deep copying with item identifiers preserved
  kCopy    //!< full deep copying with item identifiers regenerated
};

/**
 * @brief The context necessary to build SessionItem from serialized content.
 */
struct ItemComposerContext
{
  const IItemFactory* factory{nullptr};
  ConverterMode mode{ConverterMode::kNone};
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_CONVERTER_TYPES_H_
