add_library(qsynq_file_scanner STATIC
    file-scanner.cpp
)

target_include_directories(qsynq_file_scanner
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qsynq_file_scanner
    PUBLIC
        qsynq_common
)
