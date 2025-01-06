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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_COMMANDS_COMMAND_STATUS_H_
#define MVVM_COMMANDS_COMMAND_STATUS_H_

namespace mvvm
{

/**
 * @brief The CommandStatus enum defines 3 states in which a command can be.
 */
enum class CommandStatus
{
  Initial,       //! after construction
  AfterExecute,  //! method Execute was called, and successfully finished
  AfterUndo      //! method Undo was called, and successfully finished
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_STATUS_H_
