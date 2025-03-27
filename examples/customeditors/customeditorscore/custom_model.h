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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_MODEL_H_
#define CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_MODEL_H_

#include <mvvm/model/application_model.h>

namespace customeditors
{

/**
 * @brief The CustomModel class is a model with few items to study cell editors with auto-complete
 * features.
 */
class CustomModel : public mvvm::ApplicationModel
{
public:
  CustomModel();
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_MODEL_H_
