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

#include "file_based_user_interactor.h"

#include <QFileDialog>

namespace mvvm
{

namespace
{

QString GetFilter(const QString& application_type)
{
  return application_type.isEmpty()
             ? QString("XML files (*.xml *.XML)")
             : QString("XML files for %1 (*.xml *.XML)").arg(application_type);
}

}  // namespace

FileBasedUserInteractor::FileBasedUserInteractor(const QString& application_type, QWidget* parent)
    : AbstractProjectUserInteractor(application_type, parent)
{
}

std::string FileBasedUserInteractor::GetNewProjectPathImpl() const
{
  auto workdir = QString::fromStdString(GetCurrentWorkdir());
  auto file_name = QFileDialog::getSaveFileName(GetParent(), "Save File", workdir,
                                                GetFilter(GetApplicationType()));

  return file_name.toStdString();
}

std::string FileBasedUserInteractor::GetExistingProjectPathImpl() const
{
  auto workdir = QString::fromStdString(GetCurrentWorkdir());
  auto file_name = QFileDialog::getOpenFileName(GetParent(), "Open file", workdir,
                                                GetFilter(GetApplicationType()));

  return file_name.toStdString();
}

}  // namespace mvvm
