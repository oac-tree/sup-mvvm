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

#ifndef MVVM_VIEWMODEL_VARIANT_CONVERTER_H_
#define MVVM_VIEWMODEL_VARIANT_CONVERTER_H_

//! @file variant_converter.h
//! Collection of utility functions to convert variant_t to QVariant and back.

#include <mvvm/core/variant.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/viewmodel_export.h>

#include <QVariant>

namespace mvvm
{
//! Constructs QVariant from mvvm variant_t.
MVVM_VIEWMODEL_EXPORT QVariant GetQtVariant(const variant_t& variant);

//! Constructs variant_t from QVariant. Will throw if it is not possible.
MVVM_VIEWMODEL_EXPORT variant_t GetStdVariant(const QVariant& variant);

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VARIANT_CONVERTER_H_
