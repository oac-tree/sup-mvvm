/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_CALLBACK_TYPES_H_
#define MVVM_SIGNALS_CALLBACK_TYPES_H_

#include <functional>
#include <string>

namespace mvvm
{
class SessionItem;
class SessionModelInterface;
class TagIndex;

namespace Callbacks
{
using item_t = std::function<void(SessionItem*)>;
using item_int_t = std::function<void(SessionItem*, int)>;
using item_str_t = std::function<void(SessionItem*, const std::string&)>;
using item_tagindex_t = std::function<void(SessionItem*, const TagIndex&)>;
using model_t = std::function<void(SessionModelInterface*)>;
}  // namespace Callbacks
}  // namespace mvvm

#endif  // MVVM_SIGNALS_CALLBACK_TYPES_H_
