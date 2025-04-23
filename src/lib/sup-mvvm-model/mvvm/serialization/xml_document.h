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

#ifndef MVVM_SERIALIZATION_XML_DOCUMENT_H_
#define MVVM_SERIALIZATION_XML_DOCUMENT_H_

#include <mvvm/serialization/i_model_document.h>

#include <vector>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The XmlDocument class saves and restores list of SessionModel's to/from disk using XML
 * format.
 *
 * Single XmlDocument corresponds to a single file on disk but can store multiple models.
 */
class MVVM_MODEL_EXPORT XmlDocument : public IModelDocument
{
public:
  static inline const std::string kRootElementType = "MvvmDocument";

  /**
   * @brief Main c-tor.
   *
   * Models will be saved/restored according to the model list given at construction. When
   * application type name is provided, it will be added as an attribute to the root element.
   *
   * @param models List of models to process.
   * @param application_type The type name of the application that can open this document.
   */
  explicit XmlDocument(const std::vector<ISessionModel*>& models,
                       const std::string& application_type = {});
  ~XmlDocument() override;

  /**
   * @brief Returns the type name of the application.
   */
  std::string GetApplicationType() const override;

  void Save(const std::string& file_name) const override;

  /**
   * @details If model contains the data already, it will be reset. If loading is not possible due
   * absent file, or XML parsing error, will throw.
   */
  void Load(const std::string& file_name) override;

private:
  std::vector<ISessionModel*> m_models;
  std::string m_application_type;
};
}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_XML_DOCUMENT_H_
