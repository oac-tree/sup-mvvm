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

#ifndef FLATVIEW_FLATVIEWCORE_SAMPLE_MODEL_H_
#define FLATVIEW_FLATVIEWCORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace flatview
{

//! Demo item demonstrating supported properties.

class DemoItem : public mvvm::CompoundItem
{
public:
  DemoItem();

  void Activate() override;
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();


};

}  // namespace flatview

#endif  // FLATVIEW_FLATVIEWCORE_SAMPLE_MODEL_H_
