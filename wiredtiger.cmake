function (build_git_project target repository tag)
    set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/ExternalProjects/${target}")

    set(CMAKELIST_CONTENT "
        cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})
        project(build_external_project)
        include(ExternalProject)
        ExternalProject_add(${target}
            PREFIX \"${TARGET_DIR}\"
            GIT_REPOSITORY \"${repository}\"
            GIT_TAG \"${tag}\"
            CMAKE_GENERATOR \"${CMAKE_GENERATOR}\"
            CMAKE_GENERATOR_PLATFORM \"${CMAKE_GENERATOR_PLATFORM}\"
            CMAKE_GENERATOR_TOOLSET \"${CMAKE_GENERATOR_TOOLSET}\"
            CMAKE_GENERATOR_INSTANCE \"${CMAKE_GENERATOR_INSTANCE}\"
            CMAKE_ARGS ${ARGN})

        add_custom_target(build_external_project)
        add_dependencies(build_external_project ${target})
    ")

    file(WRITE "${TARGET_DIR}/CMakeLists.txt" "${CMAKELIST_CONTENT}")

    file(MAKE_DIRECTORY "${TARGET_DIR}" "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
        -G "${CMAKE_GENERATOR}"
        -A "${CMAKE_GENERATOR_PLATFORM}"
        -T "${CMAKE_GENERATOR_TOOLSET}"
        ..
        WORKING_DIRECTORY "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
        --build .
        --config ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY "${TARGET_DIR}/build")

endfunction()

function(add_wiredtiger lib tag install_dir)
    build_git_project(wiredtiger_external "https://github.com/wiredtiger/wiredtiger.git" ${tag} "-DCMAKE_INSTALL_PREFIX=${install_dir}")
    find_library(${lib} wiredtiger PATHS "${install_dir}/lib" NO_DEFAULT_PATH REQUIRED)
endfunction()
