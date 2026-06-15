find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter)

set(QSYNQ_CLI_OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qsynq)

add_custom_command(
    OUTPUT ${QSYNQ_CLI_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/scripts/qsynq ${QSYNQ_CLI_OUTPUT}
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/qsynq
    COMMENT "Copying qsynq Python CLI"
)

add_custom_target(qsynq_cli ALL
    DEPENDS ${QSYNQ_CLI_OUTPUT}
)

install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/qsynq DESTINATION bin)
