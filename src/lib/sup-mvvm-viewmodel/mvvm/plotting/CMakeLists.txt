# Various controllers to connect plotting items with certain plotting
# framework (QCustomPlot, QtCharts)

target_sources(${library_name} PRIVATE
  plot_constants.h
  plot_helper.cpp
  plot_helper.h
  plot_types.h
  )

add_subdirectory(customplot)
add_subdirectory(charts)
