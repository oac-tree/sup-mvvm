/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
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
 ******************************************************************************/

#include "mvvm/serialization/xmlparseutils.h"

#include "mvvm/core/filesystem.h"

#include <libxml/parser.h>

#include <stdexcept>

namespace ModelView
{
static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node);

static void AddXMLAttributes(TreeData *tree, xmlNodePtr node);

static void AddXMLChildren(TreeData *tree, xmlDocPtr doc, xmlNodePtr node);

static std::string ToString(const xmlChar *xml_name);

std::unique_ptr<TreeData> ParseXMLDataFile(const std::string &filename)
{
  // Read file into xmlDocPtr
  if (!std::filesystem::exists(filename))
    throw std::runtime_error("Error in ParseXMLDataFile: files '" + filename + "' doesn't exist");

  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc == nullptr)
    throw std::runtime_error("Error in ParseXMLDataFile: parse error");

  return ParseXMLDoc(doc);
}

std::unique_ptr<TreeData> ParseXMLDataString(const std::string &xml_str)
{
  // Read the string into xmlDocPtr
  xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar *>(xml_str.c_str()));
  auto xml_head = xml_str.substr(0, 1024);
  if (doc == nullptr)
    throw std::runtime_error("Error in ParseXMLDataFile: parse error");

  return ParseXMLDoc(doc);
}

std::unique_ptr<TreeData> ParseXMLElementString(const std::string &xml_str)
{
  const std::string header{R"RAW(<?xml version = "1.0" encoding = "UTF-8"?>
)RAW"};

  std::string content = header + xml_str;
  // Read the string into xmlDocPtr
  xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar *>(content.c_str()));
  auto xml_head = xml_str.substr(0, 1024);
  if (doc == nullptr)
    throw std::runtime_error("Error in ParseXMLDataFile: parse error");

  return ParseXMLDoc(doc);
}

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc)
{
  // Check root element
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  if (root_node == nullptr)
    throw std::runtime_error("Error in ParseXMLDataFile: couldn't retrieve root element");

  auto data_tree = ParseDataTree(doc, root_node);
  xmlFreeDoc(doc);
  return data_tree;
}

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<TreeData> result(new TreeData(node_name));

  AddXMLAttributes(result.get(), node);

  AddXMLChildren(result.get(), doc, node);

  return result;
}

static void AddXMLAttributes(TreeData *tree, xmlNodePtr node)
{
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    tree->AddAttribute(name, value);
    attribute = attribute->next;
  }
}

static void AddXMLChildren(TreeData *tree, xmlDocPtr doc, xmlNodePtr node)
{
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_TEXT_NODE)
    {
      auto xml_content = xmlNodeListGetString(doc, child_node, 1);
      auto content = ToString(xml_content);
      tree->SetContent(content);
      xmlFree(xml_content);
    }
    else if (child_node->type == XML_ELEMENT_NODE)
    {
      auto child_data = ParseDataTree(doc, child_node);
      tree->AddChild(*child_data);
    }
    child_node = child_node->next;
  }
}

static std::string ToString(const xmlChar *xml_name)
{
  return std::string(reinterpret_cast<const char *>(xml_name), xmlStrlen(xml_name));
}

}  // namespace ModelView
