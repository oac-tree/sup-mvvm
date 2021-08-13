// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedataitemconverter.h"

#include "mvvm/interfaces/itemfactoryinterface.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/converter_types.h"
#include "mvvm/serialization/treedataitemdataconverter.h"
#include "mvvm/serialization/treedatataggeditemsconverter.h"

namespace
{
const std::string kItemElementType = "Item";
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

  void populate_item(const TreeData& tree_data, SessionItem& item)
  {
    auto modelType = tree_data.GetAttribute(kModelAttributeKey);

    if (modelType != item.modelType())
      throw std::runtime_error("Item model mismatch");

    auto item_data = m_itemdata_converter->ToSessionItemData(tree_data.Children().at(0));
    auto tagged_items = m_taggedtems_converter->ToTaggedItems(tree_data.Children().at(1));

    item.setDataAndTags(std::move(item_data), std::move(tagged_items));

    // FIXME implement SessionItemData and SessionItemTags update instead of full rebuild
    //    populate_item_data(json[JsonItemFormatAssistant::itemDataKey].toArray(),
    //    *item.itemData());
    //    populate_item_tags(json[JsonItemFormatAssistant::itemTagsKey].toObject(),
    //    *item.itemTags());

    for (auto child : item.children()) child->setParent(&item);

    // FIXME restore functionality
    //    if (isRegenerateIdWhenBackFromJson(m_context.m_mode))
    //        item.setData(UniqueIdGenerator::generate(), ItemDataRole::IDENTIFIER);
  }
};

TreeDataItemConverter::TreeDataItemConverter(const ItemFactoryInterface* factory)
    : p_impl(std::make_unique<TreeDataItemConverterImpl>(this, factory))
{
}

TreeDataItemConverter::~TreeDataItemConverter() = default;

bool TreeDataItemConverter::IsSessionItemConvertible(const TreeData& tree_data) const
{
  static const std::vector<std::string> expected_attributes({kModelAttributeKey});

  const bool correct_type = tree_data.GetType() == kItemElementType;
  const bool correct_attributes = tree_data.Attributes().GetAttributeNames() == expected_attributes;
  const bool correct_children_count = tree_data.GetNumberOfChildren() == 2;

  return correct_type && correct_attributes && correct_children_count;
}

std::unique_ptr<SessionItem> TreeDataItemConverter::ToSessionItem(const TreeData& tree_data) const
{
  if (!IsSessionItemConvertible(tree_data))
    throw std::runtime_error("Error in TreeDataItemConverter: uncompatible TreeData");

  auto model_type = tree_data.GetAttribute(kModelAttributeKey);
  auto result = p_impl->m_factory->CreateItem(model_type);

  p_impl->populate_item(tree_data, *result);
  return result;
}

std::unique_ptr<TreeData> TreeDataItemConverter::ToTreeData(const SessionItem& item) const
{
  auto result = std::make_unique<TreeData>(kItemElementType);
  result->AddAttribute(kModelAttributeKey, item.modelType());
  // p_impl->populate_item relies of the order of adding
  result->AddChild(*p_impl->m_itemdata_converter->ToTreeData(*item.itemData()));
  result->AddChild(*p_impl->m_taggedtems_converter->ToTreeData(*item.itemTags()));
  return result;
}

}  // namespace ModelView
