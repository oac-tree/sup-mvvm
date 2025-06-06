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
//! Collection of custom types involved into SessionItem and TreeData mutual conversion.

#include <mvvm/model_export.h>
#include <mvvm/serialization/tree_data_fwd.h>

#include <functional>
#include <memory>

namespace mvvm
{

class SessionItem;

//! Defines function to create TreeData object from SessionItem.
using create_treedata_t = std::function<std::unique_ptr<tree_data_t>(const SessionItem&)>;

//! Defines function to create SessionItem object from TreeData.
using create_item_t = std::function<std::unique_ptr<SessionItem>(const tree_data_t&)>;

//! Defines function to populate existing SessionItem from its' TreeData representaion.
using update_item_t = std::function<void(const tree_data_t&, SessionItem&)>;

//! Defines a function to filter out item during conversion to TreeData.
using filter_item_t = std::function<bool(const SessionItem&)>;

//! Provides necessary callbacks to convert SessionItem to TreeData and back.

struct MVVM_MODEL_EXPORT ConverterCallbacks
{
  create_treedata_t create_treedata;  //! creates TreeData object from session item
  create_item_t create_item;          //! creates new SessionItem from TreeData object
  update_item_t update_item;          //! updates existing SessionItem from TreeData object
  filter_item_t filter_item;          //! will filter an item if result is false
};

//! Flags to define converter behavior on the way from SessionItem to TreeData and back.

enum class ConverterMode
{
  kNone,   //!< undefined converter mode
  kClone,  //!< full deep copying with item identifiers preserved
  kCopy    //!< full deep copying with item identifiers regenerated
  //  kProject //!< selective copying for saving/loading the project (tags and data created by item,
  //           //!< updated from XML)
};

//! Returns true if given mode requires ID regeneration instead of using the one stored in XML.
inline bool IsRegenerateIdWhenBackFromXML(ConverterMode mode)
{
  // Item copy should have own unique identifier, so regeneration is required
  return mode == ConverterMode::kCopy;
}

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_CONVERTER_TYPES_H_
