/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/combo_property.h"

#include <mvvm/core/exceptions.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests for ComboProperty class.
 */
class ComboPropertyTests : public ::testing::Test
{
};

TEST_F(ComboPropertyTests, InitialState)
{
  const ComboProperty combo;
  EXPECT_EQ(combo.GetValue(), "");
  EXPECT_EQ(combo.GetValues(), std::vector<std::string>());
  EXPECT_EQ(combo.GetToolTips(), std::vector<std::string>());
  EXPECT_EQ(combo.GetCurrentIndex(), -1);
  EXPECT_EQ(combo.GetStringOfValues(), "");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>());
  EXPECT_EQ(combo.GetLabel(), "None");
}

TEST_F(ComboPropertyTests, CreateFrom)
{
  // from vector of values, first item should be selected
  std::vector<std::string> expected{"a1", "a2"};
  ComboProperty combo = ComboProperty::CreateFrom(expected);
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // from vector of values, selection provided
  expected = {"b1", "b2", "b3"};
  combo = ComboProperty::CreateFrom(expected, "b2");
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "b2");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
}

TEST_F(ComboPropertyTests, SetValue)
{
  const std::vector<std::string> expected{"a1", "a2"};
  ComboProperty combo = ComboProperty::CreateFrom(expected);

  // setting second value
  combo.SetValue("a2");
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "a2");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));

  // setting non-existing value
  EXPECT_THROW(combo.SetValue("c0"), RuntimeException);
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "a2");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
}

TEST_F(ComboPropertyTests, SetCurrentIndex)
{
  const std::vector<std::string> expected{"c1", "c2", "c3"};
  ComboProperty combo = ComboProperty::CreateFrom(expected);

  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));
  EXPECT_EQ(combo.GetValues(), expected);

  combo.SetCurrentIndex(1);
  EXPECT_EQ(combo.GetValue(), std::string("c2"));
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));

  // setting unexpected index
  EXPECT_THROW(combo.SetCurrentIndex(3), RuntimeException);
  EXPECT_EQ(combo.GetValue(), std::string("c2"));
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
}

TEST_F(ComboPropertyTests, SetValues)
{
  // seting values through stream
  const std::vector<std::string> expected_values{"a1", "a2"};
  ComboProperty combo = ComboProperty::CreateFrom(expected_values);

  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetValue(), std::string("a1"));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // setting values from setter, old values have to be overriden
  std::vector<std::string> newValues{"b1", "b2", "b3"};
  combo.SetValues(newValues);
  EXPECT_EQ(combo.GetValue(), std::string("b1"));
  EXPECT_EQ(combo.GetValues(), newValues);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // setting new/old values through setter, old value should be preserved
  newValues = {"c1", "b1", "c2"};
  combo.SetValues(newValues);
  EXPECT_EQ(combo.GetValue(), std::string("b1"));
  EXPECT_EQ(combo.GetValues(), newValues);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));

  // setting empty list shouldn't change anything
  const std::vector<std::string> empty;
  combo.SetValues(empty);
  EXPECT_EQ(combo.GetValue(), std::string("b1"));
  EXPECT_EQ(combo.GetValues(), newValues);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
}

TEST_F(ComboPropertyTests, SetSelected)
{
  const std::vector<std::string> expected_values = {"a1", "a2", "a3"};
  ComboProperty combo = ComboProperty::CreateFrom(expected_values);

  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a1"}));

  // selecting already selected element, nothing should change
  combo.SetSelected(0);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a1"}));

  // deselecting index
  combo.SetSelected(0, false);
  EXPECT_EQ(combo.GetCurrentIndex(), -1);
  EXPECT_EQ(combo.GetValue(), "");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>());
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>());

  // selecting two indeces
  combo.SetSelected(1, true);
  combo.SetSelected(2, true);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "a2");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1, 2}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a2", "a3"}));

  // selecting by name
  combo.SetSelected("a2", false);
  combo.SetSelected("a1", true);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0, 2}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a1", "a3"}));

  // setting current index invalidates selection
  combo.SetCurrentIndex(1);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "a2");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a2"}));
}

TEST_F(ComboPropertyTests, SetSelectedIndices)
{
  const std::vector<std::string> expected_values = {"a1", "a2", "a3"};
  ComboProperty combo = ComboProperty::CreateFrom(expected_values);
  combo.SetSelectedIndices({0, 1});

  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetValues(), expected_values);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0, 1}));
  EXPECT_EQ(combo.GetSelectedValues(), std::vector<std::string>({"a1", "a2"}));
}

TEST_F(ComboPropertyTests, FromStream)
{
  ComboProperty combo = ComboProperty() << "a1"
                                        << "a2";
  std::vector<std::string> expected{"a1", "a2"};
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // adding more
  combo << "c0";
  expected = {"a1", "a2", "c0"};
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // setting another index, adding more, state should be preserved
  combo.SetCurrentIndex(1);
  combo.SetSelected(2, true);
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValue(), "a2");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1, 2}));
  combo << "c1";
  expected = {"a1", "a2", "c0", "c1"};
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
}

