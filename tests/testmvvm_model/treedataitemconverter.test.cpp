// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedataitemconverter.h"

#include "test_utils.h"

#include "mvvm/factories/itemcataloguefactory.h"
#include "mvvm/model/itemfactory.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/serialization/TreeData.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing TreeDataItemConverter.

class TreeDataItemConverterTest : public ::testing::Test
{
public:
  TreeDataItemConverterTest() : m_factory(ModelView::CreateStandardItemCatalogue()){};

  std::unique_ptr<TreeDataItemConverter> CreateConverter() const
  {
    return std::make_unique<TreeDataItemConverter>(&m_factory);
  }

private:
  ModelView::ItemFactory m_factory;
};

//! Parsing XML data string representing empty document.

TEST_F(TreeDataItemConverterTest, PropertyItemToTreeData)
{
  auto converter = CreateConverter();

  PropertyItem item;

  auto tree_data = converter->ToTreeData(&item);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));
}
