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

#ifndef MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
#define MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The AllItemsViewModel class is a view model to show the whole content of SessionModel in
 * Qt trees.
 *
 * Represents a two-column tree with label/data, with one-to-one child/parent correspondence as in
 the
 * original SessionModel. All visible items, like top-level items and property items, are shown.
 * With an additional flag it is possible to show hidden items too.
 */
class MVVM_VIEWMODEL_EXPORT AllItemsViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit AllItemsViewModel(ISessionModel* model, QObject* parent = nullptr,
                             bool show_hidden = false);
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
