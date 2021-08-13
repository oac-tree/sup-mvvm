// **************************************************************************  //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedataitemcontainerconverter.h"

#include "test_utils.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitemcontainer.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/TreeData.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing of TreeDataItemContainerConverter.
//! It uses simplified item constructiion.

class TreeDataItemContainerConverterTest : public ::testing::Test
{
public:
  TreeDataItemContainerConverterTest()
  {
    //! Simplified method to convert SessionItem to TreeData.
    auto to_treedata = [this](const SessionItem& item)
    {
      auto result = std::make_unique<TreeData>("Item");
      result->AddAttribute("model", item.modelType());
      return result;
    };
    m_create_tree_callback = to_treedata;

    //! Simplified method to convert TreeData to SessionItem.
    auto to_item = [this](const TreeData&) { return std::make_unique<PropertyItem>(); };
    m_create_item_callback = to_item;
  }

  create_treedata_t m_create_tree_callback;
  create_item_t m_create_item_callback;
};

//! Container with single items is converted to TreeData.

TEST_F(TreeDataItemContainerConverterTest, ContainerToTreeData)
{
  // creating container
  TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::Type);
  SessionItemContainer container(tag);

  // inserting single property item
  auto item = new PropertyItem;
  item->setData(42);
  EXPECT_TRUE(container.InsertItem(item, 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  EXPECT_TRUE(ContainerConverter::IsItemContainerConvertible(*tree_data));
}

//! Container with single items is converted to TreeData and back.

TEST_F(TreeDataItemContainerConverterTest, ContainerToTreeDataAndBack)
{
  // creating container
  TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::Type);
  SessionItemContainer container(tag);

  // inserting single property item
  EXPECT_TRUE(container.InsertItem(new PropertyItem, 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  auto container2 = ContainerConverter::ToSessionItemContainer(*tree_data, m_create_item_callback);
  EXPECT_EQ(container2->GetItemCount(), 1);
  EXPECT_EQ(container2->GetTagInfo().GetName(), std::string("thickness"));
}
