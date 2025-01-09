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

#ifndef MVVM_EDITORS_CUSTOM_EDITOR_H_
#define MVVM_EDITORS_CUSTOM_EDITOR_H_

#include <mvvm/core/variant.h>
#include <mvvm/viewmodel_export.h>

#include <QVariant>
#include <QWidget>

namespace mvvm
{

//! Base class for all custom variant editors.

class MVVM_VIEWMODEL_EXPORT CustomEditor : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QVariant value MEMBER m_data READ GetData WRITE SetData NOTIFY dataChanged USER true)

public:
  explicit CustomEditor(QWidget* parent_widget = nullptr);

  QVariant GetData() const;

  virtual bool IsPersistent() const;

  void SetData(const QVariant& data);

signals:
  //! Emmits signal when data was changed in an editor.
  void dataChanged(QVariant value);

protected:
  void SetDataIntern(const QVariant& data);
  //! Should update widget components from m_data, if necessary.
  virtual void UpdateComponents() = 0;

private:
  QVariant m_data;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_CUSTOM_EDITOR_H_
