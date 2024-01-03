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

#ifndef CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
#define CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QTabWidget;

namespace celleditors
{

class SampleModel;
class PropertyPanel;

//! Widget with two tabs for different property set.

class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(SampleModel* model = nullptr, QWidget* parent = nullptr);

  void SetModel(SampleModel* model);

private:
  QTabWidget* m_tab_widget{nullptr};
  PropertyPanel* m_panel0{nullptr};
  PropertyPanel* m_panel1{nullptr};
  PropertyPanel* m_panel2{nullptr};
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
