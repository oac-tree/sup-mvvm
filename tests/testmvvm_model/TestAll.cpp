#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  // run all google tests
  return RUN_ALL_TESTS();
}
