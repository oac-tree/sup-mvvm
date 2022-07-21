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

#include "mvvm/model/application_model.h"

#include <mvvm/interfaces/item_manager_interface.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/notifying_model.h>
#include <mvvm/model/sessionmodel.h>

namespace mvvm
{

namespace
{
std::unique_ptr<mvvm::SessionModelInterface> CreateDecoratedModel(
    std::string model_type, std::unique_ptr<mvvm::ItemManagerInterface> manager)
{
  auto model = std::make_unique<mvvm::SessionModel>(std::move(model_type), std::move(manager));
  return std::make_unique<mvvm::NotifyingModel>(std::move(model));
}

}  // namespace

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), std::move(CreateDefaultItemManager()))
{
}

ApplicationModel::ApplicationModel(std::string model_type,
                                   std::unique_ptr<ItemManagerInterface> manager)
    : AbstractModelDecorator(CreateDecoratedModel(std::move(model_type), std::move(manager)))
{
  m_decorated_model->GetRootItem()->SetModel(this);
}

void ApplicationModel::Clear(std::unique_ptr<SessionItem> root_item, SessionModelInterface *model)
{
  AbstractModelDecorator::Clear(std::move(root_item), model ? model : this);
}

}  // namespace mvvm
