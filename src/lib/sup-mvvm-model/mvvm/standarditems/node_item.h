/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STANDARDITEMS_NODE_ITEM_H_
#define MVVM_STANDARDITEMS_NODE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

/**
 * @brief The NodeItem class represents a connectable node for NodeEditor.
 *
 * It doesn't have any plotting capabilities. It can be adapted for plotting by using
 * IConnectableAdapter.
 */
class NodeItem : public CompoundItem
{
public:
  static constexpr std::string_view kXpos{"kXpos"};
  static constexpr std::string_view kYpos{"kYpos"};
  static constexpr std::string_view kColor{"kColor"};

  explicit NodeItem(const std::string& model_type = {});

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * Returns X coordinate of the node on the graphics scene.
   */
  double GetX() const;

  /**
   * @brief Sets X coordinate of the node on the graphics scene.
   */
  void SetX(double value);

  /**
   * Returns Y coordinate of the node on the graphics scene.
   */
  double GetY() const;

  /**
   * @brief Sets Y coordinate of the node on the graphics scene.
   */
  void SetY(double value);

  /**
   * @brief Returns color of the node for display in the node editor.
   *
   */
  std::string GetBaseColor() const;

  /**
   * @brief Returns color of the node for display in the node editor.
   *
   * This should be the name which QColor understands,
   * i.e. https://www.w3.org/TR/css-color-3/#svg-color, or any "#RRGGBB" format.
   */
  void SetBaseColor(const std::string& value);

protected:
  void RegisterSingleChildTag();

  void RegisterMultipleChildrenTag();
};

template <>
struct item_traits<NodeItem>
{
  static constexpr std::string_view type_name() noexcept { return "NodeItem"; }
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_NODE_ITEM_H_
