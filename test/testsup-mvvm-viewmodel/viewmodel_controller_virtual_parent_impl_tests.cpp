/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewmodel_controller_virtual_parent_impl.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/viewmodelbase/viewmodel_base.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for ViewModelControllerImpl class.

class ViewModelControllerVirtualParentImplTest : public ::testing::Test
{
public:
  template <typename ChildrenT, typename RowT>
  std::unique_ptr<ViewModelControllerVirtualParentImpl> CreateController(ViewModelBase* view_model)
  {
    auto children_strategy = std::make_unique<ChildrenT>();
    auto row_strategy = std::make_unique<RowT>();
    auto result = std::make_unique<ViewModelControllerVirtualParentImpl>(
        view_model, std::move(children_strategy), std::move(row_strategy));
    return result;
  }

  template <typename ChildrenT = AllChildrenStrategy, typename RowT = LabelDataRowStrategy>
  std::unique_ptr<ViewModelControllerVirtualParentImpl> CreateController()
  {
    return CreateController<ChildrenT, RowT>(&m_viewmodel);
  }

  ApplicationModel m_model;
  ViewModelBase m_viewmodel;
};

TEST_F(ViewModelControllerVirtualParentImplTest, CreateRowFromSingleItem)
{
  auto controller = CreateController();

  SessionItem item;
  item.SetDisplayName("abc");
  item.SetData(42);

  auto row = controller->CreateTreeOfRows(item);
  EXPECT_EQ(row.size(), 2);

  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(&item), row.at(0).get());

  auto view_item0 = row.at(0).get();
  auto view_item1 = row.at(1).get();

  EXPECT_EQ(view_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("abc"));
  EXPECT_EQ(view_item1->Data(Qt::EditRole).toInt(), 42);
}

//! Validate CreateRow() method for VectorItem.

TEST_F(ViewModelControllerVirtualParentImplTest, CreateRowFromVectorItem)
{
  auto controller = CreateController();

  VectorItem item;
  item.SetX(1.0);
  item.SetY(2.0);
  item.SetZ(3.0);

  // parent item
  auto parent_row = controller->CreateTreeOfRows(item);
  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 4);  // parent, x, y, z

  EXPECT_EQ(parent_row.size(), 2);
  EXPECT_EQ(controller->GetViewItemMap().FindView(&item), parent_row.at(0).get());
  auto view_item0 = parent_row.at(0).get();
  auto view_item1 = parent_row.at(1).get();
  EXPECT_EQ(view_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("VectorItem"));
  EXPECT_EQ(view_item1->Data(Qt::EditRole).toString().toStdString(),
            std::string("(0, 0, 0)"));  // label is broken for the moment

  EXPECT_EQ(view_item0->GetRowCount(), 3);
  EXPECT_EQ(view_item0->GetColumnCount(), 2);
  EXPECT_EQ(view_item1->GetRowCount(), 0);
  EXPECT_EQ(view_item1->GetColumnCount(), 0);

  // x item
  auto x_item0 = view_item0->GetChild(0, 0);
  auto x_item1 = view_item0->GetChild(0, 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(item.GetItem(VectorItem::kX)), x_item0);
  EXPECT_EQ(x_item0->GetParent(), view_item0);
  EXPECT_EQ(x_item1->GetParent(), view_item0);
  EXPECT_EQ(x_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("X"));
  EXPECT_EQ(x_item1->Data(Qt::EditRole).toDouble(), 1.0);
  EXPECT_EQ(x_item0->GetRowCount(), 0);
  EXPECT_EQ(x_item0->GetColumnCount(), 0);
  EXPECT_EQ(x_item1->GetRowCount(), 0);
  EXPECT_EQ(x_item1->GetColumnCount(), 0);

  // y item
  auto y_item0 = view_item0->GetChild(1, 0);
  auto y_item1 = view_item0->GetChild(1, 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(item.GetItem(VectorItem::kY)), y_item0);
  EXPECT_EQ(y_item0->GetParent(), view_item0);
  EXPECT_EQ(y_item1->GetParent(), view_item0);
  EXPECT_EQ(y_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("Y"));
  EXPECT_EQ(y_item1->Data(Qt::EditRole).toDouble(), 2.0);
  EXPECT_EQ(y_item0->GetRowCount(), 0);
  EXPECT_EQ(y_item0->GetColumnCount(), 0);
  EXPECT_EQ(y_item1->GetRowCount(), 0);
  EXPECT_EQ(y_item1->GetColumnCount(), 0);

  // z item
  auto z_item0 = view_item0->GetChild(2, 0);
  auto z_item1 = view_item0->GetChild(2, 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(item.GetItem(VectorItem::kZ)), z_item0);
  EXPECT_EQ(z_item0->GetParent(), view_item0);
  EXPECT_EQ(z_item1->GetParent(), view_item0);
  EXPECT_EQ(z_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("Z"));
  EXPECT_EQ(z_item1->Data(Qt::EditRole).toDouble(), 3.0);
  EXPECT_EQ(z_item0->GetRowCount(), 0);
  EXPECT_EQ(z_item0->GetColumnCount(), 0);
  EXPECT_EQ(z_item1->GetRowCount(), 0);
  EXPECT_EQ(z_item1->GetColumnCount(), 0);
}

