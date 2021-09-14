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

#include "mvvm/viewmodel/presentationitemfactory.h"

#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodelbase/presentationitem.h"

namespace ModelView
{
std::unique_ptr<ViewItemDataInterface> CreateDisplayNamePresentation(SessionItem* item)
{
  auto on_data = [](SessionItem* item, int role)
  {
    return role == Qt::DisplayRole || role == Qt::EditRole
               ? QString::fromStdString(item->GetDisplayName())
               : QVariant();
  };

  auto on_setdata = [](SessionItem* item, QVariant data, int role)
  {
    if (role == Qt::EditRole)
    {
      item->SetDisplayName(data.toString().toStdString());
      return true;
    }
    return false;
  };

  return std::make_unique<PresentationItem<SessionItem>>(item, on_data, on_setdata);
}

std::unique_ptr<ViewItemDataInterface> CreateDataPresentation(SessionItem* item, int role,
                                                              ModelComposer* composer)
{
  // attribute_name copied into lambda

  //  auto on_data = [](SessionItem* item, int role)
  //  {
  //    return role == Qt::DisplayRole || role == Qt::EditRole
  //               ? QString::fromStdString(item->GetAttribute(attribute_name))
  //               : QVariant();
  //  };

  //  auto on_setdata = [attribute_name, composer](item* instruction, QVariant data, int role)
  //  {
  //    if (role == Qt::EditRole)
  //    {
  //      if (composer)
  //        return composer->SetAttribute(instruction, attribute_name,
  //        data.toString().toStdString());
  //      else
  //        return instruction->SetAttribute(attribute_name, data.toString().toStdString());
  //    }
  //    return false;
  //  };

  //  return std::make_unique<ModelView::PresentationItem<instruction_t>>(instruction, on_data,
  //                                                                      on_setdata);
  return {};
}

}  // namespace ModelView
