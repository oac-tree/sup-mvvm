# Contains source specific for ViewModelBase (non-specific to SessionItem)

target_sources(${library_name} PRIVATE
  presentation_item.h
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
