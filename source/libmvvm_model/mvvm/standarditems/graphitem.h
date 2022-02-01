/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STANDARDITEMS_GRAPHITEM_H
#define MVVM_STANDARDITEMS_GRAPHITEM_H

#include "mvvm/model/compounditem.h"

namespace mvvm
{

class Data1DItem;
class PenItem;

//! One-dimensional graph representation of Data1DItem.
//! Contains plot properties (i.e. color, line type etc) and link to Data1DItem, which will provide
//! actual data to plot. GraphItem is intended for plotting only via GraphViewportItem.

class MVVM_MODEL_EXPORT GraphItem : public CompoundItem
{
public:
  static inline const std::string kType = "Type";
  static inline const std::string P_LINK = "P_LINK";
  static inline const std::string P_GRAPH_TITLE = "P_GRAPH_TITLE";
  static inline const std::string P_PEN = "P_PEN";
  static inline const std::string P_DISPLAYED = "P_DISPLAYED";

  explicit GraphItem(const std::string& model_type = Type);

  void setDataItem(const Data1DItem* item);

  void setFromGraphItem(const GraphItem* graph_item);

  Data1DItem* dataItem() const;

  std::vector<double> binCenters() const;

  std::vector<double> binValues() const;

  std::vector<double> binErrors() const;

  std::string colorName() const;
  void setNamedColor(const std::string& named_color);

  PenItem* penItem() const;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_GRAPHITEM_H
