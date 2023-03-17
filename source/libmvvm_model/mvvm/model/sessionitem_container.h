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

#ifndef MVVM_MODEL_SESSIONITEM_CONTAINER_H_
#define MVVM_MODEL_SESSIONITEM_CONTAINER_H_

#include <mvvm/model/taginfo.h>

#include <vector>
#include <memory>

namespace mvvm
{
class SessionItem;

//! Holds collection of SessionItem objects related to the same tag.

class MVVM_MODEL_EXPORT SessionItemContainer
{
public:
  using container_t = std::vector<std::unique_ptr<SessionItem>>;
  using const_iterator = container_t::const_iterator;

  explicit SessionItemContainer(TagInfo tag_info);
  SessionItemContainer(const SessionItemContainer&) = delete;
  SessionItemContainer& operator=(const SessionItemContainer&) = delete;

  ~SessionItemContainer();

  bool IsEmpty() const;

  int GetItemCount() const;

  std::vector<SessionItem*> GetItems() const;

  bool InsertItem(SessionItem* item, int index);

  std::unique_ptr<SessionItem> TakeItem(int index);

  bool CanTakeItem(int index) const;

  bool CanInsertItem(const SessionItem* item, int index) const;

  int IndexOfItem(const SessionItem* item) const;

  SessionItem* ItemAt(int index) const;

  std::string GetName() const;

  TagInfo GetTagInfo() const;

  const_iterator begin() const;

  const_iterator end() const;

private:
  bool IsMaximumReached() const;
  bool IsMinimumReached() const;

  TagInfo m_tag_info;
  container_t m_items;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONITEM_CONTAINER_H_
