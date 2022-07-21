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

#ifndef MVVM_MODEL_APPLICATION_MODEL_V2_H_
#define MVVM_MODEL_APPLICATION_MODEL_V2_H_

#include <mvvm/model/abstract_model_decorator.h>

namespace mvvm
{
class ModelEventSubscriberInterface;
class ItemManagerInterface;

//! Main class to hold application data. Extends SessionModel with signaling capabilities.
//! All modification of the model are done through the composer, which takes care
//! about notifications and all subscribers.

class MVVM_MODEL_EXPORT ApplicationModelV2 : public AbstractModelDecorator
{
public:
  using SessionModelInterface::InsertItem;

  explicit ApplicationModelV2(std::string model_type = {});
  ApplicationModelV2(std::string model_type, std::unique_ptr<ItemManagerInterface> manager);

  void Clear(std::unique_ptr<SessionItem> root_item, SessionModelInterface* model) override;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_V2_H_
