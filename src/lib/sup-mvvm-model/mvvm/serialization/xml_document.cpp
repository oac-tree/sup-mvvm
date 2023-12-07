/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <mvvm/interfaces/sessionmodel_interface.h>

#include <stdexcept>

namespace
{
const std::string kDocumentElementType = "Document";
}  // namespace

namespace mvvm
{
struct XmlDocument::XmlDocumentImpl
{
  std::vector<SessionModelInterface*> models;
  explicit XmlDocumentImpl(std::vector<SessionModelInterface*> models) : models(std::move(models))
  {
  }
};

//! Models will be writen/restored to and from XML according to the order given in `models` vector.

XmlDocument::XmlDocument(const std::vector<SessionModelInterface*>& models)
    : p_impl(std::make_unique<XmlDocumentImpl>(models))
{
}

//! Saves models on disk.
void XmlDocument::Save(const std::string& file_name) const
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  TreeData document_tree(kDocumentElementType);
  for (auto model : p_impl->models)
  {
    document_tree.AddChild(*converter.ToTreeData(*model));
  }

  ::mvvm::WriteToXMLFile(file_name, document_tree);
}

//! Loads models from disk. If models have some data already, it will be rewritten.

void XmlDocument::Load(const std::string& file_name)
{
  auto document_tree = ParseXMLDataFile(file_name);
  if (document_tree->GetType() != kDocumentElementType)
  {
    throw std::runtime_error(
        "Error in XmlDocument: given XML doesn't containt correct entry element");
  }

  TreeDataModelConverter converter(ConverterMode::kClone);
  int index{0};
  for (const auto& child : document_tree->Children())
  {
    converter.PopulateSessionModel(child, *p_impl->models.at(index++));
  }
}

XmlDocument::~XmlDocument() = default;

}  // namespace mvvm
