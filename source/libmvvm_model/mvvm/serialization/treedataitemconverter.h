// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_TREEDATAITEMCONVERTER_H
#define MVVM_TREEDATAITEMCONVERTER_H

#include "mvvm/serialization/treedataitemconverterinterface.h"

namespace ModelView
{
class ItemFactoryInterface;

//! Converters SessionItem to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataItemConverter : public TreeDataItemConverterInterface
{
public:
  TreeDataItemConverter(const ItemFactoryInterface* factory);
  ~TreeDataItemConverter();

  //! Returns true if given TreeData represents SessionItem object.
  bool IsSessionItemConvertible(const TreeData& tree_data) const;

  //! Creates SessionItem from TreeData.
  std::unique_ptr<SessionItem> ToSessionItem(const TreeData&) const override;

  //! Creates TreeData from SessionItem.
  std::unique_ptr<TreeData> ToTreeData(const SessionItem& item) const override;

private:
  struct TreeDataItemConverterImpl;
  std::unique_ptr<TreeDataItemConverterImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_TREEDATAITEMCONVERTER_H