//! Validate CreateRow() method for VectorItem with hidden coordinate.

TEST_F(ViewModelControllerVirtualParentImplTest, CreateRowFromVectorItemWithHiddenCoordinate)
{
  // controller that filters out hidden item thanks to PropertyItemsStrategy
  auto controller = CreateController<PropertyItemsStrategy, LabelDataRowStrategy>();

  VectorItem item;
  item.SetX(1.0);
  item.SetY(2.0);
  item.SetZ(3.0);

  item.GetItem(VectorItem::kY)->SetVisible(false);

  // parent item
  auto parent_row = controller->CreateTreeOfRows(item);
  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 3);  // parent, x, z

  EXPECT_EQ(parent_row.size(), 2);
  EXPECT_EQ(controller->GetViewItemMap().FindView(&item), parent_row.at(0).get());
  auto view_item0 = parent_row.at(0).get();
  auto view_item1 = parent_row.at(1).get();
  EXPECT_EQ(view_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("VectorItem"));
  EXPECT_EQ(view_item1->Data(Qt::EditRole).toString().toStdString(),
            std::string("(0, 0, 0)"));  // label is broken for the moment

  EXPECT_EQ(view_item0->GetRowCount(), 2);
  EXPECT_EQ(view_item0->GetColumnCount(), 2);
  EXPECT_EQ(view_item1->GetRowCount(), 0);
  EXPECT_EQ(view_item1->GetColumnCount(), 0);

  // x item
  auto x_item0 = view_item0->GetChild(0, 0);
  auto x_item1 = view_item0->GetChild(0, 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(item.GetItem(VectorItem::kX)), x_item0);
  EXPECT_EQ(x_item0->GetParent(), view_item0);
  EXPECT_EQ(x_item1->GetParent(), view_item0);
  EXPECT_EQ(x_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("X"));
  EXPECT_EQ(x_item1->Data(Qt::EditRole).toDouble(), 1.0);
  EXPECT_EQ(x_item0->GetRowCount(), 0);
  EXPECT_EQ(x_item0->GetColumnCount(), 0);
  EXPECT_EQ(x_item1->GetRowCount(), 0);
  EXPECT_EQ(x_item1->GetColumnCount(), 0);

  // z item
  auto z_item0 = view_item0->GetChild(1, 0);
  auto z_item1 = view_item0->GetChild(1, 1);
  EXPECT_EQ(controller->GetViewItemMap().FindView(item.GetItem(VectorItem::kZ)), z_item0);
  EXPECT_EQ(z_item0->GetParent(), view_item0);
  EXPECT_EQ(z_item1->GetParent(), view_item0);
  EXPECT_EQ(z_item0->Data(Qt::DisplayRole).toString().toStdString(), std::string("Z"));
  EXPECT_EQ(z_item1->Data(Qt::EditRole).toDouble(), 3.0);
  EXPECT_EQ(z_item0->GetRowCount(), 0);
  EXPECT_EQ(z_item0->GetColumnCount(), 0);
  EXPECT_EQ(z_item1->GetRowCount(), 0);
  EXPECT_EQ(z_item1->GetColumnCount(), 0);
}

