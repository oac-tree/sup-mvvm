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

#ifndef CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
#define CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace celleditors
{

//! Demo item demonstrating basing supported properties.

class BasicPropertyItem : public mvvm::CompoundItem
{
public:
  BasicPropertyItem();
};

//! Demo item showing all kinds of integer-like properties.

class AllIntPropertyItem : public mvvm::CompoundItem
{
public:
  AllIntPropertyItem();
};

//! Demo item showing all kinds of float properties.

class FloatPropertyItem : public mvvm::CompoundItem
{
public:
  FloatPropertyItem();
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
