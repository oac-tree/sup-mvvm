// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_TREEDATAITEMCONTAINERCONVERTER_H
#define MVVM_TREEDATAITEMCONTAINERCONVERTER_H

//! @file treedataitemcontainerconverter.h
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

#include "mvvm/model_export.h"
#include "mvvm/serialization/converter_types.h"

#include <memory>

namespace ModelView
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

}  // namespace ModelView

#endif  // MVVM_TREEDATAITEMCONTAINERCONVERTER_H
