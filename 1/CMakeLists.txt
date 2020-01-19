cmake_minimum_required(VERSION 3.12)

#Use the syntax $ENV{VAR} to read environment variable VAR
#To test whether an environment variable is defined, use the signature if(DEFINED ENV{<name>})
if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(helloworld VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Copy a file to another location and modify its contents
configure_file(version.h.in version.h)

#Add an executable to the project using the specified source files
#Adds an executable target called <name> to be built from the source
#files listed in the command invocation. (The source files can be 
#omitted here if they are added later using 
#target_sources().)
add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)

# Targets can have properties that affect how they are built.
set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Add include directories to a target
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
target_include_directories(test_version 
      PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(helloworld_cli PRIVATE
    helloworld
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld
)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
# стандарты сами содержат набор рекомендаций к производителям компиляторов для генерации предупреждений
# Включить соответствующие предупреждения можно указав для команды компиляции ключ -pedantic


install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

# This command should be in the source directory root because 
# ctest expects to find a test file in the build directory root.
# This command is automatically invoked when the CTest module is 
# included, except if the BUILD_TESTING option is turned off.

enable_testing()

# Add a test to the project to be run
add_test(test_version test_version)
