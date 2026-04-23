/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <gtest/gtest.h>

#include <memory>

namespace mvvm::test
{

namespace
{

class ICloneable
{
public:
  virtual ~ICloneable() = default;

  std::unique_ptr<ICloneable> Clone() const
  {
    return std::unique_ptr<ICloneable>(this->CloneImpl());
  }

private:
  virtual ICloneable* CloneImpl() const = 0;
};

template <typename DerivedT, typename BaseT>
class Cloneable : public BaseT
{
public:
  std::unique_ptr<DerivedT> Clone() const
  {
    return std::unique_ptr<DerivedT>(static_cast<DerivedT*>(this->CloneImpl()));
  }

private:
  // for bugprone-crtp-constructor-accessibility
  Cloneable() = default;
  friend DerivedT;
  Cloneable* CloneImpl() const override
  {
    return new DerivedT(*static_cast<const DerivedT*>(this));
  }
};

class MyItem : public Cloneable<MyItem, ICloneable>
{
public:
  MyItem() = default;
};

class MyDerivedItem : public Cloneable<MyDerivedItem, MyItem>
{
public:
  MyDerivedItem() = default;
};

}  // namespace

class ExperimentalCloneV2Test : public ::testing::Test
{
};

TEST_F(ExperimentalCloneV2Test, InitialState)
{
  const MyItem item;
  auto item_clone = item.Clone();
  EXPECT_NE(item_clone.get(), &item);
  EXPECT_TRUE(dynamic_cast<MyItem*>(item_clone.get()) != nullptr);
  EXPECT_TRUE(dynamic_cast<MyDerivedItem*>(item_clone.get()) == nullptr);
  const MyDerivedItem derived_item;
  auto derived_item_clone = derived_item.Clone();
  EXPECT_NE(derived_item_clone.get(), &derived_item);
  EXPECT_TRUE(dynamic_cast<MyDerivedItem*>(derived_item_clone.get()) != nullptr);
  EXPECT_TRUE(dynamic_cast<MyItem*>(derived_item_clone.get()) != nullptr);
}

}  // namespace mvvm::test
