add_executable(qsynqd
    qsynqd.cpp
)

target_link_libraries(qsynqd
    PRIVATE
        qsynq_sync_manager
        qsynq_transport
)
