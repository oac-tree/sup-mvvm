// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_TREEDATAITEMCONVERTERINTERFACE_H
#define MVVM_TREEDATAITEMCONVERTERINTERFACE_H

#include "mvvm/model_export.h"

#include <memory>

namespace ModelView
{
class TreeData;
class SessionItem;

//! Base class for all converters of SessionItem to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataItemConverterInterface
{
public:
  virtual ~TreeDataItemConverterInterface() = default;

  //! Converts item to TreeData.
  virtual std::unique_ptr<TreeData> ToTreeData(const SessionItem* item) const = 0;

  //! Creates item from TreeData.
  virtual std::unique_ptr<SessionItem> FromTreeData(const TreeData&) const = 0;
};

}  // namespace ModelView

#endif  // MVVM_TREEDATAITEMCONVERTERINTERFACE_H
