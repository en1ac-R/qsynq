add_library(qsynq_file_watcher STATIC
    file-watcher.cpp
)

target_include_directories(qsynq_file_watcher
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
