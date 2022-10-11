# Experimental code to try out various refactoring solutions. Not intended for production.

target_sources(${library_name} PRIVATE
  event_types.h
  event_types.cpp
  event_handler.cpp
  event_handler.h
)
