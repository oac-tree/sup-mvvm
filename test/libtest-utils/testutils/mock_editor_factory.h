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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_EDITOR_FACTORY_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_EDITOR_FACTORY_H_

#include "mvvm/providers/i_editor_factory.h"

#include <gmock/gmock.h>

#include <QWidget>

namespace mvvm::test
{

/**
 * @brief The MockEditorFactory class to test classes relying on IEditorFactory interface.
 */
class MockEditorFactory : public IEditorFactory
{
public:
  ~MockEditorFactory() override;

  MOCK_METHOD(editor_t, CreateEditor, (const QModelIndex& index), (const, override));
  MOCK_METHOD(editor_t, CreateEditor, (const SessionItem* item), (const, override));
};

/**
 * @brief The EditorFactoryDecorator intended to wrap MockEditorFactory to avoid passing an
 * ownerhship to mock object.
 */
class EditorFactoryDecorator : public IEditorFactory
{
public:
  explicit EditorFactoryDecorator(IEditorFactory* decoratee);
  ~EditorFactoryDecorator() override;

  editor_t CreateEditor(const QModelIndex& index) const override;

  editor_t CreateEditor(const SessionItem* item) const override;

private:
  IEditorFactory* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_EDITOR_FACTORY_H_
