// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_VIEWMODEL_VIEWITEMDATA_H
#define MVVM_VIEWMODEL_VIEWITEMDATA_H

#include "viewitemdatainterface.h"

#include <QMap>

namespace ModelView
{
//! Carries the data for ViewItem.

class ViewItemData : public ViewItemDataInterface
{
public:
  ViewItemData();

  virtual ~ViewItemData() = default;

  virtual QVariant data(int role = Qt::EditRole) const override;

  virtual bool setData(const QVariant& data, int role = Qt::EditRole) override;

  bool isEnabled() const override;

  void setEnabled(bool value);

  bool isEditable() const override;

  void setEditable(bool value);

private:
  bool m_is_enabled;
  bool m_is_editable;
  QMap<int, QVariant> m_data;
};

}  // namespace ModelView

#endif // MVVM_VIEWMODEL_VIEWITEMDATA_H
