/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROVIDERS_CUSTOM_METATYPES_H
#define MVVM_PROVIDERS_CUSTOM_METATYPES_H

namespace mvvm
{

/**
 * @brief Registers custom metatypes involved in Qt's signal/slot mahcinery.
 */
void RegisterCustomMetaTypes();

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_CUSTOM_METATYPES_H
