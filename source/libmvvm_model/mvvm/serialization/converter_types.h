/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SERIALIZATION_CONVERTER_TYPES_H
#define MVVM_SERIALIZATION_CONVERTER_TYPES_H

//! @file converter_types.h
//! Collection of custom types involved into SessionItem and TreeData mutual convertion.

#include "mvvm/model_export.h"

#include <functional>
#include <memory>

namespace mvvm
{
class TreeData;
class SessionItem;

//! Defines function to create TreeData object from SessionItem.
using create_treedata_t = std::function<std::unique_ptr<TreeData>(const SessionItem&)>;

//! Defines function to create SessionItem object from TreeData.
using create_item_t = std::function<std::unique_ptr<SessionItem>(const TreeData&)>;

//! Defines function to populate existing SessionItem from its' TreeData representaion.
using update_item_t = std::function<void(const TreeData&, SessionItem&)>;

//! Provides necessary callbacks to convert SessionItem to TreeData and back.

struct MVVM_MODEL_EXPORT ConverterCallbacks
{
  create_treedata_t create_treedata;  //! creates TreeData object from session item
  create_item_t create_item;          //! creates new SessionItem from TreeData object
  update_item_t update_item;          //! updates existing SessionItem from TreeData object
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

#endif  // MVVM_SERIALIZATION_CONVERTER_TYPES_H
