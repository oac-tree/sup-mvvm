// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "dragviewmodel.h"
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/widgets/widget_utils.h>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <algorithm>

namespace {
const QString AppMimeType = "application/org.bornagainproject.moveitem";
} // namespace

using namespace mvvm;

namespace DragAndMove {

DragViewModel::DragViewModel(SessionModel* model, QObject* parent)
    : PropertyTableViewModel(model, parent)
{
}

Qt::ItemFlags DragViewModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = PropertyTableViewModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData* DragViewModel::mimeData(const QModelIndexList& index_list) const
{
  // FIXME restore
//    auto mimeData = new QMimeData;
//    auto items = utils::ParentItemsFromIndex(index_list);

//    // Saving list of SessionItem's identifiers related to all DemoItem

//    QStringList identifiers;
//    for (auto item : Utils::ParentItemsFromIndex(index_list))
//        identifiers.append(QString::fromStdString(item->identifier()));

//    mimeData->setData(AppMimeType, utils::GetByteArray(identifiers));
//    return mimeData;
  return {};
}

Qt::DropActions DragViewModel::supportedDragActions() const
{
    return Qt::TargetMoveAction;
}

Qt::DropActions DragViewModel::supportedDropActions() const
{
    return Qt::TargetMoveAction;
}

bool DragViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction, int, int,
                                    const QModelIndex&) const
{
    return data->hasFormat(AppMimeType);
}

bool DragViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                 const QModelIndex& parent)
{
  // FIXME restore
//    if (!canDropMimeData(data, action, row, column, parent))
//        return false;

//    int requested_row = parent.isValid() ? parent.row() : row;

//    // retrieving list of item identifiers and accessing items
//    auto identifiers = Utils::deserialize(data->data(AppMimeType));
//    for (const auto& id : identifiers) {
//        auto item = sessionModel()->findItem(id.toStdString());

//        int row = std::clamp(requested_row, 0, item->parent()->itemCount(item->tagRow().tag) - 1);
//        sessionModel()->moveItem(item, rootSessionItem(), {"", row});
//    }

    return false;
}

} // namespace DragAndMove
