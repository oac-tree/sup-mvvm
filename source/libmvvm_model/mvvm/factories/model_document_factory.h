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

#ifndef MVVM_FACTORIES_MODELDOCUMENTFACTORY_H
#define MVVM_FACTORIES_MODELDOCUMENTFACTORY_H

#include <mvvm/interfaces/model_document_interface.h>

#include <memory>
#include <vector>

namespace mvvm
{

class ApplicationModel;

//! Creates XmlDocument to save and load models.
MVVM_MODEL_EXPORT std::unique_ptr<ModelDocumentInterface> CreateXmlDocument(
    const std::vector<ApplicationModel*>& models);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_MODELDOCUMENTFACTORY_H
