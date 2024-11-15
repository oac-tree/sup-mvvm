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

#include "abstract_project_user_interactor.h"

#include <mvvm/project/project_context.h>
#include <mvvm/utils/file_utils.h>

#include <QMessageBox>
#include <map>

namespace mvvm
{

namespace
{

/**
 * @brief Creates a map to convert standard Qt answers to what ProjectManager expects.
 */
std::map<QMessageBox::StandardButton, SaveChangesAnswer> CreateMapOfAnswers()
{
  std::map<QMessageBox::StandardButton, SaveChangesAnswer> result = {
      {QMessageBox::Save, SaveChangesAnswer::kSave},
      {QMessageBox::Discard, SaveChangesAnswer::kDiscard},
      {QMessageBox::Cancel, SaveChangesAnswer::kCancel}};
  return result;
}

}  // namespace

AbstractProjectUserInteractor::AbstractProjectUserInteractor(const QString &application_type,
                                                             QWidget *parent)
    : m_application_type(application_type), m_parent(parent)
{
}

std::string AbstractProjectUserInteractor::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

void AbstractProjectUserInteractor::SetCurrentWorkdir(const std::string &path)
{
  m_current_workdir = path;
}

std::string AbstractProjectUserInteractor::GetNewProjectPath()
{
  auto result = GetNewProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

std::string AbstractProjectUserInteractor::GetExistingProjectPath()
{
  auto result = GetExistingProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

SaveChangesAnswer AbstractProjectUserInteractor::OnSaveCurrentChangesRequest() const
{
  static auto translate_map = CreateMapOfAnswers();

  QMessageBox msg_box;
  msg_box.setText("The project has been modified.");
  msg_box.setInformativeText("Do you want to save your changes?");
  msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msg_box.setDefaultButton(QMessageBox::Save);
  auto ret = static_cast<QMessageBox::StandardButton>(msg_box.exec());
  return translate_map[ret];
}

void AbstractProjectUserInteractor::SendMessage(const std::string &message) const
{
  QMessageBox msg_box;
  msg_box.setWindowTitle("Can't open the file");
  msg_box.setText("Exception was caught while trying to open the file");
  msg_box.setInformativeText(QString::fromStdString(message));
  msg_box.setIcon(QMessageBox::Warning);
  msg_box.exec();
}

void AbstractProjectUserInteractor::SetUseNativeDialog(bool value)
{
  m_use_native_dialogs = value;
}

bool AbstractProjectUserInteractor::GetUseNativeDialogFlag() const
{
  return m_use_native_dialogs;
}

UserInteractionContext AbstractProjectUserInteractor::CreateContext()
{
  UserInteractionContext result;
  result.existing_path_callback = [this]() { return GetExistingProjectPath(); };
  result.new_path_callback = [this]() { return GetNewProjectPath(); };
  result.answer_callback = [this]() { return OnSaveCurrentChangesRequest(); };
  result.message_callback = [this](const std::string &text) { SendMessage(text); };

  return result;
}

void AbstractProjectUserInteractor::UpdateCurrentWorkdir(const std::string &path)
{
  if (!path.empty())
  {
    // updating mutable variable from const method
    m_current_workdir = utils::GetParentPath(path);
  }
}

QString AbstractProjectUserInteractor::GetApplicationType() const
{
  return m_application_type;
}

QWidget *AbstractProjectUserInteractor::GetParent() const
{
  return m_parent;
}

}  // namespace mvvm
