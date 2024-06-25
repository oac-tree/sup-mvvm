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

#ifndef MVVM_STANDARDITEMS_LINE_SERIES_DATA_ITEM_H_
#define MVVM_STANDARDITEMS_LINE_SERIES_DATA_ITEM_H_

#include <mvvm/model/compound_item.h>

#include <vector>

namespace mvvm
{

class PointItem;

/**
 * @brief The LineSeriesDataItem provides an access for waveform data information.
 */
class MVVM_MODEL_EXPORT LineSeriesDataItem : public CompoundItem
{
public:
  static inline const std::string Type = "LineSeriesData";

  LineSeriesDataItem();

  using CompoundItem::CompoundItem;

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  /**
   * @brief Returns vector of (x,y) points representing a waveform.
   */
  std::vector<std::pair<double, double>> GetWaveform() const;

  /**
   * @brief Set waveform according to data provided.
   *
   * All previous points will be erased.
   */
  void SetWaveform(const std::vector<std::pair<double, double>>& data);

  /**
   * @brief Clears all points in the waveform.
   */
  void Clear();
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_
