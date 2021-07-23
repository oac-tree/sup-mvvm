// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_VIEWMODEL_PRESENTATIONITEM_H
#define MVVM_VIEWMODEL_PRESENTATIONITEM_H

#include "mvvm/viewmodel/viewitemdata.h"

#include <functional>

namespace ModelView
{
//! Forward data/setData calls to underlying sequencer parts.

template <typename T>
class PresentationItem : public ViewItemData
{
public:
  using data_callback_t = std::function<QVariant(T*, int)>;
  using setdata_callback_t = std::function<bool(T*, QVariant, int)>;

  PresentationItem(T* context, data_callback_t data_callback = {},
                   setdata_callback_t setdata_callback = {})
      : m_context(context), m_data_callback(data_callback), m_setdata_callback(setdata_callback)
  {
  }

  QVariant data(int role = Qt::EditRole) const override
  {
    return m_data_callback ? m_data_callback(m_context, role) : QVariant();
  }

  bool setData(const QVariant& data, int role = Qt::EditRole) override
  {
    return m_setdata_callback ? m_setdata_callback(m_context, data, role) : false;
  }

  const T* context() const { return m_context; }

  T* context() { return const_cast<T*>(static_cast<const PresentationItem<T>*>(this)->context()); }

private:
  T* m_context;
  data_callback_t m_data_callback;
  setdata_callback_t m_setdata_callback;
};

}  // namespace ModelView

#endif // MVVM_VIEWMODEL_PRESENTATIONITEM_H
