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

#ifndef MVVM_EDITORS_ABSTRACT_EDITOR_FACTORY_H_
#define MVVM_EDITORS_ABSTRACT_EDITOR_FACTORY_H_

#include <mvvm/editors/editor_builders.h>
#include <mvvm/interfaces/editor_factory_interface.h>

#include <map>

namespace mvvm
{

//! Abstract editor factory for ViewModelDelegate.
//! Creates cell editors for Qt trees and tables from model index. Cell editor is
//! Qt widget intended for editing DATA role of some SessionItem.

class MVVM_VIEWMODEL_EXPORT AbstractEditorFactory : public EditorFactoryInterface
{
public:
  void RegisterBuilder(const std::string& name, editorbuilder_t builder);

  editorbuilder_t FindBuilder(const std::string& name) const;

private:
  std::map<std::string, editorbuilder_t> m_name_to_builder;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ABSTRACT_EDITOR_FACTORY_H_
