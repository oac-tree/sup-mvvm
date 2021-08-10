// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedataitemconverter.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedatavariantconverter.h"

namespace
{
const std::string kElementType = "Item";
const std::string kItemDataElementType = "ItemData";
const std::string kModelAttributeKey = "model";

//! Returns vector of attributes which TreeData object should have.
std::vector<std::string> GetExpectedAttributeKeys()
{
  return std::vector<std::string>({kModelAttributeKey});
}

}  // namespace

namespace ModelView
{
struct TreeDataItemConverter::TreeDataItemConverterImpl
{
  const ItemFactoryInterface *m_factory{nullptr};

  TreeDataItemConverterImpl(const ItemFactoryInterface *factory) : m_factory(factory){};

  TreeData CreateTreeData(const SessionItemData& item_data)
  {
    TreeData result(kItemDataElementType);
    for (const auto& x : item_data) {
      result.AddChild(GetTreeData(x));
    }
    return result;
  }
};

TreeDataItemConverter::TreeDataItemConverter(const ItemFactoryInterface *factory)
    : p_impl(std::make_unique<TreeDataItemConverterImpl>(factory))
{
}

TreeDataItemConverter::~TreeDataItemConverter() = default;

std::unique_ptr<TreeData> TreeDataItemConverter::ToTreeData(const SessionItem *item) const
{
  auto result = std::make_unique<TreeData>(kElementType);
  result->AddAttribute(kModelAttributeKey, item->modelType());
  result->AddChild(TreeData("TaggedItems"));
  result->AddChild(p_impl->CreateTreeData(*item->itemData()));
  return result;
}

std::unique_ptr<SessionItem> TreeDataItemConverter::FromTreeData(const TreeData &) const
{
  return {};
}

bool TreeDataItemConverter::IsSessionItemConvertible(const TreeData &tree_data)
{
  bool correct_type = tree_data.GetType() == kElementType;
  bool correct_attributes =
      tree_data.Attributes().GetAttributeNames() == GetExpectedAttributeKeys();
  bool correct_children_count = tree_data.GetNumberOfChildren() == 2;
  return correct_type && correct_attributes && correct_children_count;
}

}  // namespace ModelView
