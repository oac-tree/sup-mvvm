/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROVIDERS_VIEWMODEL_DELEGATE_H_
#define MVVM_PROVIDERS_VIEWMODEL_DELEGATE_H_

#include <mvvm/viewmodel_export.h>

#include <QStyledItemDelegate>
#include <memory>

namespace mvvm
{

class IEditorFactory;
class ICellDecorator;

//! Model delegate to provide editing/painting for custom variants.

class MVVM_VIEWMODEL_EXPORT ViewModelDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit ViewModelDelegate(QObject* parent = nullptr);
  ~ViewModelDelegate() override;

  void setEditorFactory(std::unique_ptr<IEditorFactory> editor_factory);
  void setCellDecoration(std::unique_ptr<ICellDecorator> cell_decoration);

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                        const QModelIndex& index) const override;

  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model,
                    const QModelIndex& index) const override;

  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const override;

  void onCustomEditorDataChanged();

protected:
  void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;

private:
  std::unique_ptr<IEditorFactory> m_editor_factory;
  std::unique_ptr<ICellDecorator> m_cell_decoration;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWMODEL_DELEGATE_H_
