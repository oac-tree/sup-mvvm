/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "xml_document.h"

#include "treedata.h"
#include "treedata_model_converter.h"
#include "xml_parse_utils.h"
#include "xml_write_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/sessionmodel_interface.h>

namespace
{
const std::string kProjectNameAttribute = "project";
}

namespace mvvm
{

XmlDocument::XmlDocument(const std::vector<SessionModelInterface*>& models,
                         const std::string& project_name)
    : m_models(models), m_project_name(project_name)
{
}

std::string XmlDocument::GetProjectName() const
{
  return m_project_name;
}

XmlDocument::~XmlDocument() = default;

void XmlDocument::Save(const std::string& file_name) const
{
  const TreeDataModelConverter converter(ConverterMode::kClone);

  TreeData document_tree(kRootElementType);
  if (!GetProjectName().empty())
  {
    document_tree.AddAttribute(kProjectNameAttribute, GetProjectName());
  }

  for (auto model : m_models)
  {
    document_tree.AddChild(*converter.ToTreeData(*model));
  }

  ::mvvm::WriteToXMLFile(file_name, document_tree);
}

void XmlDocument::Load(const std::string& file_name)
{
  auto document_tree = ParseXMLDataFile(file_name);
  if (document_tree->GetType() != kRootElementType)
  {
    throw RuntimeException(
        "Error in XML document: given XML doesn't containt correct entry element");
  }

  if (document_tree->HasAttribute(kProjectNameAttribute) || !GetProjectName().empty())
  {
    if (document_tree->GetAttribute(kProjectNameAttribute) != GetProjectName())
    {
      throw RuntimeException("Error in XML document: expected project name [" + GetProjectName()
                             + "] doesn't coincide with XML project attribute ["
                             + document_tree->GetAttribute(kProjectNameAttribute) + "]");
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
