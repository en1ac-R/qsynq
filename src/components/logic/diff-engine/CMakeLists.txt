add_library(qsynq_diff_engine STATIC
    diff-engine.cpp
)

target_include_directories(qsynq_diff_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qsynq_diff_engine
    PUBLIC
        qsynq_common
)
