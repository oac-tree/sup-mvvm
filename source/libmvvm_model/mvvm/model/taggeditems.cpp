// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/taggeditems.h"

#include "mvvm/model/sessionitemcontainer.h"

#include <stdexcept>

using namespace ModelView;

TaggedItems::TaggedItems() = default;

TaggedItems::~TaggedItems()
{
  for (auto tag : m_containers) delete tag;
}

void TaggedItems::registerTag(const TagInfo& tagInfo, bool set_as_default)
{
  if (isTag(tagInfo.name()))
    throw std::runtime_error("SessionItemTags::registerTag() -> Error. Existing name '"
                             + tagInfo.name() + "'");

  m_containers.push_back(new SessionItemContainer(tagInfo));
  if (set_as_default)
    m_default_tag = tagInfo.name();
}

//! Returns true if container with such name exists.

bool TaggedItems::isTag(const std::string& name) const
{
  for (auto tag : m_containers)
    if (tag->name() == name)
      return true;
  return false;
}

//! Returns the name of the default tag.

std::string TaggedItems::defaultTag() const
{
  return m_default_tag;
}

void TaggedItems::setDefaultTag(const std::string& name)
{
  m_default_tag = name;
}

int TaggedItems::itemCount(const std::string& tag_name) const
{
  return container(tag_name)->itemCount();
}

//! Returns true if item can be inserted.

bool TaggedItems::canInsertItem(const SessionItem* item, const TagIndex& tag_index) const
{
  auto tag_container = container(tag_index.tag);
  // negative index means appending to the vector
  auto index = tag_index.index < 0 ? tag_container->itemCount() : tag_index.index;
  return container(tag_index.tag)->canInsertItem(item, index);
}

//! Inserts item in container with given tag name and at given index.
//! Returns true in the case of success. If tag name is empty, default tag will be used.

bool TaggedItems::insertItem(SessionItem* item, const TagIndex& tag_index)
{
  auto tag_container = container(tag_index.tag);
  // negative index means appending to the vector
  auto index = tag_index.index < 0 ? tag_container->itemCount() : tag_index.index;
  return container(tag_index.tag)->insertItem(item, index);
}

//! Returns true if item can be taken.

bool TaggedItems::canTakeItem(const TagIndex& tag_index) const
{
  return container(tag_index.tag)->canTakeItem(tag_index.index);
}

//! Removes item at given index and for given tag, returns it to the user.

SessionItem* TaggedItems::takeItem(const TagIndex& tag_index)
{
  return container(tag_index.tag)->takeItem(tag_index.index);
}

//! Returns item at given index of given tag.

SessionItem* TaggedItems::getItem(const TagIndex& tag_index) const
{
  return container(tag_index.tag)->itemAt(tag_index.index);
}

//! Returns vector of items in the container with given name.
//! If tag name is empty, default tag will be used.

std::vector<SessionItem*> TaggedItems::getItems(const std::string& tag) const
{
  return container(tag)->items();
}

std::vector<SessionItem*> TaggedItems::allitems() const
{
  std::vector<SessionItem*> result;
  for (auto cont : m_containers)
  {
    auto container_items = cont->items();
    result.insert(result.end(), container_items.begin(), container_items.end());
  }

  return result;
}

//! Returns tag name and index of item in container.

TagIndex TaggedItems::TagIndexOfItem(const SessionItem* item) const
{
  for (auto cont : m_containers)
  {
    int index = cont->indexOfItem(item);
    if (index != -1)
      return {cont->name(), index};
  }

  return {};
}

TaggedItems::const_iterator TaggedItems::begin() const
{
  return m_containers.begin();
}

TaggedItems::const_iterator TaggedItems::end() const
{
  return m_containers.end();
}

//! Returns true if given tag corresponds to registered single property tag.

bool TaggedItems::isSinglePropertyTag(const std::string& tag) const
{
  auto cont = find_container(tag);
  return cont ? cont->tagInfo().isSinglePropertyTag() : false;
}

int TaggedItems::tagsCount() const
{
  return static_cast<int>(m_containers.size());
}

SessionItemContainer& TaggedItems::at(int index)
{
  if (index < 0 || index >= tagsCount())
    throw std::runtime_error("Error it SessionItemTags: wrong container index");
  return *m_containers.at(index);
}

//! Returns container corresponding to given tag name. If name is empty,
//! default tag will be used. Exception is thrown if no such tag exists.

SessionItemContainer* TaggedItems::container(const std::string& tag_name) const
{
  std::string tagName = tag_name.empty() ? defaultTag() : tag_name;
  auto container = find_container(tagName);
  if (!container)
    throw std::runtime_error("SessionItemTags::container() -> Error. No such container '" + tagName
                             + "'");

  return container;
}

//! Returns container corresponding to given tag name.

SessionItemContainer* TaggedItems::find_container(const std::string& tag_name) const
{
  for (auto cont : m_containers)
    if (cont->name() == tag_name)
      return cont;

  return nullptr;
}
