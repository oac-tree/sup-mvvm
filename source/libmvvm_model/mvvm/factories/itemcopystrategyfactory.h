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

#ifndef MVVM_INTERFACES_ITEMCOPYSTRATEGYFACTORY_H
#define MVVM_INTERFACES_ITEMCOPYSTRATEGYFACTORY_H

#include "mvvm/interfaces/itemcopystrategyinterface.h"

#include <memory>

namespace mvvm
{
class ItemFactoryInterface;

//! Returns default strategy for item copy (deep copying, all identifiers are regenerated to stay
//! unique).
MVVM_MODEL_EXPORT std::unique_ptr<ItemCopyStrategyInterface> CreateItemCopyStrategy(
    const ItemFactoryInterface* factory);

//! Returns default strategy for item clone (deep copying, all identifiers stays the same).
MVVM_MODEL_EXPORT std::unique_ptr<ItemCopyStrategyInterface> CreateItemCloneStrategy(
    const ItemFactoryInterface* factory);

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEMCOPYSTRATEGYFACTORY_H
