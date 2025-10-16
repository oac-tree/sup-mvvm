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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/bin_utils.h"

#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>
#include <testutils/cmake_info.h>

namespace mvvm::test
{

/**
 * @brief Tests for utility functions from bin_utils.h
 */
class BinUtilsTest : public ::testing::TestWithParam<std::pair<std::string, bool>>
{
public:
  /**
   * @brief Returns full path to a test file.
   */
  static std::string GetTestFilePath(const std::string& file_name)
  {
    return ProjectResourceDir() + "/binary_samples/" + file_name;
  }
};

TEST_P(BinUtilsTest, IsBinaryFile)
{
  const auto& [file_name, is_binary] = GetParam();
  const auto full_path = GetTestFilePath(file_name);

  EXPECT_EQ(utils::IsBinaryFile(full_path), is_binary);

  EXPECT_FALSE(utils::IsBinaryFile("non-existing-file"));
}

TEST_P(BinUtilsTest, IsTextFile)
{
  const auto& [file_name, is_binary] = GetParam();
  const auto full_path = GetTestFilePath(file_name);

  EXPECT_EQ(utils::IsTextFile(full_path), !is_binary);

  EXPECT_FALSE(utils::IsTextFile("non-existing-file"));
}

//! Name of the file from resource folder and flag if it is a binary file.
INSTANTIATE_TEST_SUITE_P(FunctionalTests, BinUtilsTest,
                         ::testing::Values(std::make_pair("compiled_cpp.out", true),
                                           std::make_pair("hello_world.cpp", false),
                                           std::make_pair("pdf_file.pdf", true),
                                           std::make_pair("png_file.png", true),
                                           std::make_pair("word_file.doc", true),
                                           std::make_pair("text_UTF-8-BOM.txt", false),
                                           std::make_pair("text_UTF-8.txt", false)));

}  // namespace mvvm::test
