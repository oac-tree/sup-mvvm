/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_FACTORIES_MODEL_DOCUMENT_FACTORY_H_
#define MVVM_FACTORIES_MODEL_DOCUMENT_FACTORY_H_

#include <mvvm/serialization/i_model_document.h>

#include <memory>
#include <vector>

namespace mvvm
{

class ISessionModel;

/**
 * @brief Creates XmlDocument to save and load models.
 *
 * @param models Models to save in a single XML file.
 * @param application_name The name of the aplication which.
 */
MVVM_MODEL_EXPORT std::unique_ptr<IModelDocument> CreateXmlDocument(
    const std::vector<ISessionModel*>& models, const std::string& application_name);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_MODEL_DOCUMENT_FACTORY_H_
