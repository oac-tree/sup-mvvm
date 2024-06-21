# Contains source for generic ViewModel (no SessionItem dependency)

target_sources(${library_name} PRIVATE
  viewitem.cpp
  viewitem_data.cpp
  viewitem_data.h
  viewitem_data_interface.h
  viewitem.h
  viewmodel_base.cpp
  viewmodel_base.h
  viewmodel_base_utils.cpp
  viewmodel_base_utils.h
  )
