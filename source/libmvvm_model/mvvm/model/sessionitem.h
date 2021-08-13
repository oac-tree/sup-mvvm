// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_SESSIONITEM_H
#define MVVM_MODEL_SESSIONITEM_H

#include "mvvm/core/variant.h"
#include "mvvm/model/mvvm_types.h"
#include "mvvm/model/tagindex.h"

#include <memory>
#include <stdexcept>
#include <vector>

namespace ModelView
{
class SessionModel;
class TagInfo;
class SessionItemData;
class TaggedItems;

//! The main object representing an editable/displayable/serializable entity. Serves as a
//! construction element (node) of SessionModel to represent all the data of GUI application.

class MVVM_MODEL_EXPORT SessionItem
{
public:
  static inline const std::string Type = "SessionItem";

  SessionItem();
  virtual ~SessionItem();

  SessionItem(const SessionItem&) = delete;
  SessionItem& operator=(const SessionItem&) = delete;

  // basic item properties

  std::string modelType() const;

  std::string identifier() const;

  virtual SessionItem* setDisplayName(const std::string& name);
  virtual std::string displayName() const;

  SessionModel* model() const;

  SessionItem* parent() const;

  TagIndex GetTagIndex() const;

  // methods to deal with item data

  bool hasData(int role = DataRole::kData) const;

  variant_t data(int role = DataRole::kData) const { return data_internal(role);}

  template <typename T>
  T data(int role = DataRole::kData) const;

  template <typename T>
  bool setData(const T& value, int role = DataRole::kData, bool direct = false);

  SessionItemData* itemData();
  const SessionItemData* itemData() const;

  // children access

  int childrenCount() const;

  std::vector<SessionItem*> children() const;

  int itemCount(const std::string& tag) const;

  SessionItem* getItem(const std::string& tag, int index = 0) const;

  std::vector<SessionItem*> getItems(const std::string& tag) const;

  template <typename T>
  T* item(const std::string& tag) const;
  template <typename T = SessionItem>
  std::vector<T*> items(const std::string& tag) const;

  TagIndex TagIndexOfItem(const SessionItem* item) const;

  void registerTag(const TagInfo& tagInfo, bool set_as_default = false);

  TaggedItems* itemTags();
  const TaggedItems* itemTags() const;

  // item manipulation

  bool insertItem(SessionItem* item, const TagIndex& tag_index);

  SessionItem* insertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index);
  template <typename T = SessionItem>
  T* insertItem(const TagIndex& tag_index);

  std::unique_ptr<SessionItem> takeItem(const TagIndex& tag_index);

  // more convenience methods

  bool isEditable() const;
  SessionItem* setEditable(bool value);

  bool isEnabled() const;
  SessionItem* setEnabled(bool value);

  bool isVisible() const;
  SessionItem* setVisible(bool value);

  std::string toolTip() const;
  SessionItem* setToolTip(const std::string& tooltip);

  template <typename T>
  T property(const std::string& tag) const;
  template <typename T>
  void setProperty(const std::string& tag, const T& value);
  void setProperty(const std::string& tag, const char* value);

protected:
  explicit SessionItem(const std::string& modelType);

private:
  friend class SessionModel;
  friend class TreeDataItemConverter;
  bool set_data_internal(const variant_t& value, int role, bool direct);
  variant_t data_internal(int role) const;
  void setParent(SessionItem* parent);
  void setModel(SessionModel* model);
  void setAppearanceFlag(int flag, bool value);

  void setDataAndTags(std::unique_ptr<SessionItemData> data, std::unique_ptr<TaggedItems> tags);

  struct SessionItemImpl;
  std::unique_ptr<SessionItemImpl> p_impl;
};

//! Sets data for a given role. When extra parameter `direct` is false (default case), will act
//! through the model to register command in undo/redo framework (if enabled) and so allow later
//! undo.

template <typename T>
inline bool SessionItem::setData(const T& value, int role, bool direct)
{
  return set_data_internal(value, role, direct);
}

//! Returns data of given type T for given role.

template <typename T>
inline T SessionItem::data(int role) const
{
  return std::get<T>(data_internal(role));
}

//! Returns first item under given tag casted to a specified type.
//! Returns nullptr, if item doesn't exist. If item exists but can't be casted will throw.

template <typename T>
inline T* SessionItem::item(const std::string& tag) const
{
  if (auto item = getItem(tag); item)
  {
    T* tag_item = dynamic_cast<T*>(item);
    if (!tag_item)
      throw std::runtime_error("Can't cast an item to given type");
    return tag_item;
  }
  return nullptr;
}

//! Returns all items under given tag casted to specific type.

template <typename T>
std::vector<T*> SessionItem::items(const std::string& tag) const
{
  std::vector<T*> result;
  for (auto item : getItems(tag))
    if (auto casted = dynamic_cast<T*>(item); casted)
      result.push_back(casted);
  return result;
}

//! Creates a new item and insert it into given tag under the given row.
//! Returns pointer to inserted item to the user.

template <typename T>
inline T* SessionItem::insertItem(const TagIndex& tag_index)
{
  return static_cast<T*>(insertItem(std::make_unique<T>(), tag_index));
}

//! Returns data stored in property item.
//! Property is single item registered under certain tag via CompoundItem::addProperty method.

template <typename T>
inline T SessionItem::property(const std::string& tag) const
{
  return getItem(tag)->data<T>();
}

//! Sets value to property item.
//! Property is single item registered under certain tag via CompoundItem::addProperty method, the
//! value will be assigned to it's data role.

template <typename T>
inline void SessionItem::setProperty(const std::string& tag, const T& value)
{
  getItem(tag)->setData(value);
}

//! Sets value to property item (specialized for special "const char *" case).
//! Property is single item registered under certain tag via CompoundItem::addProperty method, the
//! value will be assigned to it's data role.

inline void SessionItem::setProperty(const std::string& tag, const char* value)
{
  setProperty(tag, std::string(value));
}

}  // namespace ModelView

#endif  // MVVM_MODEL_SESSIONITEM_H
