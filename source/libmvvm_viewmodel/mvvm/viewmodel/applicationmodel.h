/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef MVVM_VIEWMODEL_APPLICATIONMODEL_H
#define MVVM_VIEWMODEL_APPLICATIONMODEL_H

#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel_export.h"

namespace ModelView
{
class ModelEventListenerInterface;

//! Main class to hold application data. Extends SessionModel with signaling capabilities.
//! All modification of the model are done through the composer, which takes care
//! about notifications and all subscribers.

class MVVM_VIEWMODEL_EXPORT ApplicationModel : public SessionModel
{
public:
  explicit ApplicationModel(std::string model_type = {}, std::shared_ptr<ItemPool> pool = {});
  ~ApplicationModel() override;

  void Subscribe(ModelEventListenerInterface* listener);

  virtual SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent,
                                     const TagIndex& tag_index) override;

  virtual void RemoveItem(SessionItem* parent, const TagIndex& tag_index) override;

  virtual bool SetData(SessionItem* item, const variant_t& value, int role) override;

private:
  struct ApplicationModelImpl;
  std::unique_ptr<ApplicationModelImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_APPLICATIONMODEL_H
