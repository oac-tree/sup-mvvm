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

#ifndef MVVM_SERIALIZATION_TREEDATA_ITEM_CONTAINER_CONVERTER_H_
#define MVVM_SERIALIZATION_TREEDATA_ITEM_CONTAINER_CONVERTER_H_

//! @file
//! Declares utility functions to convert between TreeData and SessionItemContainer objects.

//! These are auxiliary functions to provide parsing of xml elements
//! <ItemContainer>
//!   <TagInfo min="-1" max="0" name="defaultTag">models, models, models</TagInfo>
//!   <Item model="Property">
//!     <!--Here goes item content-->
//!   </Item>
//!   <Item model="Property">
//!     <!--Here goes item content-->
//!   </Item>
//! </ItemContainer>

#include <mvvm/model_export.h>
#include <mvvm/serialization/converter_types.h>

#include <memory>

namespace mvvm
{
class TreeData;
class SessionItemContainer;

namespace ContainerConverter
{
//! Returns true if given TreeData represents SessionItemContainer object.
MVVM_MODEL_EXPORT bool IsItemContainerConvertible(const TreeData& tree_data);

//! Returns SessionItemContainer object from its TreeData representation.
MVVM_MODEL_EXPORT std::unique_ptr<SessionItemContainer> ToSessionItemContainer(
    const TreeData& tree_data, const create_item_t& func);

//! Populate SessionItemContainer object from its TreeData representation.
MVVM_MODEL_EXPORT void PopulateSessionItemContainer(const TreeData& tree_data,
                                                    SessionItemContainer& container,
                                                    const create_item_t& func);

//! Returns TreeData object SessionItemContainer from SessionItemContainer object.
MVVM_MODEL_EXPORT std::unique_ptr<TreeData> ToTreeData(const SessionItemContainer& container,
                                                       const create_treedata_t& func);
}  // namespace ContainerConverter

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_ITEM_CONTAINER_CONVERTER_H_
