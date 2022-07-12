/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_
#define MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_

#include <mvvm/interfaces/sessionmodel_interface.h>

#include <memory>

namespace mvvm
{
class MVVM_MODEL_EXPORT AbstractModelDecorator : public SessionModelInterface
{
public:
  explicit AbstractModelDecorator(std::unique_ptr<SessionModelInterface> decorated_model);

  std::string GetType() const override;

  SessionItem* GetRootItem() const override;

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent,
                             const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void Clear(std::function<void(SessionItem*)> callback = {}) override;

protected:
  std::unique_ptr<SessionModelInterface> m_decorated_model;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_
