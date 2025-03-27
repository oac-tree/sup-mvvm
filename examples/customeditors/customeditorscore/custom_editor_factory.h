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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_EDITOR_FACTORY_H_
#define CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_EDITOR_FACTORY_H_

#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QStringList>
#include <functional>

namespace customeditors
{

/**
 * @brief The CustomEditorFactory class is a factory to generate cell editors with auto-complete
 * features.
 *
 * It extends mvvm::DefaultEditorFactory.
 */
class CustomEditorFactory : public mvvm::DefaultEditorFactory
{
public:
  using string_list_func_t = std::function<QStringList()>;

  explicit CustomEditorFactory(const string_list_func_t& string_list_func);

  mvvm::editor_t CreateEditor(const QModelIndex& index) const override;

private:
  string_list_func_t m_string_list_func;
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSSCORE_CUSTOM_EDITOR_FACTORY_H_
