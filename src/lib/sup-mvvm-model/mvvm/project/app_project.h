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

#ifndef MVVM_PROJECT_APP_PROJECT_H_
#define MVVM_PROJECT_APP_PROJECT_H_

#include <mvvm/core/exceptions.h>
#include <mvvm/project/abstract_project.h>

namespace mvvm
{

/**
 * @brief The AppProject class is the main project containing all application models.
 *
 * Models are registered before the first use. They will be re-created on every new project
 * creation, or every load from disk.
 */
class AppProject : public mvvm::AbstractProject
{
public:
  using model_factory_func_t = std::function<std::unique_ptr<mvvm::ISessionModel>()>;
  using callback_t = std::function<void()>;

  /**
   * @brief Main c-tor.
   *
   * @param context A context with project settings.
   */
  explicit AppProject(const mvvm::ProjectContext& context);

  std::vector<mvvm::ISessionModel*> GetModels() const override;

  /**
   * @brief Registers models that will be created for new project.
   *
   * The model will be created and stored following the order of registrations.
   *
   * @tparam ModelT The type of the model being registered.
   * @return The position of the model in the array of models.
   */
  template <typename ModelT>
  std::size_t RegisterModel();

  /**
   * @brief Registers models that will be created for new project using its factory function.
   *
   * The model will be created and stored following the order of registrations.
   *
   * @param factory_func The models' factory function.
   * @return The position of the model in the array of models.
   */
  std::size_t RegisterModel(model_factory_func_t factory_func);

  /**
   * @brief Returns number of existing models.
   */
  std::size_t GetModelCount() const;

  /**
   * @brief Returns the model stored for given index and casted to the given type.
   *
   * Will throw if cast can't be done.
   */
  template <typename ModelT>
  ModelT* GetModel(std::size_t index = 0);

private:
  bool SaveImpl(const std::string& path) override;
  bool LoadImpl(const std::string& path) override;
  bool CloseProjectImpl() override;
  bool CreateEmptyProjectImpl() override;

  /**
   * @brief Creates models by calling all registered factory functions.
   *
   * All previous models will be destroyed.
   */
  std::vector<std::unique_ptr<mvvm::ISessionModel>> CreateModels();

  std::vector<model_factory_func_t> m_factory_func_list;
  std::vector<std::unique_ptr<mvvm::ISessionModel>> m_models;
};

template <typename ModelT>
inline std::size_t AppProject::RegisterModel()
{
  return RegisterModel([]() { return std::make_unique<ModelT>(); });
}

template <typename ModelT>
inline ModelT* AppProject::GetModel(std::size_t index)
{
  if (index >= m_models.size())
  {
    throw RuntimeException("Index exeeds model count");
  }
  auto model = dynamic_cast<ModelT*>(m_models.at(index).get());
  if (!model)
  {
    throw RuntimeException("Can't cast the model to a given type");
  }
  return model;
}

}  // namespace mvvm

#endif  // MVVM_PROJECT_APP_PROJECT_H_
