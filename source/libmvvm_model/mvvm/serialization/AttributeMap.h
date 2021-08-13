/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

/**
 * @file AttributeMap.h
 * @brief Header file for AttributeMap class.
 * @date 09/02/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the AttributeMap class.
 */

#ifndef _SEQ_AttributeMap_h_
#define _SEQ_AttributeMap_h_

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ModelView
{
namespace attributes
{
const std::string NAME_ATTRIBUTE = "name";
}

/**
 * @brief Class that contains attributes and exposes a map interface.
 */

class AttributeMap
{
public:
  using map_type = std::map<std::string, std::string>;

private:
  /**
   * @brief Implementation of the map of attributes.
   */
  map_type _attributes;

protected:
public:
  /**
   * @brief Constructor.
   */
  AttributeMap();

  /**
   * @brief Destructor.
   */
  ~AttributeMap();

  /**
   * @brief Copy/move Constructor.
   */
  AttributeMap(const AttributeMap& other);
  AttributeMap(AttributeMap&& other);

  /**
   * @brief Copy/move Assignment.
   */
  AttributeMap& operator=(const AttributeMap& other);
  AttributeMap& operator=(AttributeMap&& other);

  /**
   * @brief Comparison operators.
   */
  bool operator==(const AttributeMap& other) const;
  bool operator!=(const AttributeMap& other) const;

  /**
   * @brief Iterator forwarding.
   */
  using iterator = map_type::iterator;
  using const_iterator = map_type::const_iterator;
  iterator begin() { return _attributes.begin(); }
  iterator end() { return _attributes.end(); }
  const_iterator begin() const { return _attributes.begin(); }
  const_iterator end() const { return _attributes.end(); }

  /**
   * @brief Get number of attributes in map.
   *
   * @return Number of attributes in map.
   */
  size_t GetNumberOfAttributes() const;

  /**
   * @brief Indicate presence of attribute with given name.
   *
   * @param name Attribute name.
   * @return true when present.
   */
  bool HasAttribute(const std::string& name) const;

  /**
   * @brief Get attribute with given name.
   *
   * @param name Attribute name.
   * @return Attribute value.
   */
  std::string GetAttribute(const std::string& name) const;

  /**
   * @brief Get all attribute names.
   *
   * @return List of all Attribute names.
   */
  std::vector<std::string> GetAttributeNames() const;

  /**
   * @brief Add attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
   * @details This method will not try to overwrite an existing attribute. It returns
   * false if the attribute with the given name already exists.
   */
  bool AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Set attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @details This method will overwrite an existing attribute or create a new one.
   */
  void SetAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Clear all attributes.
   */
  void Clear();

  /**
   * @brief Remove attribute with given name.
   *
   * @param name Name of attribute to remove.
   * @return true when attribute was found and removed.
   */
  bool Remove(const std::string& name);

  /**
   * @brief Initialise variable attributes with values from other map.
   *
   * @param source Map containing variable name - value pairs.
   * @return true when all variable attributes were initialised.
   */
  bool InitialiseVariableAttributes(const AttributeMap& source);
};

}  // namespace ModelView

#endif  // _SEQ_AttributeMap_h_
