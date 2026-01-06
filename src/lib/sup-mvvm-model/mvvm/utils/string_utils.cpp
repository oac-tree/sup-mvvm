/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "string_utils.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <regex>
#include <sstream>

namespace mvvm::utils
{

std::string DoubleToString(double input, std::int32_t precision)
{
  std::ostringstream inter;
  inter << std::setprecision(precision);
  if (std::abs(input) < std::numeric_limits<double>::epsilon())
  {
    inter << "0.0";
    return inter.str();
  }
  inter << input;
  if ((inter.str().find('e') == std::string::npos) && (inter.str().find('.') == std::string::npos))
  {
    inter << ".0";
  }
  return inter.str();
}

std::string ScientificDoubleToString(double input, std::int32_t precision)
{
  std::ostringstream inter;
  inter << std::scientific;
  inter << std::setprecision(precision);
  inter << input;

  const std::string::size_type pos = inter.str().find('e');
  if (pos == std::string::npos)
  {
    return inter.str();
  }

  std::string part1 = inter.str().substr(0, pos);
  const std::string part2 = inter.str().substr(pos, std::string::npos);

  (void)part1.erase(part1.find_last_not_of('0') + 1, std::string::npos);
  if (part1.back() == '.')
  {
    part1 += "0";
  }

  return part1 + part2;
}

std::string TrimWhitespace(const std::string& str)
{
  const char* whitespace{" \t\n"};
  const size_t first = str.find_first_not_of(whitespace);
  if (std::string::npos == first)
  {
    return {};
  }
  const size_t last = str.find_last_not_of(whitespace);
  return str.substr(first, (last - first + 1));
}

bool StringToBool(const std::string& str)
{
  std::locale loc("C");
  auto value = TrimWhitespace(str);
  (void)std::transform(value.begin(), value.end(), value.begin(),
                       [&loc](char c) { return std::tolower(c, loc); });

  static const std::vector<std::string> true_answers = {"true", "yes", "on"};
  static const std::vector<std::string> false_answers = {"false", "no"};

  if (std::find(true_answers.begin(), true_answers.end(), value) != true_answers.end())
  {
    return true;
  }

  if (std::find(false_answers.begin(), false_answers.end(), value) != false_answers.end())
  {
    return false;
  }

  throw RuntimeException("Can't parse string to bool");
}

std::string FromBool(bool value)
{
  std::ostringstream os;
  os << std::boolalpha << value;
  return os.str();
}

std::optional<double> StringToDouble(const std::string& str)
{
  std::istringstream iss(utils::TrimWhitespace(str));
  (void)iss.imbue(std::locale::classic());
  double value{0.0};
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<double>(value) : std::optional<double>{};
}

std::optional<std::int32_t> StringToInteger(const std::string& str)
{
  std::istringstream iss(utils::TrimWhitespace(str));
  std::int32_t value{0};
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<std::int32_t>(value)
                                    : std::optional<std::int32_t>{};
}

std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter)
{
  // splitting string following Python's str.split()
  if (delimiter.empty())
  {
    throw RuntimeException("Empty delimiter");
  }
  if (str.empty())
  {
    return {};
  }

  std::vector<std::string> result;
  std::string_view view(str);
  size_t pos{0};

  while ((pos = view.find(delimiter)) != std::string::npos)
  {
    (void)result.emplace_back(view.substr(0, pos));
    view.remove_prefix(pos + delimiter.length());
  }
  (void)result.emplace_back(view);
  return result;
}

std::vector<double> ParseSpaceSeparatedDoubles(const std::string& str)
{
  std::vector<double> result;
  std::istringstream iss(str);
  (void)iss.imbue(std::locale::classic());
  (void)std::copy(std::istream_iterator<double>(iss), std::istream_iterator<double>(),
                  back_inserter(result));
  return result;
}

std::string ReplaceSubString(const std::string& str, const std::string& substr,
                             const std::string& replacement)
{
  if (str.empty() || substr.empty())
  {
    return str;
  }

  std::string result(str);
  auto pos = result.find(substr);
  while (pos != std::string::npos)
  {
    (void)result.replace(pos, substr.length(), replacement);
    pos = result.find(substr, pos + replacement.length());
  }
  return result;
}

std::optional<std::string> GetKeyContent(const std::string& text, const std::string& key_name)
{
  const std::regex pattern(key_name + "\\[(.*?)\\]");
  std::smatch matches;

  if (std::regex_search(text, matches, pattern))
  {
    if (matches.size() > 1)
    {
      return matches[1].str();
    }
  }
  return {};
}

}  // namespace mvvm::utils
