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

#include "custom_editor_factory.h"

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/editors/string_completer_editor.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

namespace customeditors
{

CustomEditorFactory::CustomEditorFactory(const string_list_func_t &string_list_func)
    : m_string_list_func(string_list_func)
{
}

mvvm::editor_t CustomEditorFactory::CreateEditor(const QModelIndex &index) const
{
  auto item = mvvm::utils::ItemFromProxyIndex(index);

  if (item->GetEditorType() == mvvm::constants::kStringCompleterEditorType)
  {
    return std::make_unique<mvvm::StringCompleterEditor>(m_string_list_func);
  }

  if (item->GetEditorType() == mvvm::constants::kComboCompleterEditorType)
  {
    return std::make_unique<mvvm::StringCompleterComboEditor>(m_string_list_func);
  }

  return mvvm::DefaultEditorFactory::CreateEditor(index);
}

}  // namespace customeditors
