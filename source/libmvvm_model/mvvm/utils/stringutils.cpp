/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/stringutils.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>

namespace ModelView::Utils
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
    inter << ".0";
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
    return inter.str();

  std::string part1 = inter.str().substr(0, pos);
  std::string part2 = inter.str().substr(pos, std::string::npos);

  part1.erase(part1.find_last_not_of('0') + 1, std::string::npos);
  if (part1.back() == '.')
    part1 += "0";

  return part1 + part2;
}

std::string TrimWhitespace(const std::string& str)
{
  const char* whitespace{" \t\n"};
  const size_t first = str.find_first_not_of(whitespace);
  if (std::string::npos == first)
    return {};
  const size_t last = str.find_last_not_of(whitespace);
  return str.substr(first, (last - first + 1));
}

bool StringToBool(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool value{false};
  if (!(is >> std::boolalpha >> value))
    throw std::runtime_error("Can't parse string to bool");
  return value;
}

std::string FromBool(bool value)
{
  std::ostringstream os;
  os << std::boolalpha << value;
  return os.str();
}

std::optional<double> StringToDouble(const std::string& str)
{
  std::istringstream iss(Utils::TrimWhitespace(str));
  iss.imbue(std::locale::classic());
  double value;
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<double>(value) : std::optional<double>{};
}

std::optional<int> StringToInteger(const std::string& str)
{
  std::istringstream iss(Utils::TrimWhitespace(str));
  int value;
  iss >> value;
  return (!iss.fail() && iss.eof()) ? std::optional<int>(value) : std::optional<int>{};
}

std::vector<std::string> SplitString(const std::string& str, const std::string& delimeter)
{
  // splitting string following Python's str.split()
  if (delimeter.empty())
    throw std::runtime_error("Empty delimeter");
  if (str.empty())
    return {};

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
      throw std::runtime_error("Error while parsing string of comma separated doubles");
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

}  // namespace ModelView::Utils
