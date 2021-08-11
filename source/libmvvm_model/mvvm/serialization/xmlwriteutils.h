// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_SERIALIZATION_XMLWRITEUTILS_H
#define MVVM_SERIALIZATION_XMLWRITEUTILS_H

//! @file xmlwriteutils.h
//! Collection of utilities to write TreeData into XML files.

#include <string>

namespace ModelView
{

class TreeData;

void WriteToXMLFile(const std::string& file_name, const TreeData& tree_data);

std::string GetXMLString(const TreeData& tree_data);

}  // namespace ModelView

#endif  // MVVM_SERIALIZATION_XMLWRITEUTILS_H
