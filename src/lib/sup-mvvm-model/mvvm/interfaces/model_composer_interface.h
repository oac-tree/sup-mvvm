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

#ifndef MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_
#define MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_

#include <mvvm/core/variant.h>
#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class TagIndex;
class ISessionModel;

/**
 * @brief The ModelComposerInterface class is an nterface for all classes to compose the model.
 *
 * Used in various decorators for undo/redo and notifications while composing the model.
 */
class MVVM_MODEL_EXPORT ModelComposerInterface
{
public:
  virtual ~ModelComposerInterface() = default;

  /**
   * @brief Inserts an item into the given parent and takes ownership of it.
   *
   * @param An item to insert.
   * @param parent The parent where to insert.
   * @param tag_index A tag_index pointing to the inserted place.
   * @return The pointer to the inserted item.
   */
  virtual SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                  const TagIndex& tag_index) = 0;

  /**
   * @brief Takes a child from a parent and returns it to the caller.
   *
   * @param parent A parent item from where take the item.
   * @param tag_index A tag_index pointing to the child.
   * @return Taken item.
   */
  virtual std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  /**
   * @brief Sets the value to the given data role of the given item.
   *
   * If the data is the same as before, will return false and will suppress notifications. It is not
   * possible to change the data type for a given role, once the role is set for the first time.
   * I.e. an attempt to set an integer to the role containing a string will lead to the exception.
   * User utils::ReplaceData if you need to change a data type.
   *
   * @param item The item to set the data.
   * @param value The value.
   * @param role The data role.
   * @return Returns true, if the data was changed.
   */
  virtual bool SetData(SessionItem* item, const variant_t& value, int role) = 0;

  /**
   * @brief Resets the model by
   * @param old_root_item
   * @param new_root_item
   */

  /**
   * @brief Replaces existing root item with new root item by moving from it.
   *
   * This method is used in serialization to restore the model from persistent content.
   *
   * @param root_item New root item, possibly pre-filled with some content.
   */
  virtual void ReplaceRootItem(std::unique_ptr<SessionItem>& old_root_item,
                     std::unique_ptr<SessionItem> new_root_item) = 0;

  /**
   * @brief Returns a model server by this composer.
   */
  virtual ISessionModel* GetModel() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_
