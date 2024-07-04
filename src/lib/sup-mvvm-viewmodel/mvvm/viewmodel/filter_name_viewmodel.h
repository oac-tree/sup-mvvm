/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef MVVM_PROVIDERS_FILTER_NAME_VIEWMODEL_H_
#define MVVM_PROVIDERS_FILTER_NAME_VIEWMODEL_H_

#include <QSortFilterProxyModel>

namespace mvvm
{

/**
 * @brief The FilterNameViewModel class is a simple proxy model which provides fuzzy filtering on
 * display name of the first column.
 */
class FilterNameViewModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit FilterNameViewModel(QObject* parent = nullptr);

  void SetPattern(const QString& pattern);

  bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
  QString m_pattern;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_FILTER_NAME_VIEWMODEL_H_
