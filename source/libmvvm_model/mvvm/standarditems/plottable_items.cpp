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

#include "mvvm/standarditems/plottable_items.h"

#include "mvvm/standarditems/editor_constants.h"
#include "mvvm/utils/container_utils.h"

namespace
{

//! Following Qt styles.
const mvvm::ComboProperty penStyleCombo({"NoPen", "SolidLine", "DashLine", "DotLine", "DashDotLine",
                                         "DashDotDotLine"},
                                        "SolidLine");
const int pen_default_width = 1;
const int pen_min_width = 0;
const int pen_max_width = 7;
const int penstyle_index_solid = 1;
const int penstyle_index_dashline = 2;

// We do not want to depend from widget_utils.h to get App default font size. Let's stick to
// hardcoded value for the moment, even if on different systems it can be not-optimal.
const double default_title_size = 10;
const std::string default_title_family = "Noto Sans";
}  // namespace

namespace mvvm
{

static inline const std::string kText = "kText";
static inline const std::string kFont = "kFont";
static inline const std::string kSize = "kSize";

TextItem::TextItem() : CompoundItem(Type)
{
  AddProperty(kText, "")->SetDisplayName("Text");
  AddProperty(kFont, default_title_family)->SetDisplayName("Font");
  AddProperty(kSize, default_title_size)->SetDisplayName("Size");
}

std::string TextItem::GetText() const
{
  return Property<std::string>(kText);
}

void TextItem::SetText(const std::string &value)
{
  SetProperty(kText, value);
}

std::string TextItem::GetFont() const
{
  return Property<std::string>(kFont);
}

void TextItem::SetFont(const std::string &value)
{
  SetProperty(kFont, value);
}

double TextItem::GetSize() const
{
  return Property<double>(kSize);
}

void TextItem::SetSize(double value)
{
  SetProperty(kSize, value);
}

// ----------------------------------------------------------------------------
// PenItem
// ----------------------------------------------------------------------------

PenItem::PenItem() : CompoundItem(Type)
{
  AddProperty(kColor, "black")
      ->SetDisplayName("Color")
      ->SetToolTip("Pen color")
      ->SetEditorType(constants::kColorEditorType);
  AddProperty(kStyle, penStyleCombo)->SetDisplayName("Style")->SetToolTip("Pen style");
  AddProperty(kWidth, pen_default_width)
      ->SetDisplayName("Width")
      //         ->SetLimits(RealLimits::limited(pen_min_width, pen_max_width)) FIXME restore
      ->SetToolTip("Pen width");
}

bool PenItem::IsSelected() const
{
  return Property<ComboProperty>(kStyle).GetCurrentIndex() == penstyle_index_dashline;
}

//! Sets style of the pen to represent selected object (dash line).

void PenItem::SetSelected(bool is_selected)
{
  auto combo = penStyleCombo;
  combo.SetCurrentIndex(is_selected ? penstyle_index_dashline : penstyle_index_solid);
  SetProperty(kStyle, combo);
}

//! Returns named color.
//! https://www.w3.org/TR/css-color-3/#svg-color

std::string PenItem::GetNamedColor() const
{
  return Property<std::string>(kColor);
}

//! Sets named color.
//! https://www.w3.org/TR/css-color-3/#svg-color

void PenItem::SetNamedColor(const std::string &named_color)
{
  SetProperty(kColor, named_color);
}

std::string PenItem::GetStyle() const
{
  return Property<ComboProperty>(kStyle).GetValue();
}

void PenItem::SetStyle(const std::string &style)
{
  auto combo = Property<ComboProperty>(kStyle);
  combo.SetValue(style);
  SetProperty(kStyle, combo);
}

int PenItem::GetStyleIndex() const
{
  return Property<ComboProperty>(kStyle).GetCurrentIndex();
}

int PenItem::GetWidth() const
{
  return Property<int>(kWidth);
}

void PenItem::SetWidth(int value)
{
  SetProperty(kWidth, value);
}

}  // namespace mvvm
