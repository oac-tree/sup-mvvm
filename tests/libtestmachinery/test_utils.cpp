// ************************************************************************** //
//
//  Sequencer GUI prototype
//
// ************************************************************************** //

#include "test_utils.h"

#include <string>

namespace
{
const std::string header{R"RAW(<?xml version = "1.0" encoding = "UTF-8"?>
<root>)RAW"};

const std::string footer{R"RAW(</root>
)RAW"};

}  // namespace

namespace TestUtils
{
std::string CreateXMLDocumentString(const std::string& body)
{
  return header + body + footer;
}
}  // namespace TestUtils
