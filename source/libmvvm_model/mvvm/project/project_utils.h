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

#ifndef MVVM_PROJECT_PROJECT_UTILS_H_
#define MVVM_PROJECT_PROJECT_UTILS_H_

#include <mvvm/model_export.h>

#include <memory>
#include <string>

namespace mvvm
{

class SessionModelInterface;
class ProjectInterface;
struct ProjectContext;

//! Collection of utility functions to handle project saving and loading.

namespace ProjectUtils
{

MVVM_MODEL_EXPORT std::string SuggestFileName(const SessionModelInterface& model);

MVVM_MODEL_EXPORT bool IsPossibleProjectDir(const std::string& project_dir);

MVVM_MODEL_EXPORT std::unique_ptr<ProjectInterface> CreateUntitledProject(
    const ProjectContext& context);

MVVM_MODEL_EXPORT std::string ProjectWindowTitle(const ProjectInterface& project);

MVVM_MODEL_EXPORT std::string ProjectWindowTitle(const std::string& project_dir, bool is_modified);

}  // namespace ProjectUtils

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_UTILS_H_
