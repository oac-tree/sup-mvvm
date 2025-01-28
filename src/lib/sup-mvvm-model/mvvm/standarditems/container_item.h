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

#ifndef MVVM_STANDARDITEMS_CONTAINER_ITEM_H_
#define MVVM_STANDARDITEMS_CONTAINER_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

/**
 * @brief The ContainerItem class is a simple container to store any type of child.
 *
 * May be used as a convenience item to create a branch with uniform children beneath, for example.
 */
class MVVM_MODEL_EXPORT ContainerItem : public CompoundItem
{
public:
  explicit ContainerItem(const std::string& model_type = GetStaticType());

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Checks if container is empty.
   */
  bool IsEmpty() const;

  /**
   * @brief Returns number of children in the container.
   */
  std::size_t GetSize() const;

  /**
   * @brief Returns all children in the container.
   */
  std::vector<SessionItem*> GetChildren() const;

  /**
   * @brief Removes all children from the container.
   */
  void Clear();
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_CONTAINER_ITEM_H_
