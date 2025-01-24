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

#ifndef MVVM_SERIALIZATION_TREE_DATA_FWD_H_
#define MVVM_SERIALIZATION_TREE_DATA_FWD_H_

//! @file
//! Forward declaration for TreeData object.

namespace mvvm
{

namespace xml
{
class TreeData;
}

using tree_data_t = xml::TreeData;

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_FWD_H_
