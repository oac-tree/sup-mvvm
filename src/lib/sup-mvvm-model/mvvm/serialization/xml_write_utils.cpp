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

#include "xml_write_utils.h"

#include "treedata.h"

#include <mvvm/core/exceptions.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

// ----------------------------------------------------------------------------
// Declarations of helper methods and constants in anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
//! Converts xmlChar to std::string.
std::string ToString(const xmlChar *xml_name)
{
  return std::string(reinterpret_cast<const char *>(xml_name), xmlStrlen(xml_name));
}

//! Converts std::string to xmlChar.
const xmlChar *FromString(const std::string &str)
{
  return reinterpret_cast<const xmlChar *>(str.c_str());
}

//! Main method for recursive writing of XML from TreeData.
void AddTreeData(xmlTextWriterPtr writer, const mvvm::TreeData &tree_data);

//! Adds to currently opened XML element all attributes defined in TreeData.
void AddTreeAttributes(xmlTextWriterPtr writer, const mvvm::TreeData &tree_data);

//! Set-up identation.
void SetupWriterIndentation(xmlTextWriterPtr writer);
}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

namespace mvvm
{
void WriteToXMLFile(const std::string &file_name, const TreeData &tree_data)
{
  int rc{0};
  xmlTextWriterPtr writer;
  xmlChar *tmp{nullptr};

  // Create a new XmlWriter for uri, with no compression.
  writer = xmlNewTextWriterFilename(file_name.c_str(), 0);
  if (!writer)
    throw RuntimeException("Error creating the XML writer");
  SetupWriterIndentation(writer);

  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  AddTreeData(writer, tree_data);

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);
}

std::string GetXMLString(const TreeData &tree_data)
{
  int rc(0);
  xmlTextWriterPtr writer;
  xmlBufferPtr buf;
  xmlChar *tmp{nullptr};

  // Create a new XML buffer, to which the XML document will be written
  buf = xmlBufferCreate();
  if (!buf)
  {
    throw RuntimeException("testXmlwriterMemory: Error creating the xml buffer");
  }

  // Create a new XmlWriter for memory, with no compression.
  writer = xmlNewTextWriterMemory(buf, 0);
  if (!writer)
  {
    throw RuntimeException("testXmlwriterMemory: Error creating the xml writer");
  }
  SetupWriterIndentation(writer);

  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  AddTreeData(writer, tree_data);

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);

  auto result = ToString(buf->content);
  xmlBufferFree(buf);
  return result;
}

}  // namespace mvvm

// ----------------------------------------------------------------------------
// Implementation of helper functions from anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
void AddTreeData(xmlTextWriterPtr writer, const mvvm::TreeData &tree_data)
{
  if (tree_data.GetType().empty())
  {
    throw mvvm::RuntimeException("Error in AddTreeData: missed type in TreeData.");
  }

  // opening element
  int rc = xmlTextWriterStartElement(writer, FromString(tree_data.GetType()));
  if (rc < 0)
  {
    throw mvvm::RuntimeException("testXmlwriterFilename: Error at xmlTextWriterStartElement");
  }

  // writing attribute
  AddTreeAttributes(writer, tree_data);

  // writing content
  if (!tree_data.GetContent().empty())
  {
    rc = xmlTextWriterWriteString(writer, FromString(tree_data.GetContent()));
    if (rc < 0)
    {
      throw mvvm::RuntimeException("Error in xmlTextWriterWriteString");
    }
  }

  // writing children
  for (const auto &child : tree_data.Children())
  {
    AddTreeData(writer, child);
  }

  // closing element
  rc = xmlTextWriterEndElement(writer);
  if (rc < 0)
  {
    throw mvvm::RuntimeException("testXmlwriterFilename: Error at xmlTextWriterEndElement");
  }
}

void AddTreeAttributes(xmlTextWriterPtr writer, const mvvm::TreeData &tree_data)
{
  for (const auto &attr : tree_data.Attributes())
  {
    int rc = xmlTextWriterWriteAttribute(writer, FromString(attr.first), FromString(attr.second));
    if (rc < 0)
    {
      throw mvvm::RuntimeException("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute");
    }
  }
}

void SetupWriterIndentation(xmlTextWriterPtr writer)
{
  const int indentation_on = 1;
  xmlTextWriterSetIndent(writer, indentation_on);
  xmlTextWriterSetIndentString(writer, FromString(std::string("  ")));
}

}  // namespace
