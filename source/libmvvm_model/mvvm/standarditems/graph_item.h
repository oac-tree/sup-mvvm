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

#ifndef MVVM_STANDARDITEMS_GRAPH_ITEM_H_
#define MVVM_STANDARDITEMS_GRAPH_ITEM_H_

#include <mvvm/model/compound_item.h>

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
  static inline const std::string Type = "Graph";
  static inline const std::string kLink = "kLink";
  static inline const std::string kDisplayed = "kDisplayed";

  explicit GraphItem(const std::string& model_type = Type);

  void SetDataItem(const Data1DItem* item);

  void SetFromGraphItem(const GraphItem* graph_item);

  Data1DItem* GetDataItem() const;

  std::vector<double> GetBinCenters() const;

  std::vector<double> GetValues() const;

  std::vector<double> GetErrors() const;

  std::string GetNamedColor() const;
  void SetNamedColor(const std::string& named_color);

  PenItem* GetPenItem() const;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_GRAPH_ITEM_H_
