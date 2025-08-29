# ViewModel to represent SessionItem and SessionModel.
# Folder contains QObject derived classes and is excluded from Sonar analysis

target_sources(${library_name} PRIVATE
  all_items_viewmodel.cpp
  all_items_viewmodel.h
  custom_editor.cpp
  custom_editor.h
  filter_name_viewmodel.cpp
  filter_name_viewmodel.h
  property_table_viewmodel.cpp
  property_table_viewmodel.h
  property_viewmodel.cpp
  property_viewmodel.h
  top_items_viewmodel.cpp
  top_items_viewmodel.h
  viewmodel_base.cpp
  viewmodel_base.h
  viewmodel.cpp
  viewmodel.h
  )
