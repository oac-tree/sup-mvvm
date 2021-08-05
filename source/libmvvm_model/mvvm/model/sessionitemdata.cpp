// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/model/sessionitemdata.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace ModelView;

//! Returns vector of all roles for which data exist.
std::vector<int> SessionItemData::roles() const
{
  std::vector<int> result;
  std::transform(m_values.begin(), m_values.end(), std::back_inserter(result),
                 [](auto x) { return x.second; });
  return result;
}

//! Returns data for a given role, if exist. Will return non-initialized variant otherwise.
variant_t SessionItemData::data(int role) const
{
  for (const auto& value : m_values)
  {
    if (value.second == role)
      return value.first;
  }
  return variant_t();
}

//! Sets the data for given role. Returns true if data was changed.
//! If new value is invalid, old value with this role will be removed. If new variant is
//! incompatible with existing variant, exception will be thrown. This means that it is not possible
//! to change type of variant, once the role was set.

bool SessionItemData::setData(const variant_t& value, int role)
{
  AssureCompatibility(value, role);

  for (auto it = m_values.begin(); it != m_values.end(); ++it)
  {
    if (it->second == role)
    {
      if (Utils::IsValid(value))
      {
        if (it->first == value)
          return false;
        it->first = value;
      }
      else
      {
        m_values.erase(it);
      }
      return true;
    }
  }
  m_values.push_back(datarole_t(value, role));
  return true;
}

SessionItemData::const_iterator SessionItemData::begin() const
{
  return m_values.begin();
}

SessionItemData::const_iterator SessionItemData::end() const
{
  return m_values.end();
}

//! Returns true if item has data with given role.

bool SessionItemData::hasData(int role) const
{
  auto has_role = [role](const auto& x) { return x.second == role; };
  return std::find_if(m_values.begin(), m_values.end(), has_role) != m_values.end();
}

//! Makes sure that the new variant is compatible with the old variant stored with the given role.
//! Throws exception otherwise.

void SessionItemData::AssureCompatibility(const variant_t& variant, int role)
{
  if (!Utils::AreCompatible(data(role), variant))
  {
    std::ostringstream ostr;
    ostr << "Error in SessionItemData: variant types mismatch. "
         << "Old variant index '" << data(role).index() << "' "
         << "new variant index '" << variant.index() << "\n";
    throw std::runtime_error(ostr.str());
  }
}