//! Validate CreateRow() method for compound item with child and grandchild.

TEST_F(ViewModelControllerVirtualParentImplTest, CreateRowFromCompoundItem)
{
  CompoundItem parent;
  parent.SetDisplayName("parent");
  parent.AddProperty("width", 42);
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<CompoundItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child_ptr = child.get();
  child->SetDisplayName("child");
  parent.InsertItem(std::move(child), TagIndex::Append());

  auto grandchild = std::make_unique<CompoundItem>();
  auto grandchild_ptr = grandchild.get();
  grandchild->SetDisplayName("grandchild");
  grandchild->AddProperty("length", 44);

  child_ptr->InsertItem(std::move(grandchild), TagIndex::Append());
  child_ptr->AddProperty("height", 43);

  // - parent
  //   - width
  //   - child
  //     - grandchild
  //       - length
  //     - height

  auto controller = CreateController();
  auto parent_row = controller->CreateTreeOfRows(parent);

  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 6);
  EXPECT_EQ(controller->GetViewItemMap().FindView(&parent), parent_row.at(0).get());

  auto parent_view0 = parent_row.at(0).get();
  auto parent_view1 = parent_row.at(1).get();

  EXPECT_EQ(parent_view0->Data(Qt::DisplayRole).toString(), QString("parent"));
  EXPECT_EQ(parent_view1->Data(Qt::EditRole), QVariant());

  auto width_view0 = parent_view0->GetChild(0, 0);
  auto width_view1 = parent_view0->GetChild(0, 1);

  EXPECT_EQ(width_view0->Data(Qt::DisplayRole).toString(), QString("width"));
  EXPECT_EQ(width_view1->Data(Qt::EditRole).toInt(), 42);

  auto child_view0 = parent_view0->GetChild(1, 0);
  auto child_view1 = parent_view0->GetChild(1, 1);
  EXPECT_EQ(child_view0->Data(Qt::DisplayRole).toString(), QString("child"));
  EXPECT_EQ(child_view1->Data(Qt::EditRole), QVariant());

  auto grandchild_view0 = child_view0->GetChild(0, 0);
  auto grandchild_view1 = child_view0->GetChild(0, 1);
  EXPECT_EQ(grandchild_view0->Data(Qt::DisplayRole).toString(), QString("grandchild"));
  EXPECT_EQ(grandchild_view1->Data(Qt::EditRole), QVariant());

  auto height_view0 = child_view0->GetChild(1, 0);
  auto height_view1 = child_view0->GetChild(1, 1);
  EXPECT_EQ(height_view0->Data(Qt::DisplayRole).toString(), QString("height"));
  EXPECT_EQ(height_view1->Data(Qt::EditRole).toInt(), 43);

  auto length_view0 = grandchild_view0->GetChild(0, 0);
  auto length_view1 = grandchild_view0->GetChild(0, 1);
  EXPECT_EQ(length_view0->Data(Qt::DisplayRole).toString(), QString("length"));
  EXPECT_EQ(length_view1->Data(Qt::EditRole).toInt(), 44);
}

TEST_F(ViewModelControllerVirtualParentImplTest, SetItem)
{
  auto controller = CreateController();

  VectorItem item;
  item.SetX(1.0);
  item.SetY(2.0);
  item.SetZ(3.0);

  controller->SetRootItem(&item);

  EXPECT_EQ(m_viewmodel.columnCount(), 2);
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 4);
  EXPECT_EQ(controller->GetRootItem(), &item);

  controller->SetRootItem(nullptr);
  EXPECT_EQ(m_viewmodel.columnCount(), 0);
  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(controller->GetViewItemMap().GetSize(), 0);
  EXPECT_EQ(controller->GetRootItem(), nullptr);
}
