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

#include "layout_utils.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QLayoutItem>
#include <QWidget>

namespace
{
void Remove(QGridLayout* layout, int row, int column, bool deleteWidgets);
void DeleteChildWidgets(QLayoutItem* item);
}  // namespace

namespace mvvm
{

void ClearLayout(QLayout* layout, bool deleteWidgets)
{
  if (!layout)
    return;

  while (QLayoutItem* item = layout->takeAt(0))
  {
    if (deleteWidgets)
    {
      delete item->widget();
    }
    if (QLayout* childLayout = item->layout())
    {
      ClearLayout(childLayout, deleteWidgets);
    }
    delete item;
  }
}

/**
 * Removes all layout items on the given row from the given grid
 * layout. If deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the row itself from the grid
 * layout, as this isn't possible (i.e. the rowCount() and row indices
 * will stay the same after this function has been called).
 */

void RemoveRow(QGridLayout* layout, int row, bool deleteWidgets)
{
  Remove(layout, row, -1, deleteWidgets);
  layout->setRowMinimumHeight(row, 0);
  layout->setRowStretch(row, 0);
}

/**
 * Removes all layout items on the given column from the given grid
 * layout. If deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the column itself from the grid
 * layout, as this isn't possible (i.e. the columnCount() and column
 * indices will stay the same after this function has been called).
 */

void RemoveColumn(QGridLayout* layout, int column, bool deleteWidgets)
{
  Remove(layout, -1, column, deleteWidgets);
  layout->setColumnMinimumWidth(column, 0);
  layout->setColumnStretch(column, 0);
}

void ClearGridLayout(QGridLayout* layout, bool deleteWidgets)
{
  for (int i_row = 0; i_row < layout->rowCount(); ++i_row)
  {
    RemoveRow(layout, i_row, deleteWidgets);
  }
}

QWidget* CreatePlaceHolder()
{
  auto result = new QWidget;
  result->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  return result;
}

}  // namespace mvvm

namespace
{

/**
 * Helper function. Removes all layout items within the given layout
 * which either span the given row or column. If deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */

void Remove(QGridLayout* layout, int row, int column, bool deleteWidgets)
{
  // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
  for (int i = layout->count() - 1; i >= 0; i--)
  {
    int r, c, rs, cs;
    layout->getItemPosition(i, &r, &c, &rs, &cs);
    if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column))
    {
      // This layout item is subject to deletion.
      QLayoutItem* item = layout->takeAt(i);
      if (deleteWidgets)
      {
        DeleteChildWidgets(item);
      }
      delete item;
    }
  }
}

/**
 * Helper function. Deletes all child widgets of the given layout item.
 */

void DeleteChildWidgets(QLayoutItem* item)
{
  if (item->layout())
  {
    // Process all child items recursively.
    for (int i = 0; i < item->layout()->count(); i++)
    {
      DeleteChildWidgets(item->layout()->itemAt(i));
    }
  }
  item->widget()->deleteLater();
}

}  // namespace
