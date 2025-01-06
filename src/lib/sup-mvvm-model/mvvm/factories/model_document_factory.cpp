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

#include "model_document_factory.h"

#include <mvvm/serialization/xml_document.h>

namespace mvvm
{

std::unique_ptr<IModelDocument> CreateXmlDocument(
    const std::vector<ISessionModel *> &models, const std::string &application_name)
{
  return std::make_unique<XmlDocument>(models, application_name);
}

}  // namespace mvvm
