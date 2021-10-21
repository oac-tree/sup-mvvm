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

#ifndef MVVM_WIDGETS_PROPERTYTREEVIEW_H
#define MVVM_WIDGETS_PROPERTYTREEVIEW_H

#include "mvvm/widgets/itemstreeview.h"

namespace ModelView {

//! Widget holding standard QTreeView and intended for displaying all properties of given
//! SessionItem.

class MVVM_VIEW_EXPORT PropertyTreeView : public ItemsTreeView {
    Q_OBJECT

public:
    PropertyTreeView(QWidget* parent = nullptr);
    ~PropertyTreeView();

    virtual void SetRootSessionItem(SessionItem* item) override;

    void SetItem(SessionItem* item);
};

} // namespace ModelView

#endif // MVVM_WIDGETS_PROPERTYTREEVIEW_H
