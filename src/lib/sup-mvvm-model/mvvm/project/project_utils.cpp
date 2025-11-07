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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "project_utils.h"

#include "file_based_project.h"
#include "folder_based_project.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/project/i_project.h>
#include <mvvm/utils/file_utils.h>

#include <algorithm>
#include <cctype>
#include <locale>

namespace
{
constexpr std::string_view kXMLExtension = ".xml";
constexpr std::string_view kUntitledName = "Untitled";
constexpr std::string_view kDefaultModelName = "application_model";
}  // namespace

namespace mvvm::utils
{

std::string SuggestFileName(const ISessionModel& model)
{
  std::locale loc("C");
  std::string result = model.GetType().empty() ? std::string(kDefaultModelName) : model.GetType();
  (void)std::transform(result.begin(), result.end(), result.begin(),
                       [&loc](char c) { return std::tolower(c, loc); });
  return result + std::string(kXMLExtension);
}

bool IsPossibleProjectDir(const std::string& project_dir)
{
  return !utils::FindFiles(project_dir, std::string(kXMLExtension)).empty();
}

std::unique_ptr<IProject> CreateUntitledProject(ProjectType project_type,
                                                const std::vector<ISessionModel*>& models,
                                                const ProjectContext& context)
{
  if (project_type == ProjectType::kFileBased)
  {
    return std::make_unique<FileBasedProject>(models, context);
  }

  return std::make_unique<FolderBasedProject>(models, context);
}

std::string ProjectWindowTitle(const IProject& project)
{
  return ProjectWindowTitle(project.GetPath(), project.IsModified());
}

std::string ProjectWindowTitle(const std::string& project_dir, bool is_modified)
{
  auto pos = project_dir.find_last_of('/');
  auto project_name =
      ((pos == std::string::npos) ? std::string(kUntitledName) : project_dir.substr(pos + 1));
  auto unsaved_status = is_modified ? "*" : "";
  return unsaved_status + project_name;
}

}  // namespace mvvm::utils
