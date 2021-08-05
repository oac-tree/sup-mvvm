// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/core/uniqueidgenerator.h"
#include <QUuid>

// FIXME Remove dependency from Qt
// Replace <QUuid> with thirparty https://github.com/mariusbancila/stduuid

using namespace ModelView;

std::string UniqueIdGenerator::Generate()
{
    return QUuid::createUuid().toString().toStdString();
}
