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

#ifndef MVVM_SERIALIZATION_I_MODEL_DOCUMENT_H_
#define MVVM_SERIALIZATION_I_MODEL_DOCUMENT_H_

#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{

/**
 * @brief The IModelDocument class is an interface for all documents that can be stored on disk as a
 * file.
 *
 * It can be an XML file representing the content of one or several SessionModels.
 */
class MVVM_MODEL_EXPORT IModelDocument
{
public:
  virtual ~IModelDocument() = default;

  /**
   * @brief Returns the type of the application.
   *
   * This name comes from IProject and helps to distinguish XML documents intended for various MVVM
   * projects.
   */
  virtual std::string GetApplicationType() const = 0;

  /**
   * @brief Saves the document to a file.
   *
   * @param file_name A full file name (path included) to a file on disk.
   */
  virtual void Save(const std::string& file_name) const = 0;

  /**
   * @brief Loads the document from a file.
   *
   * @param file_name A full file name (path included) to a file on disk.
   */
  virtual void Load(const std::string& file_name) = 0;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_I_MODEL_DOCUMENT_H_
