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

#ifndef MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_
#define MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_

//! @file plottable_items.h
//! Collection of items to plot in QCustomPlot context.

#include <mvvm/model/compound_item.h>

namespace mvvm
{

//! Represent text item on plot.

class MVVM_MODEL_EXPORT TextItem : public CompoundItem
{
public:
  static inline const std::string Type = "Text";

  TextItem();

  std::string GetText() const;
  void SetText(const std::string& value);

  std::string GetFont() const;
  void SetFont(const std::string& value);

  double GetSize() const;
  void SetSize(double value);
};

//! Represents basics settings of QPen.

class MVVM_MODEL_EXPORT PenItem : public CompoundItem
{
public:
  static inline const std::string Type = "Pen";

  static inline const std::string kColor = "kColor";
  static inline const std::string kStyle = "kStyle";
  static inline const std::string kWidth = "kWidth";

  PenItem();

  bool IsSelected() const;
  void SetSelected(bool is_selected);

  std::string GetNamedColor() const;
  void SetNamedColor(const std::string& named_color);

  std::string GetStyle() const;
  void SetStyle(const std::string& style);

  int GetStyleIndex() const;

  int GetWidth() const;
  void SetWidth(int value);
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_
