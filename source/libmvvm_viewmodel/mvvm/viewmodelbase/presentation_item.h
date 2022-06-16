/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODELBASE_PRESENTATION_ITEM_H_
#define MVVM_VIEWMODELBASE_PRESENTATION_ITEM_H_

#include <mvvm/viewmodelbase/viewitem_data.h>

#include <functional>

namespace mvvm
{
//! Forward data/setData calls to user-provided callbacks.

template <typename T>
class PresentationItem : public ViewItemData
{
public:
  using data_callback_t = std::function<QVariant(T*, int)>;
  using setdata_callback_t = std::function<bool(T*, QVariant, int)>;

  explicit PresentationItem(T* context, data_callback_t data_callback = {},
                            setdata_callback_t setdata_callback = {})
      : m_context(context), m_data_callback(data_callback), m_setdata_callback(setdata_callback)
  {
  }

  QVariant Data(int role = Qt::EditRole) const override
  {
    return m_data_callback ? m_data_callback(m_context, role) : QVariant();
  }

  bool SetData(const QVariant& data, int role = Qt::EditRole) override
  {
    return m_setdata_callback ? m_setdata_callback(m_context, data, role) : false;
  }

  const T* GetContext() const { return m_context; }

  T* GetContext()
  {
    return const_cast<T*>(static_cast<const PresentationItem<T>*>(this)->GetContext());
  }

private:
  T* m_context;
  data_callback_t m_data_callback;
  setdata_callback_t m_setdata_callback;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODELBASE_PRESENTATION_ITEM_H_
