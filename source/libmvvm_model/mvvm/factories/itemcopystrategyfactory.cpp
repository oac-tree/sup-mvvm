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

#include "mvvm/factories/itemcopystrategyfactory.h"

#include "mvvm/interfaces/itemcopystrategyinterface.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedataitemconverter.h"

namespace
{

//! Implements copy strategy for SessionItem using TreeDataItemConverter.
class TreeDataItemCopyStrategy : public mvvm::ItemCopyStrategyInterface
{
public:
  explicit TreeDataItemCopyStrategy(const ::mvvm::ItemFactoryInterface* factory,
                                    mvvm::ConverterMode mode)
      : m_converter(std::make_unique<mvvm::TreeDataItemConverter>(factory, mode))
  {
  }

  //! Returns deep copy of given item. All identifiers are regenerated.
  std::unique_ptr<mvvm::SessionItem> CreateCopy(const mvvm::SessionItem* item) const override
  {
    auto tree_data = m_converter->ToTreeData(*item);
    return m_converter->ToSessionItem(*tree_data);
  }

private:
  std::unique_ptr<mvvm::TreeDataItemConverterInterface> m_converter;
};

}  // namespace

namespace mvvm
{

std::unique_ptr<ItemCopyStrategyInterface> CreateItemCopyStrategy(
    const ItemFactoryInterface* factory)
{
  return std::make_unique<TreeDataItemCopyStrategy>(factory, mvvm::ConverterMode::kCopy);
}

std::unique_ptr<ItemCopyStrategyInterface> CreateItemCloneStrategy(
    const ItemFactoryInterface* factory)
{
  return std::make_unique<TreeDataItemCopyStrategy>(factory, mvvm::ConverterMode::kClone);
}

}  // namespace mvvm
