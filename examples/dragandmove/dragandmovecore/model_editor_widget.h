/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef DRAGANDMOVE_DRAGANDMOVECORE_MODEL_EDITOR_WIDGET_H
#define DRAGANDMOVE_DRAGANDMOVECORE_MODEL_EDITOR_WIDGET_H

#include <QWidget>
#include <memory>

class QToolBar;
class QAction;

namespace dragandmove
{

class SampleModel;
class ContainerEditorWidget;

//! Shows model content as two table-like trees.
//! It is expected that model has two containers with demo items, left tree shows first container,
//! right tree the second. It is possible to drag-and-drop items between two containers.

class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(SampleModel* model = nullptr, QWidget* parent = nullptr);
  ~ModelEditorWidget() override;

  void SetModel(SampleModel* model);

private:
  void OnUndo();
  void OnRedo();
  void SetupActions();

  QToolBar* m_tool_bar{nullptr};
  ContainerEditorWidget* m_left_widget{nullptr};
  ContainerEditorWidget* m_right_widget{nullptr};
  QAction* m_undo_action{nullptr};
  QAction* m_redo_action{nullptr};
  SampleModel* m_model{nullptr};
};

}  // namespace dragandmove

#endif  // DRAGANDMOVE_DRAGANDMOVECORE_MODEL_EDITOR_WIDGET_H
