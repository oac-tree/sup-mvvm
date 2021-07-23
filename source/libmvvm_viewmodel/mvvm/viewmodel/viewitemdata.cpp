// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "viewitemdata.h"

using namespace ModelView;

ViewItemData::ViewItemData() : m_is_enabled(true), m_is_editable(true) {}

QVariant ViewItemData::data(int role) const
{
  return m_data[role];
}

bool ViewItemData::setData(const QVariant& data, int role)
{
  m_data[role] = data;
  return true;
}

bool ViewItemData::isEnabled() const
{
  return m_is_enabled;
}

void ViewItemData::setEnabled(bool value)
{
  m_is_enabled = value;
}

bool ViewItemData::isEditable() const
{
  return m_is_editable;
}

void ViewItemData::setEditable(bool value)
{
  m_is_editable = value;
}