TEST_F(ComboPropertyTests, FromVectorStream)
{
  std::vector<std::string> expected{"a1", "a2"};
  ComboProperty combo = ComboProperty::CreateFrom(expected);
  combo.SetSelected(0, true);
  combo.SetSelected(1, true);

  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0, 1}));

  // adding from vector stream, old selection state should be preserved
  const std::vector<std::string> more{"c1", "c2"};
  combo << more;
  expected = {"a1", "a2", "c1", "c2"};
  EXPECT_EQ(combo.GetValues(), expected);
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValue(), "a1");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0, 1}));
}

TEST_F(ComboPropertyTests, SetSringOfValues)
{
  const std::vector<std::string> expected_values = {"a1", "a2"};
  ComboProperty combo = ComboProperty::CreateFrom(expected_values);

  EXPECT_EQ(combo.GetStringOfValues(), std::string("a1;a2"));
  EXPECT_EQ(combo.GetValue(), std::string("a1"));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // setting string of values, current value should change
  std::string string_of_values("b1;b2;b3");
  combo.SetValuesFromString(string_of_values);
  EXPECT_EQ(combo.GetStringOfValues(), string_of_values);
  EXPECT_EQ(combo.GetValue(), std::string("b1"));
  EXPECT_EQ(combo.GetValues(), std::vector<std::string>({"b1", "b2", "b3"}));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));

  // setting new string of values, containing current value. Current values should remain.
  string_of_values = std::string("c1;b1;c3");
  combo.SetValuesFromString(string_of_values);
  EXPECT_EQ(combo.GetStringOfValues(), string_of_values);
  EXPECT_EQ(combo.GetValue(), std::string("b1"));
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1}));
}

TEST_F(ComboPropertyTests, SetStringOfSelections)
{
  ComboProperty combo;
  EXPECT_EQ(combo.GetStringOfSelections(), "");

  // checking the content of stringOfSelections
  combo.SetValues(std::vector<std::string>({"a1", "a2", "a3"}));
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetStringOfSelections(), "0");

  combo.SetSelected(2, true);
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0, 2}));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetStringOfSelections(), "0,2");

  // setting string of selections
  combo.SetSelectionFromString("");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({}));
  EXPECT_EQ(combo.GetCurrentIndex(), -1);
  EXPECT_EQ(combo.GetStringOfSelections(), "");

  combo.SetSelectionFromString("1,2");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({1, 2}));
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetStringOfSelections(), "1,2");

  combo.SetSelectionFromString("0,42");
  EXPECT_EQ(combo.GetSelectedIndices(), std::vector<int>({0}));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetStringOfSelections(), "0");
}

TEST_F(ComboPropertyTests, ComboEqualityDiffIndex)
{
  ComboProperty c1 = ComboProperty::CreateFrom({"a1", "a2"});
  ComboProperty c2 = ComboProperty::CreateFrom({"a1", "a2"});

  c1.SetValue("a1");
  c2.SetValue("a2");
  EXPECT_TRUE(c1 != c2);

  c2.SetValue("a1");
  EXPECT_TRUE(c1 == c2);
}

TEST_F(ComboPropertyTests, cCmboEqualityDiffList)
{
  ComboProperty c1;
  ComboProperty c2;
  EXPECT_TRUE(c1 == c2);

  c1 << "a1"
     << "a2";
  c2 << "a1"
     << "a2";
  EXPECT_TRUE(c1 == c2);
  EXPECT_FALSE(c1 != c2);

  c2 << "a3";
  EXPECT_TRUE(c1 != c2);
  EXPECT_FALSE(c1 == c2);
  c2.SetValue("a2");
  EXPECT_TRUE(c1 != c2);
  EXPECT_FALSE(c1 == c2);

  c1 << "a3";
  c1.SetValue("a2");
  EXPECT_TRUE(c1 == c2);
  EXPECT_FALSE(c1 != c2);

  // with selection indices
  c1 = ComboProperty() << "a1"
                       << "a2"
                       << "a3";
  c2 = ComboProperty() << "a1"
                       << "a2"
                       << "a3";
  EXPECT_TRUE(c1 == c2);

  c2.SetSelected(0, false);
  c2.SetSelected(2, true);
  EXPECT_TRUE(c1 != c2);

  c1.SetSelectionFromString("2");
  c2.SetSelectionFromString("2");
  EXPECT_TRUE(c1 == c2);
}

TEST_F(ComboPropertyTests, GetTooltips)
{
  ComboProperty combo;

  const std::vector<std::string> expected_tooltips({"abc", "def"});
  combo.SetToolTips(expected_tooltips);
  EXPECT_EQ(combo.GetToolTips(), expected_tooltips);
}
