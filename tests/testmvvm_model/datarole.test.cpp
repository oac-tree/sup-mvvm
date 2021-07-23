// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/datarole.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests of container utils.

class DataRoleTest : public ::testing::Test
{
};

TEST_F(DataRoleTest, Comparison)
{
  DataRole data_role1{42, 0};
  DataRole data_role2{42, 0};
  EXPECT_TRUE(data_role1 == data_role2);

  DataRole data_role3{std::vector<double>{1, 2, 3}, 42};
  DataRole data_role4{std::vector<double>{1, 2, 3}, 42};
  EXPECT_TRUE(data_role3 == data_role4);

  DataRole data_role5{std::vector<double>{1, 2}, 42};
  DataRole data_role6{std::vector<double>{1, 2, 3}, 42};
  EXPECT_FALSE(data_role5 == data_role6);
}
