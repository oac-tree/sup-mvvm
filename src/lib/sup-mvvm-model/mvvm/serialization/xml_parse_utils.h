/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef MVVM_SERIALIZATION_XML_PARSE_UTILS_H_
#define MVVM_SERIALIZATION_XML_PARSE_UTILS_H_

#include <mvvm/serialization/treedata.h>

#include <memory>

namespace mvvm
{
std::unique_ptr<TreeData> ParseXMLDataFile(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDataString(const std::string& xml_str);

//! Parse string representing XML element. Works the same way as the method above, but doesn't
//! require standard XML declaration header.
std::unique_ptr<TreeData> ParseXMLElementString(const std::string& xml_str);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_XML_PARSE_UTILS_H_
