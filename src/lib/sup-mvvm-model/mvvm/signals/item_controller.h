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

#ifndef MVVM_SIGNALS_ITEM_CONTROLLER_H_
#define MVVM_SIGNALS_ITEM_CONTROLLER_H_

#include <mvvm/signals/item_listener.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The ItemController class provides a subscription/unsubscription mechanism to listen for
 * changes in SessionItem.
 *
 * It is normally intended to be a base class for a controller to work with multiple items (one item
 * at a time). The user should override Subscribe/Unsubscribe methods to provide automatic
 * connections and additional bookkeeping on new item changes.
 *
 * To provide simple item listening of a single item, use the ItemListener class instead.
 *
 * @tparam ItemT Type of the concrete SessionItem to listen.
 *
 */

template <typename ItemT>
class ItemController
{
public:
  /**
   * @brief Default c-tor when no item is set for listening yet.
   */
  ItemController() = default;

  virtual ~ItemController() = default;

  /**
   * @brief Sets and item for listening.
   */
  void SetItem(SessionItem* item)
  {
    // no need to subscribe again if item is the same
    if (GetItem() == item)
    {
      return;
    }

    // break previous subscription, if exist
    if (GetItem())
    {
      Unsubscribe();
      m_listener.reset();
    }

    // if provided item is nullptr, leave everything unconnected
    if (!item)
    {
      return;
    }

    // subscribe to new item
    m_listener = std::make_unique<ItemListener>(item);
    Subscribe();
  }

  /**
   * @brief Returns current item which was set for listening.
   */
  ItemT* GetItem() const
  {
    return static_cast<ItemT*>(m_listener ? m_listener->GetItem() : nullptr);
  }

  /**
   * @brief Returns underlying item listener.
   *
   * On every new item set, underlying listener is regenerated.
   */
  ItemListener* Listener() const { return m_listener.get(); }

protected:
  /**
   * @brief Subscribe to a new item.
   *
   * Will be called on new item set. This method should be overridden in the derived class if the
   * user wants to perform certain actions for the new item. For example, this is the place to
   * subscribe other methods or lambdas to certain event types.
   */
  virtual void Subscribe() {}

  /**
   * @brief Unsubscribe from the previous item.
   *
   * Will be called on new item set. This method should be overridden in the derived class if the
   * user wants to perform additional unsubscription-related activity for the previous item. For
   * example, it can be the cleanup of some view items, or controllers. Please note, that
   * unsubscription from signals of the previous item will be performed automatically.
   */
  virtual void Unsubscribe() {}

private:
  std::unique_ptr<ItemListener> m_listener;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_CONTROLLER_H_
