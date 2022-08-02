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

#ifndef DRAGANDMOVECORE_SAMPLEITEMS_H
#define DRAGANDMOVECORE_SAMPLEITEMS_H

//! @file sampleitems.h
//! Collection of items for our application model.

#include <mvvm/model/compound_item.h>
#include <string>

namespace DragAndMove {

//! Demo item which demonstrating supported properties.

class DemoItem : public mvvm::CompoundItem {
public:
    static inline const std::string P_BOOL_PROPERTY = "P_BOOL_PROPERTY";
    static inline const std::string P_INTEGER_PROPERTY = "P_INTEGER_PROPERTY";
    static inline const std::string P_STRING_PROPERTY = "P_STRING_PROPERTY";
    static inline const std::string P_DOUBLE_PROPERTY = "P_DOUBLE_PROPERTY";
    static inline const std::string P_COLOR_PROPERTY = "P_COLOR_PROPERTY";
    static inline const std::string P_COMBO_PROPERTY = "P_COMBO_PROPERTY";

    DemoItem();
};

//! Container to hold demo items

class DemoContainerItem : public mvvm::CompoundItem {
public:
    static inline const std::string T_ITEMS = "T_ITEMS";

    DemoContainerItem();
};

} // namespace DragAndMove

#endif // DRAGANDMOVECORE_SAMPLEITEMS_H
