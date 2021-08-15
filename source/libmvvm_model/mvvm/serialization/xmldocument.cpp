/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/xmldocument.h"

#include "mvvm/model/sessionmodel.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedatamodelconverter.h"
#include "mvvm/serialization/xmlparseutils.h"
#include "mvvm/serialization/xmlwriteutils.h"

#include <stdexcept>

namespace
{
const std::string kDocumentElementType = "Document";
}  // namespace

namespace ModelView
{
struct XmlDocument::XmlDocumentImpl
{
  std::vector<SessionModel*> models;
  XmlDocumentImpl(std::vector<SessionModel*> models) : models(std::move(models)) {}
};

XmlDocument::XmlDocument(const std::vector<SessionModel*>& models)
    : p_impl(std::make_unique<XmlDocumentImpl>(models))
{
}

//! Saves models on disk.
void XmlDocument::Save(const std::string& file_name) const
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  TreeData document_tree(kDocumentElementType);
  for (auto model : p_impl->models)
    document_tree.AddChild(*converter.ToTreeData(*model));

  ::ModelView::WriteToXMLFile(file_name, document_tree);
}

//! Loads models from disk. If models have some data already, it will be rewritten.

void XmlDocument::Load(const std::string& file_name)
{
  auto document_tree = ParseXMLDataFile(file_name);
  if (document_tree->GetType() != kDocumentElementType)
    throw std::runtime_error(
        "Error in XmlDocument: given XML doesn't containt correct entry element");

  TreeDataModelConverter converter(ConverterMode::kClone);
  int index{0};
  for (const auto& child : document_tree->Children())
  {
    converter.PopulateSessionModel(child, *p_impl->models.at(index++));
  }
}

XmlDocument::~XmlDocument() = default;

}  // namespace ModelView
