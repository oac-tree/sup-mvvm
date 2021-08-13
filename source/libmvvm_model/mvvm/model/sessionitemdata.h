// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_SESSIONITEMDATA_H
#define MVVM_MODEL_SESSIONITEMDATA_H

#include "mvvm/core/variant.h"

#include <vector>

namespace ModelView
{
//! Handles pairs of <data, role> for SessionItem.
class MVVM_MODEL_EXPORT SessionItemData
{
public:
  using container_t = std::vector<datarole_t>;
  using const_iterator = container_t::const_iterator;

  std::vector<int> roles() const;

  variant_t data(int role) const;

  bool setData(const variant_t& value, int role);

  const_iterator begin() const;
  const_iterator end() const;

  bool hasData(int role) const;

private:
  void AssureCompatibility(const variant_t& variant, int role);

  container_t m_values;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_SESSIONITEMDATA_H
