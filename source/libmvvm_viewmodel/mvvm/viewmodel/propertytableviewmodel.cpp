// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/viewmodel/propertytableviewmodel.h"

#include "mvvm/factories/viewmodelcontrollerfactory.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"
#include "mvvm/viewmodelbase/viewitem.h"

namespace mvvm
{

PropertyTableViewModel::PropertyTableViewModel(ApplicationModel* model, QObject* parent)
    : ViewModel(parent)
{
  SetController(factory::CreateController<TopItemsStrategy, PropertiesRowStrategy>(model, this));
}

void PropertyTableViewModel::insertRow(ViewItem* parent, int row,
                                       std::vector<std::unique_ptr<ViewItem>> items)
{
  // The code below is used to inform QTableView about layout change if the number
  // of columns before the insertion doesn't coincide with the length of `items` vector to insert.
  // This happens when PropertyTableViewModel is looking on empty SessionModel.
  int prevColumnCount = parent->columnCount();
  ViewModel::insertRow(parent, row, std::move(items));
  if (parent->columnCount() != prevColumnCount)
  {
    emit layoutChanged();
  }
}

}  // namespace mvvm
