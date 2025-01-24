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

#ifndef MVVM_SERIALIZATION_TREEDATA_VARIANT_CONVERTER_H_
#define MVVM_SERIALIZATION_TREEDATA_VARIANT_CONVERTER_H_

//! @file
//! Declares utility functions to convert between TreeData and role_data_t objects.

//! These are auxiliary functions to provide parsing of xml elements
//! - <Variant role = "0" type = "undefined"></Variant>
//! - <Variant role = "0" type = "bool">true</Variant>
//! - <Variant role = "0" type = "int">42</Variant>
//! - <Variant role = "0" type = "string">James</Variant>
//! - <Variant role = "0" type = "double">42.3</Variant>
//! - <Variant role = "0" type = "vector_double">1.0, 2.0</Variant>
//! - <Variant role = "0" type = "ComboProperty" selections="1,2">a1;a2</Variant>
//! - <Variant role = "0" type = "ExternalProperty">text;color;identifier</Variant>

#include <mvvm/core/variant.h>
#include <mvvm/model_export.h>
#include <mvvm/serialization/tree_data_fwd.h>

namespace mvvm
{

//! Returns true if given TreeData represents role_data_t object.
MVVM_MODEL_EXPORT bool IsDataRoleConvertible(const tree_data_t& tree_data);

//! Returns role_data_t object from its TreeData representation. Will throw if convertions is not
//! possible.
MVVM_MODEL_EXPORT role_data_t ToRoleData(const tree_data_t& tree_data);

//! Returns TreeData object constructed from role_data_t object.
MVVM_MODEL_EXPORT tree_data_t ToTreeData(const role_data_t& role_data);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_VARIANT_CONVERTER_H_
