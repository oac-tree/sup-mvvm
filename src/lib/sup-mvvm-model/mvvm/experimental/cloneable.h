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

#ifndef MVVM_EXPERIMENTAL_CLONEABLE_H_
#define MVVM_EXPERIMENTAL_CLONEABLE_H_

#include <memory>

namespace experimental
{

class CloneableInterface
{
public:
  virtual ~CloneableInterface() {}

  std::unique_ptr<CloneableInterface> Clone() const
  {
    return std::unique_ptr<CloneableInterface>(this->CloneImpl());
  }

private:
  virtual CloneableInterface* CloneImpl() const = 0;
};

template <typename Derived, typename Base>
class Cloneable : public Base
{
public:
  std::unique_ptr<Derived> Clone() const
  {
    return std::unique_ptr<Derived>(static_cast<Derived*>(this->CloneImpl()));
  }

private:
  virtual Cloneable* CloneImpl() const override
  {
    return new Derived(*static_cast<const Derived*>(this));
  }
};

class MyItem : public Cloneable<MyItem, CloneableInterface>
{
public:
  MyItem() {}
};

class MyDerivedItem : public Cloneable<MyDerivedItem, MyItem>
{
public:
  MyDerivedItem() {}
};

class MyDerivedItemV2 : public MyItem
{
public:
  MyDerivedItemV2() {}
};

}  // namespace experimental

#endif  // MVVM_EXPERIMENTAL_CLONEABLE_H_
