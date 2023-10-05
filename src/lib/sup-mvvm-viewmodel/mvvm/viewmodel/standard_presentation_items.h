/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_
#define MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_

//! @file standardpresentationitems.h
//! Defines standard presentations for sessionitem.

#include <mvvm/viewmodelbase/viewitem_data_interface.h>
#include <mvvm/model/mvvm_types.h>

#include <memory>

namespace mvvm
{

class SessionItem;

//! @brief Defines base class to provide ViewItem with access to the SessionItem's data.

//! Serves as an adapter to convert std::variant based data to QVariant. Provides additional Qt's
//! data roles (background, color) that aren't defined in the underlying SessionItem.

class MVVM_VIEWMODEL_EXPORT SessionItemPresentation : public ViewItemDataInterface
{
public:
  QVariant Data(int qt_role) const override;

  bool SetData(const QVariant& data, int qt_role) override;

  bool IsEnabled() const override;

  bool IsEditable() const override;

  SessionItem* GetItem();
  const SessionItem* GetItem() const;

  int GetDataRole() const;

protected:
  explicit SessionItemPresentation(SessionItem* item, int role);

private:
  SessionItem* m_item{nullptr};  // the item to present
  int m_data_role{0};            // main role to present
};

//! Presentation to show the label associated with the given context.
//! Context itself is not used directly. It might be used by ViewModelController to find
//! corresponding views.

class MVVM_VIEWMODEL_EXPORT LabelPresentationItem : public SessionItemPresentation
{
public:
  explicit LabelPresentationItem(SessionItem* item, const std::string& label = {});

  QVariant Data(int qt_role) const override;

private:
  std::string m_label;
};

//! Represents a data role of SessionItem.

class MVVM_VIEWMODEL_EXPORT DataPresentationItem : public SessionItemPresentation
{
public:
  /**
   * @brief Main c-tor of the DataPresentationItem.
   *
   * @param item An item those data will be presented.
   * @param role The data role of SessionItem
   */
  explicit DataPresentationItem(SessionItem* item, int role = DataRole::kData);

  QVariant Data(int qt_role) const override;

  bool SetData(const QVariant& data, int qt_role) override;
};

//! Represents non-editable display role of SessionItem.

class MVVM_VIEWMODEL_EXPORT DisplayNamePresentationItem : public SessionItemPresentation
{
public:
  explicit DisplayNamePresentationItem(SessionItem* item);

  QVariant Data(int qt_role) const override;
};

//! Represents editable display role of SessionItem.

class MVVM_VIEWMODEL_EXPORT EditableDisplayNamePresentationItem : public DataPresentationItem
{
public:
  explicit EditableDisplayNamePresentationItem(SessionItem* item);
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_
