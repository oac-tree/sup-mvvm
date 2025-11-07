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

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace mvvm::test
{

namespace
{

class BaseImpl
{
public:
  explicit BaseImpl(std::string item_type) : m_type(std::move(item_type)) {}
  ~BaseImpl() = default;

  BaseImpl(const BaseImpl&) = delete;
  BaseImpl& operator=(const BaseImpl&) = delete;
  BaseImpl(BaseImpl&&) = delete;
  BaseImpl& operator=(BaseImpl&&) = delete;

  std::unique_ptr<BaseImpl> Clone() const { return std::make_unique<BaseImpl>(m_type); }

  std::string GetType() const { return m_type; }

private:
  std::string m_type;
};

class Base
{
public:
  Base() : m_impl(std::make_unique<BaseImpl>("Base")) {}
  explicit Base(const std::string& item_type) : m_impl(std::make_unique<BaseImpl>(item_type)) {}
  explicit Base(std::unique_ptr<BaseImpl> impl) : m_impl(std::move(impl)) {}
  virtual ~Base() = default;

  Base(Base&&) = delete;
  Base& operator=(Base&&) = delete;

  std::string GetType() const { return m_impl->GetType(); }

  virtual std::unique_ptr<Base> Clone() const { return std::make_unique<Base>(m_impl->Clone()); };

  virtual std::string GetString() const { return "Base"; }

protected:
  Base(const Base& other) : m_impl(other.m_impl->Clone()) {}
  // Base& operator=(const Base&) { throw std::runtime_error("Not implemented"); }
  Base& operator=(const Base&)  = delete;

private:
  std::unique_ptr<BaseImpl> m_impl;
};

class Derived : public Base
{
public:
  Derived() : Base("Derived") {}

  std::unique_ptr<Base> Clone() const override { return std::make_unique<Derived>(*this); };

  std::string GetString() const override { return "Derived"; }
};

}  // namespace

class ExperimentalCloneTest : public ::testing::Test
{
};

TEST_F(ExperimentalCloneTest, InitialState)
{
  Derived abc;
  auto cloned = abc.Clone();

  EXPECT_EQ(cloned->GetType(), "Derived");
  const Base* derived_ptr = cloned.get();

  EXPECT_EQ(derived_ptr->GetType(), "Derived");
  EXPECT_EQ(derived_ptr->GetString(), "Derived");
}

}  // namespace mvvm::test
