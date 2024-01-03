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

#ifndef DRAGANDMOVE_DRAGANDMOVECORE_SAMPLE_MODEL_H_
#define DRAGANDMOVE_DRAGANDMOVECORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace mvvm
{
class SessionItem;
}

namespace dragandmove
{

//! Demo item which demonstrating supported properties.

class DemoItem : public mvvm::CompoundItem
{
public:
  DemoItem();
};

//! Container to hold demo items

class DemoContainerItem : public mvvm::CompoundItem
{
public:
  DemoContainerItem();
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();

  void AppendRandomItem(mvvm::SessionItem* container);

private:
  void PopulateModel();
};

}  // namespace dragandmove

#endif  // DRAGANDMOVE_DRAGANDMOVECORE_SAMPLE_MODEL_H_
