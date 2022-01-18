// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_STANDARDITEMS_PLOTTABLEITEMS_H
#define MVVM_STANDARDITEMS_PLOTTABLEITEMS_H

//! @file plottableitems.h
//! Collection of items to plot in QCustomPlot context.

#include "mvvm/model/compounditem.h"

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

// FIXME uncomment
////! Represents basics settings of QPen.

// class MVVM_MODEL_EXPORT PenItem : public CompoundItem
//{
// public:
//   static inline const std::string P_COLOR = "P_COLOR";
//   static inline const std::string P_STYLE = "P_STYLE";
//   static inline const std::string P_WIDTH = "P_WIDTH";

//  PenItem();

//  void setSelected(bool is_selected);

//  std::string colorName() const;
//  void setNamedColor(const std::string& named_color);
//};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_PLOTTABLEITEMS_H
