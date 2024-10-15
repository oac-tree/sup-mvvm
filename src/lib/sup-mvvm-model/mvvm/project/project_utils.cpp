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

#include "project_utils.h"

#include "file_based_project.h"
#include "folder_based_project.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/project/i_project.h>
#include <mvvm/utils/file_utils.h>

#include <algorithm>
#include <cctype>

namespace
{
const std::string kXMLExtension = ".xml";
const std::string kUntitledName = "Untitled";
const std::string kDefaultModelName = "application_model";
}  // namespace

namespace mvvm::utils
{

std::string SuggestFileName(const ISessionModel& model)
{
  std::string result = model.GetType().empty() ? kDefaultModelName : model.GetType();
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result + kXMLExtension;
}

bool IsPossibleProjectDir(const std::string& project_dir)
{
  return !utils::FindFiles(project_dir, kXMLExtension).empty();
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
  return ProjectWindowTitle(project.GetProjectPath(), project.IsModified());
}

std::string ProjectWindowTitle(const std::string& project_dir, bool is_modified)
{
  auto pos = project_dir.find_last_of('/');
  auto project_name = (pos == std::string::npos ? kUntitledName : project_dir.substr(pos + 1));
  auto unsaved_status = is_modified ? "*" : "";
  return unsaved_status + project_name;
}

}  // namespace mvvm::utils
