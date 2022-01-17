// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/standarditems/linkeditem.h"
#include "mvvm/model/sessionmodel.h"

namespace
{
const std::string empty_link;
}

namespace mvvm
{

LinkedItem::LinkedItem() : SessionItem(Type)
{
  SetData(empty_link);
  SetEditable(false);  // prevent editing in widgets, link is set programmatically.
}

//! Set link to given item.

void LinkedItem::SetLink(const SessionItem* item)
{
  SetData(item ? item->GetIdentifier() : empty_link);
}

SessionItem *LinkedItem::GetLink() const
{
  return GetModel() ? GetModel()->FindItem(Data<std::string>()) : nullptr;
}

}  // namespace mvvm
