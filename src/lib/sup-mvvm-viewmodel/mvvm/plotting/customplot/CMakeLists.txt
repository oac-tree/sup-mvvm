# Integration of plotting items and QCustomPlot framework

target_sources(${library_name} PRIVATE
  axis_title_controller.cpp
  axis_title_controller.h
  custom_plot_utils.cpp
  custom_plot_utils.h
  data1d_plot_controller.cpp
  data1d_plot_controller.h
  graph_info_formatter.cpp
  graph_info_formatter.h
  graph_plot_controller.cpp
  graph_plot_controller.h
  graph_viewport_plot_controller.cpp
  graph_viewport_plot_controller.h
  mouse_move_reporter.cpp
  mouse_move_reporter.h
  mouse_pos_info.h
  pen_controller.cpp
  pen_controller.h
  status_string_formatter_interface.h
  status_string_reporter.cpp
  status_string_reporter.h
  status_string_reporter_factory.cpp
  status_string_reporter_factory.h
  viewport_axis_plot_controller.cpp
  viewport_axis_plot_controller.h
)

