/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_CORE_VERSION_H
#define MVVM_CORE_VERSION_H

//! @file version.h
//! Automatically generated from mvvm_version.h.in

#include <string>

namespace ModelView
{

//! Returns major project version.
inline int ProjectVersionMajor()
{
    const int project_version_major = 0;
    return project_version_major;
}

//! Returns minor project version.
inline int ProjectVersionMinor()
{
    const int project_version_minor = 1;
    return project_version_minor;
}

//! Returns patch project version.
inline int ProjectVersionPatch()
{
    const int project_version_path = 0;
    return project_version_path;
}

//! Returns project version string
inline std::string ProjectVersion()
{
    const std::string project_version = "0.1.0";
    return project_version;
}

} // namespace ModelView

#endif // MVVM_CORE_VERSION_H
