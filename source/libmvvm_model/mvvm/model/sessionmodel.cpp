// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/sessionmodel.h"

#include "mvvm/model/itemcatalogue.h"
#include "mvvm/model/itemfactory.h"
#include "mvvm/model/itemmanager.h"
#include "mvvm/model/itempool.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taginfo.h"

using namespace ModelView;

//! Pimpl class for SessionModel.

struct SessionModel::SessionModelImpl
{
  SessionModel* m_self{nullptr};
  std::string m_modelType;
  std::unique_ptr<ItemManager> m_itemManager;
  std::unique_ptr<SessionItem> m_root_item;
  SessionModelImpl(SessionModel* self, std::string modelType, std::shared_ptr<ItemPool> pool)
      : m_self(self)
      , m_modelType(std::move(modelType))
      , m_itemManager(std::make_unique<ItemManager>())
  {
    setItemPool(pool);
  }

  void setItemPool(std::shared_ptr<ItemPool> pool)
  {
    m_itemManager->setItemPool(pool ? std::move(pool) : std::make_shared<ItemPool>());
  }

  //! Creates root item.
  void createRootItem()
  {
    m_root_item = m_itemManager->createRootItem();
    m_root_item->setModel(m_self);
    m_root_item->registerTag(TagInfo::universalTag("rootTag"), /*set_as_default*/ true);
  }
};

//! Main c-tor.

SessionModel::SessionModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(pool)))

{
  p_impl->createRootItem();
}

SessionModel::~SessionModel()
{
  // Explicitely call root item's destructor. It uses p_impl pointer during own descruction
  // and we have to keep pimpl pointer intact. Without line below will crash on MacOS because
  // of pecularities of MacOS libc++. See explanations here:
  // http://ibob.github.io/blog/2019/11/07/dont-use-unique_ptr-for-pimpl/
  p_impl->m_root_item.reset();
}

//! Insert new item using item's modelType.

SessionItem* SessionModel::insertNewItem(const std::string& modelType, SessionItem* parent,
                                         const TagRow& tagrow)
{
  // intentionally passing by value inside lambda
  auto create_func = [this, modelType]() { return factory()->createItem(modelType); };
  return intern_insert(create_func, parent, tagrow);
}

//! Removes given row from parent.

void SessionModel::removeItem(SessionItem* parent, const TagRow& tagrow)
{
  parent->takeItem(tagrow);
}

//! Returns the data for given item and role.

variant_t SessionModel::data(SessionItem* item, int role) const
{
  return item->data(role);
}

//! Sets the data for given item.

bool SessionModel::setData(SessionItem* item, const variant_t& value, int role)
{
  return item->setData(value, role);
}

//! Returns model type.

std::string SessionModel::modelType() const
{
  return p_impl->m_modelType;
}

//! Returns root item of the model.

SessionItem* SessionModel::rootItem() const
{
  return p_impl->m_root_item.get();
}

//! Returns item factory which can generate all items supported by this model.

const ItemFactoryInterface* SessionModel::factory() const
{
  return p_impl->m_itemManager->factory();
}

//! Returns SessionItem for given identifier.

SessionItem* SessionModel::findItem(const std::string& id)
{
  return p_impl->m_itemManager->findItem(id);
}

//! Sets brand new catalog of user-defined items. They become available for undo/redo and
//! serialization. Internally user catalog will be merged with the catalog of standard items.

void SessionModel::setItemCatalogue(std::unique_ptr<ItemCatalogue> catalogue)
{
  // adding standard items to the user catalogue
  std::unique_ptr<ItemCatalogue> full_catalogue = std::move(catalogue);
  //  full_catalogue->merge(*CreateStandardItemCatalogue()); // FIXME restore
  p_impl->m_itemManager->setItemFactory(std::make_unique<ItemFactory>(std::move(full_catalogue)));
}

//! Removes all items from the model. If callback is provided, use it to rebuild content of root
//! item (used while restoring the model from serialized content).

void SessionModel::clear(std::function<void(SessionItem*)> callback)
{
  p_impl->createRootItem();
  if (callback)
    callback(rootItem());
}

//! Registers item in pool. This will allow to find item pointer using its unique identifier.

void SessionModel::registerInPool(SessionItem* item)
{
  p_impl->m_itemManager->registerInPool(item);
}

//! Unregister item from pool.

void SessionModel::unregisterFromPool(SessionItem* item)
{
  p_impl->m_itemManager->unregisterFromPool(item);
}

//! Insert new item into given parent using factory function provided.

SessionItem* SessionModel::intern_insert(const item_factory_func_t& func, SessionItem* parent,
                                         const TagRow& tagrow)
{
  return parent->insertItem(func(), tagrow);
}

void SessionModel::intern_register(const std::string& modelType, const item_factory_func_t& func,
                                   const std::string& label)
{
  p_impl->m_itemManager->factory()->registerItem(modelType, func, label);
}
