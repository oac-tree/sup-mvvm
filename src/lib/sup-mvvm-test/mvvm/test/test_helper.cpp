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

#include "test_helper.h"

#include <fstream>
#include <sstream>

namespace
{
const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<root>)RAW"};

const std::string footer{R"RAW(</root>
)RAW"};

}  // namespace

namespace mvvm::test
{

std::string CreateXMLDocumentString(const std::string &body)
{
  return header + body + footer;
}

std::string GetTextFileContent(const std::string &file_name)
{
  std::ifstream in(file_name);
  const std::stringstream sstr;
  while (in >> sstr.rdbuf())
  {
  }
  return sstr.str();
}

void CreateTextFile(const std::string &file_name, const std::string &content)
{
  std::ofstream file_out(file_name);
  file_out.write(content.c_str(), content.size());
}

}  // namespace mvvm::test
