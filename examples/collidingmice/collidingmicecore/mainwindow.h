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

#ifndef COLLIDINGMICECORE_MAINWINDOW_H
#define COLLIDINGMICECORE_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class QGraphicsScene;
class QGraphicsView;
class QTimer;
class QAction;
class QSlider;
class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
}

namespace collidingmice
{

class MouseModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

private:
  void CreateCentralWidget();
  void InitScene();
  void InitToolbar();

  /**
   * @brief Initialize File menu.
   */
  void InitMenu();

  /**
   * @brief Populates scene with initial mouse content.
   */
  void PopulateScene();

  QGraphicsScene* m_graphics_scene{nullptr};
  QGraphicsView* m_graphics_view{nullptr};
  QTimer* m_timer{nullptr};
  QSlider* m_slider{nullptr};
  std::unique_ptr<MouseModel> m_mouse_model;

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace collidingmice
#endif  // COLLIDINGMICECORE_MAINWINDOW_H
