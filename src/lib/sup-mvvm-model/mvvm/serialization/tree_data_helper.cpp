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

#include "tree_data_helper.h"

#include <sup/xml/tree_data_parser.h>

namespace mvvm
{

std::vector<std::string> GetAttributeNames(const tree_data_t& tree_data)
{
  std::vector<std::string> result;
  for (const auto& [name, _] : tree_data.Attributes())
  {
    result.push_back(name);
  }
  return result;
}

std::unique_ptr<tree_data_t> ParseXMLElementString(const std::string& element)
{
  const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
)RAW"};

  std::string content = header + element;
  return mvvm::xml::TreeDataFromString(content);
}

}  // namespace mvvm
