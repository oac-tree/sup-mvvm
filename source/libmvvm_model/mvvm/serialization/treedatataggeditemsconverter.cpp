// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedatataggeditemsconverter.h"

#include "mvvm/model/sessionitemcontainer.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedataitemcontainerconverter.h"
#include "mvvm/serialization/treedatataginfoconverter.h"

#include <stdexcept>

namespace
{
const std::string kTaggedItemsElementType = "TaggedItems";
const std::string kDefaultTagKey = "defaultTag";
}  // namespace

namespace ModelView
{
struct TreeDataTaggedItemsConverter::TreeDataTaggedItemsConverterImpl
{
  ConverterCallbacks m_callbacks;

  TreeDataTaggedItemsConverterImpl(ConverterCallbacks callbacks) : m_callbacks(std::move(callbacks))
  {
  }
};

TreeDataTaggedItemsConverter::TreeDataTaggedItemsConverter(const ConverterCallbacks &callbacks)
    : p_impl(std::make_unique<TreeDataTaggedItemsConverterImpl>(callbacks))
{
}

TreeDataTaggedItemsConverter::~TreeDataTaggedItemsConverter() = default;

bool TreeDataTaggedItemsConverter::IsTaggedItemsConvertible(const TreeData &tree_data) const
{
  const std::vector<std::string> expected_attributes({kDefaultTagKey});
  const bool correct_type = tree_data.GetType() == kTaggedItemsElementType;
  const bool correct_attributes = tree_data.Attributes().GetAttributeNames() == expected_attributes;
  return correct_type && correct_attributes;
}

std::unique_ptr<TaggedItems> TreeDataTaggedItemsConverter::ToTaggedItems(
    const TreeData &tree_data) const
{
  if (!IsTaggedItemsConvertible(tree_data))
    throw std::runtime_error("Error in TreeDataTaggedItemsConverter: uncompatible TreeData");
  auto result = std::make_unique<TaggedItems>();
  result->SetDefaultTag(tree_data.GetAttribute(kDefaultTagKey));
  PopulateTaggedItems(tree_data, *result);
  return result;
}

void TreeDataTaggedItemsConverter::PopulateTaggedItems(const TreeData &tree_data,
                                                       TaggedItems &tagged_items) const
{
  for (const auto &child_data : tree_data.Children())
  {
    auto container =
        ContainerConverter::ToSessionItemContainer(child_data, p_impl->m_callbacks.m_create_item);
    tagged_items.AppendContainer(std::move(container));
  }
}

std::unique_ptr<TreeData> TreeDataTaggedItemsConverter::ToTreeData(
    const TaggedItems &tagged_items) const
{
  auto result = std::make_unique<TreeData>(kTaggedItemsElementType);
  result->AddAttribute(kDefaultTagKey, tagged_items.GetDefaultTag());
  for (auto container : tagged_items)
  {
    result->AddChild(
        *ContainerConverter::ToTreeData(*container, p_impl->m_callbacks.m_create_treedata));
  }

  return result;
}

}  // namespace ModelView
