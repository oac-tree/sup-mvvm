// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "samplemodel.h"

#include "sampleitems.h"

#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

namespace DragAndMove
{

namespace
{
std::string random_name()
{
  static const std::string alphabet = "abcdefgh";
  const size_t len(3);

  std::string result;
  for (size_t i = 0; i < len; ++i)
  {
    size_t random_index =
        static_cast<size_t>(mvvm::utils::RandInt(0, static_cast<int>(alphabet.size() - 1)));
    result.push_back(alphabet[random_index]);
  }

  return result;
}
}  // namespace

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<DemoItem>();
  RegisterItem<DemoContainerItem>();
  populateModel();
}

void SampleModel::appendRandomItem(mvvm::SessionItem* container)
{
  auto item = InsertItem<DemoItem>(container);
  item->SetProperty(DemoItem::P_COLOR_PROPERTY, mvvm::utils::RandomNamedColor());
  item->SetProperty(DemoItem::P_STRING_PROPERTY, random_name());
  item->SetProperty(DemoItem::P_INTEGER_PROPERTY, mvvm::utils::RandInt(0, 10));
}

//! Generates initial model content.

void SampleModel::populateModel()
{
  auto container = InsertItem<DemoContainerItem>();
  appendRandomItem(container);
  appendRandomItem(container);
  appendRandomItem(container);

  container = InsertItem<DemoContainerItem>();
  appendRandomItem(container);
  appendRandomItem(container);
}

}  // namespace DragAndMove
