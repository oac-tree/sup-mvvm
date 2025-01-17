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

#ifndef MVVM_MODEL_TAGINFO_H_
#define MVVM_MODEL_TAGINFO_H_

#include <mvvm/model_export.h>

#include <string>
#include <vector>

namespace mvvm
{

/**
 * @brief The TagInfo class holds info about the single tag for SessionItem.
 *
 * The tag specifies information about children that can be added to a SessionItem. A tag has a
 * name, min, max allowed children count, and vector of all types children can have.
 *
 * If the provided list of allowed item types is empty, any child will be considered valid for this
 * tag. For a non-empty list, only children with the type present in the list will be considered as
 * valid.
 */
class MVVM_MODEL_EXPORT TagInfo
{
public:
  /**
   * @brief Default c-tor.
   */
  TagInfo();

  /**
   * @brief Full c-tor.
   *
   * @param name The name of the tag.
   * @param min Minimum allowed number of children.
   * @param max Maximum allowed number of childre.
   * @param item_types
   */
  TagInfo(std::string name, int min, int max, std::vector<std::string> item_types);

  /**
   * @brief A factory method to constructs universal tag intended for unlimited amount of various
   * items.
   *
   * @param name The name of the tag.
   * @param item_types The list of allowed item types.
   * @return Constructed tag.
   */
  static TagInfo CreateUniversalTag(std::string name, std::vector<std::string> item_types = {});

  /**
   * @brief A factory method to constructs property tag intended to store property item.
   *
   * A property item is an item which is normally hidden in top-level item hierarchies, but shown in
   * property editors. Once inserted, it can't be removed.
   *
   * @param name The name of the tag.
   * @param item_types The list of allowed item types.
   * @return Constructed tag.
   */
  static TagInfo CreatePropertyTag(std::string name, std::string item_type);

  /**
   * @brief Returns the name of this class.
   */
  std::string GetName() const;

  /**
   * @brief Returns minimum allowed amount of items with this tag.
   */
  int GetMin() const;

  /**
   * @brief Returns maximum allowed amount of items with this tag.
   */
  int GetMax() const;

  /**
   * @brief Returns vector of allowed item types which this tag accepts.
   */
  std::vector<std::string> GetItemTypes() const;

  /**
   * @brief Checks if given item's type matches the list of possible item types.
   */
  bool IsValidType(const std::string& item_type) const;

  bool operator==(const TagInfo& other) const;
  bool operator!=(const TagInfo& other) const;

private:
  std::string m_name;                     //!< the name of the tag
  int m_min;                              //!< minimum allowed number of items in a tag
  int m_max;                              //!< maximum allowed number of items in a tag
  std::vector<std::string> m_item_types;  //!< vector of allowed item types
};

}  // namespace mvvm

#endif  // MVVM_MODEL_TAGINFO_H_
