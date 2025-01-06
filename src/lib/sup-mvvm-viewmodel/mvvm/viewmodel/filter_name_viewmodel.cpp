/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "filter_name_viewmodel.h"

namespace mvvm
{

FilterNameViewModel::FilterNameViewModel(QObject *parent) : QSortFilterProxyModel(parent)
{
  setRecursiveFilteringEnabled(true);
}

void FilterNameViewModel::SetPattern(const QString &pattern)
{
  m_pattern = pattern.toLower();
  invalidateFilter();
}

bool FilterNameViewModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  if (m_pattern.isEmpty())
  {
    return true;
  }

  const QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

  return sourceModel()->data(index0).toString().toLower().contains(m_pattern);
}

}  // namespace mvvm
