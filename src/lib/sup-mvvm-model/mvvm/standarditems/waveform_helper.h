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

#ifndef MVVM_STANDARDITEMS_WAVEFORM_HELPER_H_
#define MVVM_STANDARDITEMS_WAVEFORM_HELPER_H_

#include <string>
#include <vector>

namespace mvvm
{

using waveform_data_t = std::vector<std::pair<double, double>>;

namespace constants
{
const int kDefaultLineWidth = 2;
const std::string kDefaultLineColor("#209fdf");  // between royal blue and steel blue
}  // namespace constants

/**
 * @brief Make pair of vectors instead of vector of pairs.
 */
std::pair<std::vector<double>, std::vector<double>> GetPairOfVectors(const waveform_data_t& data);

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_WAVEFORM_HELPER_H_
