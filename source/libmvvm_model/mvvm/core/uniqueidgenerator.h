// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_CORE_UNIQUEIDGENERATOR_H
#define MVVM_CORE_UNIQUEIDGENERATOR_H

#include <string>

namespace ModelView {

//! Provides generation of unique SessionItem identifier.

//! In the future might be turned to singleton to keep track of all generated identifier
//! and make sure, that SessionItem identifiers loaded from disk, are different from those
//! generated during a dynamic session. For the moment though, we rely on zero-probability of
//! such event.

class UniqueIdGenerator {
public:
    static std::string generate();
};

} // namespace ModelView

#endif // MVVM_CORE_UNIQUEIDGENERATOR_H
