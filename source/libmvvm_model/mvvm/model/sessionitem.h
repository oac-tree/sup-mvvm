// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_SESSIONITEM_H
#define MVVM_MODEL_SESSIONITEM_H

#include <string>
#include <vector>

namespace ModelView
{
class SessionItem
{
public:
  SessionItem(std::string model_type = "BaseType") : m_model_type(model_type) {}

  std::string modelType() const { return m_model_type; }

private:
  std::string m_model_type;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_TAGINFO_H
