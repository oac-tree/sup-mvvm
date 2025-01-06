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

#ifndef MVVM_SERIALIZATION_XML_DOCUMENT_HELPER_H_
#define MVVM_SERIALIZATION_XML_DOCUMENT_HELPER_H_

#include <mvvm/model/session_model.h>
#include <mvvm/serialization/xml_document.h>

#include <memory>
#include <string>

namespace mvvm
{

/**
 * @brief Creates models and populates them with content from the file.
 *
 * It is assumed that the file contains already serialized content of models of the same type and in
 * the order given by template arguments.
 *
 * auto result = LoadModels<TestModelA, TestModelB>(file_path);
 *
 * where the result will be
 *
 * std::tuple<std::unique_ptr<TestModelA>, std::unique_ptr<TestModelB>>
 *
 * @tparam Variadic list of model types.
 *
 * @param path Full path to a single xml file containing serialised model.
 * @param application_type Possible expected
 * @return
 */
template <typename... Args>
std::tuple<std::unique_ptr<Args>...> LoadModels(const std::string& path,
                                                const std::string& application_type = {})
{
  auto result = std::make_tuple(std::make_unique<Args>()...);

  std::vector<ISessionModel*> models;
  std::apply([&models](auto&&... args) { (models.push_back(args.get()), ...); }, result);

  XmlDocument document(models, application_type);
  document.Load(path);

  return result;
}

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_XML_DOCUMENT_HELPER_H_
