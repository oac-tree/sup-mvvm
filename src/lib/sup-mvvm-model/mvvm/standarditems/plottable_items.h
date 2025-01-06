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

#ifndef MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_
#define MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_

//! @file
//! Collection of items to plot in QCustomPlot context.

#include <mvvm/model/compound_item.h>

namespace mvvm
{

/**
 * @brief The TextItem class represent text item on plot.
 */
class MVVM_MODEL_EXPORT TextItem : public CompoundItem
{
public:
  static inline const std::string Type = "Text";

  TextItem();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetText() const;
  void SetText(const std::string& value);

  std::string GetFont() const;
  void SetFont(const std::string& value);

  int GetPointSize() const;
  void SetPointSize(int value);
};

/**
 * @brief The PenItem class represents basics settings of QPen.
 */
class MVVM_MODEL_EXPORT PenItem : public CompoundItem
{
public:
  static inline const std::string Type = "Pen";
  static inline const std::string kColor = "kColor";
  static inline const std::string kStyle = "kStyle";
  static inline const std::string kWidth = "kWidth";

  PenItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Checks if the pen style corresponds to the selected object.
   */
  bool IsSelected() const;

  /**
   * @brief Set pen style as if the object is selected.
   */
  void SetSelected(bool is_selected);

  /**
   * @brief Returns named color.
   *
   * @see SetNamedColor()
   */
  std::string GetNamedColor() const;

  /**
   * @brief Sets named color.
   *
   * The color name can be the one from https://www.w3.org/TR/css-color-3/#svg-color database, like
   * "red" or "deeppink",
   *
   * The color can also be a string of "#RRGGBB" format, e.g. #008000 is a green with
   * (read:0, green:128, blue:0)
   */
  void SetNamedColor(const std::string& named_color);

  std::string GetStyle() const;
  void SetStyle(const std::string& style);

  int GetStyleIndex() const;

  int GetWidth() const;
  void SetWidth(int value);
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_PLOTTABLE_ITEMS_H_
