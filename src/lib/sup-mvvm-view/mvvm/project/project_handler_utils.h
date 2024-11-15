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

#ifndef MVVM_PROJECT_PROJECT_HANDLER_UTILS_H_
#define MVVM_PROJECT_PROJECT_HANDLER_UTILS_H_

//! @file
//! Contains utility functions to populate menus with (create new, open, save, save as) requests.

class QAction;
class QMenu;

namespace mvvm
{

class IProjectManager;

/**
 * @brief Add to the menu an action to trigger "create new project" request.
 *
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddNewProjectAction(QMenu* menu, IProjectManager& handler);

/**
 * @brief Add to the menu an action to trigger "close current project" request.
 */
void AddOpenExistingProjectAction(QMenu* menu, IProjectManager& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.
 *
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddSaveCurrentProjectAction(QMenu* menu, IProjectManager& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.

 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddSaveProjectAsAction(QMenu* menu, IProjectManager& handler);

/**
 * @brief Add to the menu entries to open recent projects.
 *
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddRecentProjectActions(QMenu* menu, IProjectManager& handler);

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_HANDLER_UTILS_H_
