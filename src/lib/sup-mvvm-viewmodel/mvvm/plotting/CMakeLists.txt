# Various controllers to connect plotting items with certain plotting
# framework (QCustomPlot, QtCharts)

target_sources(${library_name} PRIVATE
  plot_helper.cpp
  plot_helper.h
  )

add_subdirectory(customplot)
add_subdirectory(charts)
