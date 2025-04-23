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

#ifndef COLLIDINGMICECORE_MOUSEMODEL_H
#define COLLIDINGMICECORE_MOUSEMODEL_H

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

#include <QString>

namespace collidingmice
{

/**
 * @brief The MouseItem class holds information about mouse.
 */
class MouseItem : public mvvm::CompoundItem
{
public:
  static inline const std::string P_COLOR = "P_COLOR";
  static inline const std::string P_XPOS = "P_XPOS";
  static inline const std::string P_YPOS = "P_YPOS";
  static inline const std::string P_ANGLE = "P_ANGLE";
  static inline const std::string P_SPEED = "P_SPEED";

  MouseItem();
};

/**
 * @brief The MouseModel class holds collection of mouses.
 */
class MouseModel : public mvvm::ApplicationModel
{
public:
  MouseModel();

  void LoadFromFile(const QString& name);

  void SaveToFile(const QString& name);

  /**
   * @brief Sets undo stack to given position.
   *
   * value=0   - first command in the stack (corresponds to the model status "in the past")
   * value=100 - last command in the stack (corresponds to the status "now")
   */
  void SetUndoPosition(int value);

private:
  void PopulateModel();
};

}  // namespace collidingmice

#endif  // COLLIDINGMICECORE_MOUSEMODEL_H
