/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_VIEWMODECONTROLLER_H
#define MVVM_VIEWMODEL_VIEWMODECONTROLLER_H

#include "mvvm/interfaces/modeleventlistenerinterface.h"

namespace ModelView
{

class MVVM_MODEL_EXPORT ViewModelController : public ModelEventListenerInterface
{
public:
  ~ViewModelController();

  void OnAboutToInsertItem(SessionItem *parent, const TagIndex &tag_index);

  void OnItemInserted(SessionItem *parent, const TagIndex &tag_index);

  void OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index);

  void OnItemRemoved(SessionItem *parent, const TagIndex &tag_index);

  void OnDataChanged(SessionItem *item, int role);
};


}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_VIEWMODECONTROLLER_H
