/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef MVVM_EDITORS_EXTERNALPROPERTYEDITOR_H
#define MVVM_EDITORS_EXTERNALPROPERTYEDITOR_H

#include "mvvm/editors/customeditor.h"

#include <functional>

class QLabel;

namespace mvvm
{

class LostFocusFilter;

//! Custom editor for QVariant based on ExternalProperty.
//! Contains icon, label and button to call external dialog via callback mechanism.

class MVVM_VIEWMODEL_EXPORT ExternalPropertyEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ExternalPropertyEditor(QWidget* parent = nullptr);

  void setCallback(std::function<void(const QVariant&)> callback);

private:
  void buttonClicked();

  void UpdateComponents() override;

  QLabel* m_textLabel;
  QLabel* m_pixmapLabel;
  LostFocusFilter* m_focusFilter;
  std::function<void(const QVariant&)> m_callback;  //! actions to take on clicked button
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_EXTERNALPROPERTYEDITOR_H
