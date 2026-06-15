add_library(qsynq_index_db STATIC
    initializer/schema.cpp
    repo/file-repository.cpp
)

target_include_directories(qsynq_index_db
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qsynq_index_db
    PUBLIC
        qsynq_common
        SQLiteCpp
)
