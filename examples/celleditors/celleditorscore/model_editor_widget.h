/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
#define CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_

#include <QWidget>

class QTabWidget;

namespace celleditors
{

class SampleModel;

/**
 * @brief The ModelEditorWidget class contains several tabs with PropertyPanels pointing to
 * different containers.
 */
class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(SampleModel* model = nullptr, QWidget* parent = nullptr);

  void SetModel(SampleModel* model);

private:
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_MODEL_EDITOR_WIDGET_H_
