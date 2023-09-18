/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_VIEWITEM_MAP_H_
#define MVVM_VIEWMODEL_VIEWITEM_MAP_H_

#include <mvvm/viewmodel_export.h>

#include <map>

namespace mvvm
{
class ViewItem;
class SessionItem;

//! Stores correspondance of the SessionItem and ViewItem. Plays a supporting role during ViewModel
//! rebuild.

class MVVM_VIEWMODEL_EXPORT ViewItemMap
{
public:
  ViewItemMap();

  /**
   * @brief Insert item and its corresponding view to the database.
   */
  void Insert(const SessionItem* item, ViewItem* view_item);

  /**
   * @brief Find view for given item.
   */
  ViewItem* FindView(const SessionItem* item) const;

  /**
   * @brief Removes view corresponding to given item.
   */
  void Remove(const SessionItem* item);

  /**
   * @brief Remove item, all its children and their corresponsding views from the map.
   */
  void OnItemRemove(const SessionItem* item);

  /**
   * @brief Clear the whole map.
   */
  void Clear();

  /**
   * @brief Returns number of entries in a map.
   */
  int GetSize() const;

private:
  std::map<const SessionItem*, ViewItem*> m_item_to_view;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWITEM_MAP_H_
