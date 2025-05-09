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

#include "xml_document.h"

#include "tree_data.h"
#include "tree_data_model_converter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>

#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_serialize.h>

namespace
{
const std::string kApplicationTypeAttribute = "application";
}

namespace mvvm
{

XmlDocument::XmlDocument(const std::vector<ISessionModel*>& models,
                         const std::string& application_type)
    : m_models(models), m_application_type(application_type)
{
}

std::string XmlDocument::GetApplicationType() const
{
  return m_application_type;
}

XmlDocument::~XmlDocument() = default;

void XmlDocument::Save(const std::string& file_name) const
{
  const TreeDataModelConverter converter(ConverterMode::kClone);

  tree_data_t document_tree(kRootElementType);
  if (!GetApplicationType().empty())
  {
    document_tree.AddAttribute(kApplicationTypeAttribute, GetApplicationType());
  }

  for (auto model : m_models)
  {
    document_tree.AddChild(*converter.ToTreeData(*model));
  }

  mvvm::xml::TreeDataToFile(file_name, document_tree);
}

void XmlDocument::Load(const std::string& file_name)
{
  auto document_tree = mvvm::xml::TreeDataFromFile(file_name);
  if (document_tree->GetNodeName() != kRootElementType)
  {
    throw RuntimeException(
        "Error in XML document: given XML doesn't containt correct entry element ["
        + kRootElementType + "].");
  }

  if (document_tree->HasAttribute(kApplicationTypeAttribute) || !GetApplicationType().empty())
  {
    if (document_tree->GetAttribute(kApplicationTypeAttribute) != GetApplicationType())
    {
      throw RuntimeException("Error in XML document: application type attribute ["
                             + document_tree->GetAttribute(kApplicationTypeAttribute)
                             + "] doesn't match expectations [" + GetApplicationType() + "]");
    }
  }

  const TreeDataModelConverter converter(ConverterMode::kClone);
  int index{0};
  for (const auto& child : document_tree->Children())
  {
    converter.PopulateSessionModel(child, *m_models.at(index++));
  }
}

}  // namespace mvvm
