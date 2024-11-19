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

#ifndef MVVM_PROJECT_EXTERNAL_MODEL_PROJECT_H_
#define MVVM_PROJECT_EXTERNAL_MODEL_PROJECT_H_

#include <mvvm/project/abstract_project.h>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The ExternalModelProject is a base for all projects that manipulate external models.
 *
 * Models are created somewhere else, normally in the MainWindow, and stay alive during the whole
 * life of the application. The ExternalModelProject clears these models when the project is
 * "closed" or "created as new". There are two concrete implementations of this class,
 * FileBasedProject, and FolderBasedProject, which implement actual save and load.
 *
 * The recommended alternative to this class is AppProject, which owns models and recreates them
 * during the creation of new projects.
 */
class MVVM_MODEL_EXPORT ExternalModelProject : public AbstractProject
{
public:
  explicit ExternalModelProject(ProjectType project_type, const std::vector<ISessionModel*>& models,
                                const ProjectContext& context);

  std::vector<ISessionModel*> GetModels() const override;

private:
  bool CloseProjectImpl() override;
  bool CreateEmptyProjectImpl() override;

  std::vector<ISessionModel*> m_models;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_EXTERNAL_MODEL_PROJECT_H_
