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

#ifndef MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_
#define MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_

//! @file standardpresentationitems.h
//! Defines standard presentations for sessionitem.

#include <mvvm/model/mvvm_types.h>
#include <mvvm/viewmodelbase/viewitem_data_interface.h>

#include <memory>

namespace mvvm
{

class SessionItem;

/**
 * @brief The SessionItemPresentation class provides ViewItem with access to the SessionItem's data.
 *
 * Serves as an adapter to convert std::variant based data to QVariant. Provides additional Qt's
 * data roles (background, color) that aren't defined in the underlying SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT SessionItemPresentation : public ViewItemDataInterface
{
public:
  QVariant Data(int qt_role) const override;

  bool SetData(const QVariant& data, int qt_role) override;

  bool IsEnabled() const override;

  bool IsEditable() const override;

  SessionItem* GetItem();
  const SessionItem* GetItem() const;

  /**
   * @brief Returns main item data role served by this presentation.
   */
  int GetDataRole() const;

  /**
   * @brief Returns true if given item data role is served by this presentation.
   */
  virtual bool IsValidItemDataRole(int data_role) const;

  /**
   * @brief Returns vector of Qt data roles corresponding.
   */
  virtual QVector<int> GetQtRoles(int data_role) const;

protected:
  explicit SessionItemPresentation(SessionItem* item, int role);

private:
  SessionItem* m_item{nullptr};  // the item to present
  int m_data_role{0};            // the main item's data role to present
};

/**
 * @brief The LabelPresentationItem class shows the label associated with the given item.
 *
 * Item itself is not used directly. It might be used by ViewModelController to find
 * corresponding views. The label is non-editable, and it is not possible to set any other data to
 * it.
 */
class MVVM_VIEWMODEL_EXPORT LabelPresentationItem : public SessionItemPresentation
{
public:
  explicit LabelPresentationItem(SessionItem* item, const std::string& label = {});

  QVariant Data(int qt_role) const override;

private:
  std::string m_label;
};

/**
 * @brief The DataPresentationItem class represents a data role of SessionItem.
 */
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

/**
 * @brief The DisplayNamePresentationItem class represents non-editable display role of SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT DisplayNamePresentationItem : public SessionItemPresentation
{
public:
  explicit DisplayNamePresentationItem(SessionItem* item);

  QVariant Data(int qt_role) const override;
};

/**
 * @brief The EditableDisplayNamePresentationItem class represents editable display role of
 * SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT EditableDisplayNamePresentationItem : public DataPresentationItem
{
public:
  explicit EditableDisplayNamePresentationItem(SessionItem* item);

  QVariant Data(int qt_role) const override;
};

/**
 * @brief The FixedDataPresentationItem class shows any type of QVariant based data associated with
 * given qt_role and given item.
 *
 * The item itself is not used directly. It might be used by ViewModelController to find
 * corresponding views.
 *
 * The data is fixed and has no connection with the original item. It is used to show any types of
 * icons, labels and background color in a cell associated with the item.
 */
class MVVM_VIEWMODEL_EXPORT FixedDataPresentationItem : public SessionItemPresentation
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param item Reference item.
   * @param fixed_data Map of qt roles associated with the data
   */
  FixedDataPresentationItem(SessionItem* item, std::map<int, QVariant> fixed_data);

  QVariant Data(int qt_role) const override;

  bool SetData(const QVariant& data, int qt_role) override;

  QVector<int> GetQtRoles(int data_role) const override;

private:
  std::map<int, QVariant> m_data;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARD_PRESENTATION_ITEMS_H_
