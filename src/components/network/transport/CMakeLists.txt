add_library(qsynq_transport STATIC
    transport-client.cpp
    transport-message.cpp
    transport-server.cpp
)

target_include_directories(qsynq_transport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
