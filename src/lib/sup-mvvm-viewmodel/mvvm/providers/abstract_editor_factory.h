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

#ifndef MVVM_PROVIDERS_ABSTRACT_EDITOR_FACTORY_H_
#define MVVM_PROVIDERS_ABSTRACT_EDITOR_FACTORY_H_

#include <mvvm/providers/editor_types_fwd.h>
#include <mvvm/providers/i_editor_factory.h>

#include <map>

namespace mvvm
{

/**
 * @brief The AbstractEditorFactory class is a base for all editor factories.
 *
 * It allows to register cell editor builder functions for the given builder name.
 */
class MVVM_VIEWMODEL_EXPORT AbstractEditorFactory : public IEditorFactory
{
public:
  void RegisterBuilder(const std::string& name, editorbuilder_t builder);

  /**
   * @brief Returns editor factory function registered for the given editor name.
   */
  editorbuilder_t FindBuilder(const std::string& name) const;

private:
  std::map<std::string, editorbuilder_t> m_name_to_builder;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ABSTRACT_EDITOR_FACTORY_H_
