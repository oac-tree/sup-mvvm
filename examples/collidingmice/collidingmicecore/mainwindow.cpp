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

#include "mainwindow.h"

#include "mouse.h"
#include "mousemodel.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMenuBar>
#include <QSlider>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

namespace collidingmice
{

namespace
{
const int msec_update_period = 30;
const int max_slider_value = 100;
}  // namespace

MainWindow::MainWindow()
    : m_graphics_scene(new QGraphicsScene)
    , m_graphics_view(new QGraphicsView(m_graphics_scene))
    , m_timer(new QTimer)
    , m_slider(new QSlider)
    , m_mouse_model(std::make_unique<MouseModel>())
    , m_tree_view(new QTreeView)
    , m_component_provider(
          mvvm::CreateProvider<mvvm::AllItemsViewModel>(m_tree_view, m_mouse_model.get()))
{
  CreateCentralWidget();
  InitScene();
  InitToolbar();
  InitMenu();

  QObject::connect(m_timer, SIGNAL(timeout()), m_graphics_scene, SLOT(advance()));
  m_timer->start(msec_update_period);
}

MainWindow::~MainWindow() = default;

void MainWindow::CreateCentralWidget()
{
  auto central_widget = new QWidget;
  auto layout = new QHBoxLayout;
  layout->addWidget(m_tree_view, 2);
  layout->addWidget(m_graphics_view, 5);
  central_widget->setLayout(layout);
  setCentralWidget(central_widget);
  resize(1200, 900);
  setWindowTitle("Colliding Mice");
}

void MainWindow::InitScene()
{
  m_graphics_scene->setSceneRect(-300, -300, 600, 600);
  m_graphics_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

  m_graphics_view->setRenderHint(QPainter::Antialiasing);
  m_graphics_view->setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
  m_graphics_view->setCacheMode(QGraphicsView::CacheBackground);
  m_graphics_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  m_graphics_view->setDragMode(QGraphicsView::ScrollHandDrag);

  PopulateScene();
}

void MainWindow::InitToolbar()
{
  auto toolbar = addToolBar("toolbar");

  // creates pause/resume button
  auto pause_resume_action = new QAction("Pause/Resume", this);
  auto on_pause_action = [this]()
  {
    if (m_timer->isActive())
    {
      m_timer->stop();
    }
    else
    {
      m_timer->start(msec_update_period);
      m_slider->setValue(max_slider_value);
    }
  };
  connect(pause_resume_action, &QAction::triggered, on_pause_action);
  toolbar->addAction(pause_resume_action);

  toolbar->addSeparator();

  // creates undo/redo slider
  toolbar->addWidget(new QLabel("   Back in time  "));
  m_slider->setRange(0, max_slider_value);
  m_slider->setValue(max_slider_value);
  m_slider->setOrientation(Qt::Horizontal);
  m_slider->setMaximumWidth(300);
  toolbar->addWidget(m_slider);

  auto on_slider_moved = [this](auto value) { m_mouse_model->SetUndoPosition(value); };
  connect(m_slider, &QSlider::sliderMoved, on_slider_moved);

  auto on_slider_pressed = [this]() { m_timer->stop(); };
  connect(m_slider, &QSlider::sliderPressed, on_slider_pressed);
}

void MainWindow::InitMenu()
{
  auto file_menu = menuBar()->addMenu("&File");

  // open file
  auto open_action = new QAction("&Open...", this);
  file_menu->addAction(open_action);
  auto on_open_action = [&]()
  {
    const QString file_name = QFileDialog::getOpenFileName(this);
    if (!file_name.isEmpty())
    {
      m_mouse_model->LoadFromFile(file_name);
      PopulateScene();
    }
  };
  connect(open_action, &QAction::triggered, on_open_action);

  // save file
  auto save_action = new QAction("&Save As...", this);
  file_menu->addAction(save_action);

  auto on_save_action = [&]()
  {
    const QString file_name = QFileDialog::getSaveFileName(this);
    if (!file_name.isEmpty())
    {
      m_mouse_model->SaveToFile(file_name);
    }
  };
  connect(save_action, &QAction::triggered, on_save_action);
}

void MainWindow::PopulateScene()
{
  m_graphics_scene->clear();
  for (auto item : mvvm::utils::GetTopItems<MouseItem>(m_mouse_model.get()))
  {
    m_graphics_scene->addItem(new Mouse(item));
  }
  m_tree_view->expandAll();
}

}  // namespace collidingmice
