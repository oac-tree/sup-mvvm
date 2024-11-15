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

#ifndef MVVM_WIDGETS_ADJUSTING_SCROLL_AREA_H_
#define MVVM_WIDGETS_ADJUSTING_SCROLL_AREA_H_

#include <mvvm/view_export.h>

#include <QScrollArea>

namespace mvvm
{

/**
 * @brief The AdjustingScrollArea class modifies the standard scroll area, allowing widgets with
 * dynamic layouts to occupy all available space.
 */
class MVVM_VIEW_EXPORT AdjustingScrollArea : public QScrollArea
{
  Q_OBJECT

public:
  explicit AdjustingScrollArea(QWidget* parent = nullptr);

  void SetWidget(QWidget* widget);

  QSize sizeHint() const override;

private:
  bool eventFilter(QObject* object, QEvent* event) override;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ADJUSTING_SCROLL_AREA_H_
