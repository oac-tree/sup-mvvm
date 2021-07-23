// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_DATAROLE_H
#define MVVM_MODEL_DATAROLE_H

#include "mvvm/core/variant.h"

namespace ModelView
{
//! Represents pair of data,role for SessionItemData.

struct DataRole
{
  DataRole(variant_t data, int role);
  variant_t m_data;
  int m_role;
  bool operator==(const DataRole& other) const;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_DATAROLE_H
