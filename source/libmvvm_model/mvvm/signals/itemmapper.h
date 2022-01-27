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

#ifndef MVVM_SIGNALS_ITEMMAPPER_H
#define MVVM_SIGNALS_ITEMMAPPER_H

#include "mvvm/interfaces/itemeventsubstriberinterface.h"

namespace mvvm
{

class SessionItem;

//! Provides notifications on various changes for a specific item. ItemMapper listens to signals
//! coming from the model and processes only signals related to the given item. Notifies all
//! interested subscribers about things going with the item and its relatives.

class ItemMapper : public ItemEventSubscriberInterface
{
public:


};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEMMAPPER_H
