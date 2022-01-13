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

#ifndef MVVM_SERIALIZATION_XMLDOCUMENT_H
#define MVVM_SERIALIZATION_XMLDOCUMENT_H

#include "mvvm/model_export.h"

#include <memory>
#include <vector>

namespace mvvm
{
class SessionModel;

//! Saves and restores list of SessionModel's to/from disk using XML format.
//! Single XMLDocument corresponds to a single file on disk.

class MVVM_MODEL_EXPORT XmlDocument
{
public:
  XmlDocument(const std::vector<SessionModel*>& models);
  ~XmlDocument();

  void Save(const std::string& file_name) const;
  void Load(const std::string& file_name);

private:
  struct XmlDocumentImpl;
  std::unique_ptr<XmlDocumentImpl> p_impl;
};
}  // namespace ModelView

#endif  // MVVM_SERIALIZATION_XMLDOCUMENT_H
