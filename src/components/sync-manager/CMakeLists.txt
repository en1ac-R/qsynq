add_library(qsynq_sync_manager STATIC
    sync-manager.cpp
)

target_include_directories(qsynq_sync_manager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qsynq_sync_manager
    PUBLIC
        qsynq_file_scanner
        qsynq_index_db
        qsynq_diff_engine
)
