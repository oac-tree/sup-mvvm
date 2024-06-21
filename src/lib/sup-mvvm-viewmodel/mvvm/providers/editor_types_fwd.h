/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROVIDERS_EDITOR_TYPES_FWD_H_
#define MVVM_PROVIDERS_EDITOR_TYPES_FWD_H_

#include <mvvm/viewmodel_export.h>

#include <functional>
#include <memory>

class QWidget;

namespace mvvm
{

class CustomEditor;
class SessionItem;

using editor_t = std::unique_ptr<QWidget>;
using editorbuilder_t = std::function<editor_t(const SessionItem*)>;

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_EDITOR_TYPES_FWD_H_
