// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedataitemconverter.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/converter_types.h"
#include "mvvm/serialization/treedataitemdataconverter.h"
#include "mvvm/serialization/treedatataggeditemsconverter.h"

namespace
{
const std::string kTaggedItemsElementType = "Item";
const std::string kModelAttributeKey = "model";

}  // namespace

namespace ModelView
{
struct TreeDataItemConverter::TreeDataItemConverterImpl
{
  TreeDataItemConverter* m_self{nullptr};
  const ItemFactoryInterface* m_factory{nullptr};
  std::unique_ptr<TreeDataItemDataConverter> m_itemdata_converter;
  std::unique_ptr<TreeDataTaggedItemsConverter> m_taggedtems_converter;

  TreeDataItemConverterImpl(TreeDataItemConverter* self, const ItemFactoryInterface* factory)
      : m_self(self), m_factory(factory)
  {
    //! Callback to convert SessionItem to JSON object.
    auto create_tree = [this](const SessionItem& item) { return m_self->ToTreeData(item); };

    //! Callback to create SessionItem from JSON object.
    auto create_item = [this](const TreeData& tree_data)
    { return m_self->ToSessionItem(tree_data); };

    //! Callback to update SessionItem from JSON object.
    auto update_item = [this](const TreeData& tree_data, SessionItem& item)
    { populate_item(tree_data, item); };

    ConverterCallbacks callbacks{create_tree, create_item, update_item};

    m_itemdata_converter = std::make_unique<TreeDataItemDataConverter>();
    m_taggedtems_converter = std::make_unique<TreeDataTaggedItemsConverter>(callbacks);
  };

  void populate_item(const TreeData& json, SessionItem& item) {}
};

TreeDataItemConverter::TreeDataItemConverter(const ItemFactoryInterface* factory)
    : p_impl(std::make_unique<TreeDataItemConverterImpl>(this, factory))
{
}

TreeDataItemConverter::~TreeDataItemConverter() = default;

bool TreeDataItemConverter::IsSessionItemConvertible(const TreeData& tree_data)
{
  static const std::vector<std::string> expected_attributes({kModelAttributeKey});

  const bool correct_type = tree_data.GetType() == kTaggedItemsElementType;
  const bool correct_attributes = tree_data.Attributes().GetAttributeNames() == expected_attributes;
  const bool correct_children_count = tree_data.GetNumberOfChildren() == 2;

  return correct_type && correct_attributes && correct_children_count;
}

std::unique_ptr<SessionItem> TreeDataItemConverter::ToSessionItem(const TreeData&) const
{
  return {};
}

std::unique_ptr<TreeData> TreeDataItemConverter::ToTreeData(const SessionItem& item) const
{
  auto result = std::make_unique<TreeData>(kTaggedItemsElementType);
  result->AddAttribute(kModelAttributeKey, item.modelType());
  result->AddChild(*p_impl->m_itemdata_converter->ToTreeData(*item.itemData()));
  result->AddChild(*p_impl->m_taggedtems_converter->ToTreeData(*item.itemTags()));
  return result;
}

}  // namespace ModelView
