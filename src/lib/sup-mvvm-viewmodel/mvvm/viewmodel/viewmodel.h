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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_H_
#define MVVM_VIEWMODEL_VIEWMODEL_H_

#include <mvvm/viewmodelbase/viewmodel_base.h>

namespace mvvm
{

class SessionItem;
class ViewItem;
class AbstractViewModelController;
class ISessionModel;

/**
 * @brief The ViewModel class represents the content of SessionModel in Qt's trees and tables.
 *
 * When ViewModel is looking at some SessionModel, its invisible root ViewItem is looking at the
 * root item of SessionModel. It is possible to change root SessionItem, thus presenting various
 * parts of the SessionModel with the same view.
 *
 * ViewModel doesn't have its own logic and needs ViewModelController to listen for SessionModel
 * changes. It extends ViewModelBase API with methods to convert SessionItem to indexes and back.
 */
class MVVM_VIEWMODEL_EXPORT ViewModel : public ViewModelBase
{
  Q_OBJECT

public:
  explicit ViewModel(QObject* parent = nullptr);
  ~ViewModel() override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  /**
   * @brief Returns SessionModel which is currently presented.
   */
  const ISessionModel* GetModel() const;

  /**
   * @brief Sets the model to present.
   */
  void SetModel(ISessionModel* model);

  /**
   * @brief Get SessionItem playing the role of root item (const version).
   */
  const SessionItem* GetRootSessionItem() const;

  /**
   * @brief Get SessionItem playing the role of root item (non-const version).
   */
  SessionItem* GetRootSessionItem();

  /**
   * @brief Sets the item to be our root SessionItem.
   *
   * This will regenerate viewmodel. Invisible root ViewItem will be pointing to a given
   * SessionItem.
   */
  void SetRootSessionItem(SessionItem* item);

  /**
   * @brief Returns SessionItem corresponding to the given index (const version).
   */
  const SessionItem* GetSessionItemFromIndex(const QModelIndex& index) const;

  /**
   * @brief Returns SessionItem corresponding to the given index (non-const version).
   */
  SessionItem* GetSessionItemFromIndex(const QModelIndex& index);

  /**
   * @brief Returns view item corresponding to the given index (const version).
   */
  const ViewItem* GetViewItemFromIndex(const QModelIndex& index) const;

  /**
   * @brief Returns view item corresponding to the given index (non-const version).
   */
  ViewItem* GetViewItemFromIndex(const QModelIndex& index);

  /**
   * @brief Returns vector of all ViewItem's representing given SessionItem.
   *
   * Expensive call.
   */
  std::vector<const ViewItem*> FindViews(const mvvm::SessionItem* item) const;

  /**
   * @brief Returns index list for all cells pesenting given item.
   *
   * Expensive call.
   */
  QModelIndexList GetIndexOfSessionItem(const SessionItem* item) const;

  /**
   * @brief Returns internal SessionModel controller.
   */
  AbstractViewModelController* Controller();

protected:
  /**
   * @brief Set internal controller for SessionModel listening.
   *
   * Intended to be used from c-tors of derived classes.
   */
  void SetController(std::unique_ptr<AbstractViewModelController> controller);

private:
  /**
   * @brief Validates that controller was set, throws if it wasn't the case.
   */
  void ValidateController() const;
  std::unique_ptr<AbstractViewModelController> m_controller;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_H_
