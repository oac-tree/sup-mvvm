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

#ifndef MVVM_CORE_VERSION_H_
#define MVVM_CORE_VERSION_H_

//! Project version information as defined by CMake project

#include <string>

namespace mvvm
{

//! Returns major project version.
int ProjectVersionMajor();

//! Returns minor project version.
int ProjectVersionMinor();

//! Returns patch project version.
int ProjectVersionPatch();

//! Returns project version string.
std::string ProjectVersion();

}  // namespace sequencergui

#endif  // MVVM_CORE_VERSION_H_
