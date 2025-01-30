# Various controllers to connect plotting items with certain plotting
# framework (QCustomPlot, QtCharts)

target_sources(${library_name} PRIVATE
  plot_constants.h
  plot_helper.cpp
  plot_helper.h
  plot_types.h
  )

if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  add_subdirectory(customplot)
endif()

add_subdirectory(charts)
