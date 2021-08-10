// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_TREEDATAVARIANTCONVERTER_H
#define MVVM_TREEDATAVARIANTCONVERTER_H

//! @file treedatavariantconverter.h
//! Declares utility functions to convert between TreeData and datarole_t objects.

//! These are auxiliary functions to provide parsing of xml elements
//! - <Variant role = "0" type = "undefined"></Variant>
//! - <Variant role = "0" type = "bool">bool</Variant>
//! - <Variant role = "0" type = "int">42</Variant>
//! - <Variant role = "0" type = "string">James</Variant>
//! - <Variant role = "0" type = "double">42.3</Variant>
//! - <Variant role = "0" type = "vector_double_t">1.0, 2.0</Variant>

#include "mvvm/core/variant.h"
#include "mvvm/model_export.h"

namespace ModelView
{
class TreeData;

//! Returns true if given TreeData represents datarole_t object.
MVVM_MODEL_EXPORT bool IsDataRoleConvertible(const TreeData& tree_data);

//! Returns datarole_t object from its TreeData representation. Will throw if convertions is not
//! possible.
MVVM_MODEL_EXPORT datarole_t GetDataRole(const TreeData& tree_data);

//! Returns TreeData object constructed from datarole_t object.
MVVM_MODEL_EXPORT TreeData GetTreeData(const datarole_t& data_role);

}  // namespace ModelView

#endif  // MVVM_TREEDATAVARIANTCONVERTER_H
