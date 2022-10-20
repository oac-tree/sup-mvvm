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

#ifndef MVVM_MODEL_APPLICATION_MODEL_H_
#define MVVM_MODEL_APPLICATION_MODEL_H_

#include <mvvm/model/sessionmodel.h>

#include <memory>

namespace mvvm
{
class ModelEventHandler;
class ItemManagerInterface;
class CommandStackInterface;

//! Main class to hold application data. Extends SessionModel with signaling capabilities.
//! All modification of the model are done through the composer, which takes care
//! about notifications and all subscribers.

class MVVM_MODEL_EXPORT ApplicationModel : public SessionModel
{
public:
  using SessionModelInterface::InsertItem;

  explicit ApplicationModel(std::string model_type = {});
  ApplicationModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager);

  ~ApplicationModel() override;

  ModelEventHandler* GetSubscriber() const override;

  void CheckIn(SessionItem* item) override;

  void SetUndoEnabled(bool value);

  CommandStackInterface* GetCommandStack() const;

private:
  struct ApplicationModelImpl;
  std::unique_ptr<ApplicationModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_V2_H_
