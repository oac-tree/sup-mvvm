// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H
#define MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H

#include <QVariant>

namespace ModelView
{
//! Defines interface for ViewItem's data access.

class ViewItemDataInterface
{
public:
  virtual QVariant data(int role = Qt::EditRole) const = 0;

  virtual bool setData(const QVariant& data, int role = Qt::EditRole) = 0;

  virtual bool isEnabled() const = 0;

  virtual bool isEditable() const = 0;
};

}  // namespace ModelView

#endif // MVVM_VIEWMODEL_VIEWITEMDATAINTERFACE_H
