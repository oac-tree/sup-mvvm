// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_CELLDECORATORINTERFACE_H
#define MVVM_INTERFACES_CELLDECORATORINTERFACE_H

#include "mvvm/viewmodel_export.h"

#include <string>

class QModelIndex;
class QStyleOptionViewItem;

namespace mvvm
{

//! Interface class to generate cell decorations (i.e. text) in Qt trees and tables.

class MVVM_VIEWMODEL_EXPORT CellDecoratorInterface
{
public:
  virtual ~CellDecoratorInterface() = default;

  virtual bool HasCustomDecoration(const QModelIndex& index) const = 0;
  virtual void InitStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_CELLDECORATORINTERFACE_H
