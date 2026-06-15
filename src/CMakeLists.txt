foreach(QSYNQ_VCPKG_PREFIX
    "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-linux"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/vcpkg_installed/x64-linux"
)
    if(EXISTS "${QSYNQ_VCPKG_PREFIX}")
        list(APPEND CMAKE_PREFIX_PATH "${QSYNQ_VCPKG_PREFIX}")
    endif()
endforeach()

find_package(SQLiteCpp CONFIG REQUIRED)

add_subdirectory(components/common)
add_subdirectory(components/file-system-layer)
add_subdirectory(components/index-db)
add_subdirectory(components/logic)
add_subdirectory(components/network)
add_subdirectory(components/sync-manager)
add_subdirectory(daemon)
add_subdirectory(cli)
