/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SERIALIZATION_XML_WRITE_UTILS_H_
#define MVVM_SERIALIZATION_XML_WRITE_UTILS_H_

//! @file xmlwriteutils.h
//! Collection of utilities to write TreeData into XML files.

#include <string>

namespace mvvm
{
class TreeData;

void WriteToXMLFile(const std::string& file_name, const TreeData& tree_data);

std::string GetXMLString(const TreeData& tree_data);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_XML_WRITE_UTILS_H_
