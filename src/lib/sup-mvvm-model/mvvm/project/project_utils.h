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

#ifndef MVVM_PROJECT_PROJECT_UTILS_H_
#define MVVM_PROJECT_PROJECT_UTILS_H_

//! @file
//! Collection of utility functions to handle project saving and loading.

#include <mvvm/model_export.h>
#include <mvvm/project/project_types.h>

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{

class ISessionModel;
class IProject;
struct ProjectContext;

namespace utils
{

/**
 * @brief Suggests file name which can be used to store Xml content of given model. Uses the model
 * type to construct a filename: MaterialModel -> materialmodel.xml
 */
std::string SuggestFileName(const ISessionModel& model);

/**
 * @brief Creates new untitled project.
 */
std::unique_ptr<IProject> CreateUntitledProject(ProjectType project_type,
                                                const std::vector<ISessionModel*>& models,
                                                const ProjectContext& context);

/**
 * @brief Returns 'true' if given directory might be a project directory.
 *
 * @details This simplified check counts number of files with XML extention.
 */
bool IsPossibleProjectDir(const std::string& project_dir);

/**
 * @brief Returns a MainWindow title for given project.
 */
std::string ProjectWindowTitle(const IProject& project);

/**
 * @brief Returns a title composed from last part of project path, and `is_modified` flag.
 *
 * @details Project without projectDir will be "Untitled", modified project will be "*Untitled".
 * Project with projectDir in "/home/user/project1" will get title "project1".
 */
std::string ProjectWindowTitle(const std::string& project_dir, bool is_modified);

}  // namespace utils

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_UTILS_H_
