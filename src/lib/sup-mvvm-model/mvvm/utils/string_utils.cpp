/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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
 *****************************************************************************/

#include "string_utils.h"

#include <mvvm/core/exceptions.h>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>

namespace mvvm::utils
{
std::string DoubleToString(double input, int precision)
{
  std::ostringstream inter;
  inter << std::setprecision(precision);
  if (std::abs(input) < std::numeric_limits<double>::epsilon())
  {
    inter << "0.0";
    return inter.str();
  }
  inter << input;
  if (inter.str().find('e') == std::string::npos && inter.str().find('.') == std::string::npos)
  {
    inter << ".0";
  }
  return inter.str();
}

std::string ScientificDoubleToString(double input, int precision)
{
  std::ostringstream inter;
  inter << std::scientific;
  inter << std::setprecision(precision);
  inter << input;

  std::string::size_type pos = inter.str().find('e');
  if (pos == std::string::npos)
  {
    return inter.str();
  }

  std::string part1 = inter.str().substr(0, pos);
  std::string part2 = inter.str().substr(pos, std::string::npos);

  part1.erase(part1.find_last_not_of('0') + 1, std::string::npos);
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
  auto value = TrimWhitespace(str);
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);

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
  iss.imbue(std::locale::classic());
  double value{0.0};
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<double>(value) : std::optional<double>{};
}

std::optional<int> StringToInteger(const std::string& str)
{
  std::istringstream iss(utils::TrimWhitespace(str));
  int value{0};
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<int>(value) : std::optional<int>{};
}

std::vector<std::string> SplitString(const std::string& str, const std::string& delimeter)
{
  // splitting string following Python's str.split()
  if (delimeter.empty())
  {
    throw RuntimeException("Empty delimeter");
  }
  if (str.empty())
  {
    return {};
  }

  std::vector<std::string> result;
  std::string_view view(str);
  size_t pos{0};

  while ((pos = view.find(delimeter)) != std::string::npos)
  {
    result.emplace_back(std::string(view.substr(0, pos)));
    view.remove_prefix(pos + delimeter.length());
  }
  result.emplace_back(std::string(view));
  return result;
}

std::vector<double> ParseSpaceSeparatedDoubles(const std::string& str)
{
  std::vector<double> result;
  ParseSpaceSeparatedDoubles(str, result);
  return result;
}

void ParseSpaceSeparatedDoubles(const std::string& str, std::vector<double>& result)
{
  std::istringstream iss(str);
  iss.imbue(std::locale::classic());
  std::copy(std::istream_iterator<double>(iss), std::istream_iterator<double>(),
            back_inserter(result));
}

std::vector<double> ParseCommaSeparatedDoubles(const std::string& str)
{
  auto trimmed = TrimWhitespace(str);
  std::vector<double> result;
  for (const auto& token : SplitString(trimmed, ","))
  {
    auto converted = StringToDouble(token);
    if (!converted.has_value())
    {
      throw RuntimeException("Error while parsing string of comma separated doubles");
    }
    result.push_back(converted.value());
  }
  return result;
}

std::string ToCommaSeparatedString(const std::vector<double>& vec)
{
  auto comma_fold = [](std::string a, double value)
  { return std::move(a) + ", " + DoubleToString(value); };
  return vec.empty() ? std::string()
                     : std::accumulate(std::next(vec.begin()), vec.end(), DoubleToString(vec[0]),
                                       comma_fold);
}

std::string ToCommaSeparatedString(const std::vector<std::string>& vec)
{
  auto comma_fold = [](std::string a, const std::string& value)
  { return std::move(a) + ", " + value; };
  return vec.empty() ? std::string()
                     : std::accumulate(std::next(vec.begin()), vec.end(), vec[0], comma_fold);
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
    result.replace(pos, substr.length(), replacement);
    pos = result.find(substr, pos + replacement.length());
  }
  return result;
}

}  // namespace mvvm::utils
