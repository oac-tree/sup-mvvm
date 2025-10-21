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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "plottable_items.h"

#include "editor_constants.h"

#include <mvvm/model/item_limits_helper.h>
#include <mvvm/utils/container_utils.h>

namespace mvvm
{

namespace
{

/**
 * @brief Returns combo property containing pen styles.
 *
 * Folows Qt one-to-one.
 */
ComboProperty GetPenStyleCombo()
{
  static const ComboProperty kPenStyleCombo(
      {"NoPen", "SolidLine", "DashLine", "DotLine", "DashDotLine", "DashDotDotLine"}, "SolidLine");
  return kPenStyleCombo;
}

const int kPenDefaultWidth = 1;
const int kPenMinWidth = 0;
const int kPenMaxWidth = 7;
const int kPenStyleIndexSolid = 1;
const int kPenStyleIndexDashline = 2;
constexpr std::string_view kText = "kText";
constexpr std::string_view kFont = "kFont";
constexpr std::string_view kSize = "kSize";
}  // namespace

TextItem::TextItem() : CompoundItem(GetTypeName<TextItem>())
{
  (void)SetEditable(false);  // to disable editing of on-board data
  (void)AddProperty(kText, "").SetDisplayName("Text");
  (void)AddProperty(kFont, std::string(kDefaultFontFamily)).SetDisplayName("Font");
  (void)AddProperty(kSize, kDefaultPointSize).SetDisplayName("Size");
}

std::unique_ptr<SessionItem> TextItem::Clone() const
{
  return std::make_unique<TextItem>(*this);
}

std::string TextItem::GetText() const
{
  return Property<std::string>(kText);
}

void TextItem::SetText(const std::string &value)
{
  (void)SetProperty(kText, value);
}

std::string TextItem::GetFont() const
{
  return Property<std::string>(kFont);
}

void TextItem::SetFont(const std::string &value)
{
  (void)SetProperty(kFont, value);
}

int TextItem::GetPointSize() const
{
  return Property<int>(kSize);
}

void TextItem::SetPointSize(int value)
{
  (void)SetProperty(kSize, value);
}

// ----------------------------------------------------------------------------
// PenItem
// ----------------------------------------------------------------------------

PenItem::PenItem() : CompoundItem(GetTypeName<PenItem>())
{
  (void)SetEditable(false);  // to disable editing of on-board data
  (void)AddProperty(kColor, "black")
      .SetDisplayName("Color")
      .SetToolTip("Pen color")
      .SetEditorType(constants::kColorEditorType);
  (void)AddProperty(kStyle, GetPenStyleCombo()).SetDisplayName("Style").SetToolTip("Pen style");
  auto &width_property =
      AddProperty(kWidth, kPenDefaultWidth).SetDisplayName("Width").SetToolTip("Pen width");
  SetLimited(kPenMinWidth, kPenMaxWidth, width_property);
}

std::unique_ptr<SessionItem> PenItem::Clone() const
{
  return std::make_unique<PenItem>(*this);
}

bool PenItem::IsSelected() const
{
  return Property<ComboProperty>(kStyle).GetCurrentIndex() == kPenStyleIndexDashline;
}

void PenItem::SetSelected(bool is_selected)
{
  static ComboProperty combo = GetPenStyleCombo();
  combo.SetCurrentIndex(is_selected ? kPenStyleIndexDashline : kPenStyleIndexSolid);
  (void)SetProperty(kStyle, combo);
}

std::string PenItem::GetNamedColor() const
{
  return Property<std::string>(kColor);
}

void PenItem::SetNamedColor(const std::string &named_color)
{
  (void)SetProperty(kColor, named_color);
}

std::string PenItem::GetStyle() const
{
  return Property<ComboProperty>(kStyle).GetValue();
}

void PenItem::SetStyle(const std::string &style)
{
  auto combo = Property<ComboProperty>(kStyle);
  combo.SetValue(style);
  (void)SetProperty(kStyle, combo);
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
  (void)SetProperty(kWidth, value);
}

}  // namespace mvvm
