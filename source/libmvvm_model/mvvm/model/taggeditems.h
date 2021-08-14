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

#ifndef MVVM_MODEL_TAGGEDITEMS_H
#define MVVM_MODEL_TAGGEDITEMS_H

#include "mvvm/model/tagindex.h"
#include "mvvm/model_export.h"

#include <memory>
#include <string>
#include <vector>

namespace ModelView
{
class SessionItemContainer;
class TagInfo;
class SessionItem;

//! Collection of SessionItem's containers according to their tags.

class MVVM_MODEL_EXPORT TaggedItems
{
public:
  using container_t = std::vector<SessionItemContainer*>;
  using const_iterator = container_t::const_iterator;

  TaggedItems();
  ~TaggedItems();
  TaggedItems(const TaggedItems&) = delete;
  TaggedItems& operator=(const TaggedItems&) = delete;

  // tag

  void RegisterTag(const TagInfo& tag_info, bool set_as_default = false);

  bool HasTag(const std::string& name) const;

  std::string GetDefaultTag() const;

  void SetDefaultTag(const std::string& name);

  int GetItemCount(const std::string& tag_name) const;

  // adding and removal

  bool CanInsertItem(const SessionItem* item, const TagIndex& tag_index) const;

  bool InsertItem(SessionItem* item, const TagIndex& tag_index);

  bool CanTakeItem(const TagIndex& tag_index) const;

  SessionItem* TakeItem(const TagIndex& tag_index);

  // item access
  SessionItem* GetItem(const TagIndex& tag_index) const;

  std::vector<SessionItem*> GetItems(const std::string& tag = {}) const;

  std::vector<SessionItem*> GetAllItems() const;

  TagIndex TagIndexOfItem(const SessionItem* item) const;

  const_iterator begin() const;
  const_iterator end() const;

  bool IsSinglePropertyTag(const std::string& tag) const;

  int GetTagsCount() const;

  SessionItemContainer& GetContainer(int index);

  void AppendContainer(std::unique_ptr<SessionItemContainer> container);

private:
  SessionItemContainer* container(const std::string& tag_name) const;
  SessionItemContainer* find_container(const std::string& tag_name) const;

  std::vector<SessionItemContainer*> m_containers;
  std::string m_default_tag;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_TAGGEDITEMS_H
