
cmake_minimum_required(VERSION 3.12)

#Use the syntax $ENV{VAR} to read environment variable VAR
#To test whether an environment variable is defined, use the signature if(DEFINED ENV{<name>})
if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(sortIpV4HW2 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(sortIpV4HW2 VERSION 0.0.1)
endif()

add_executable(sortIpV4HW2_cli ip_filter-main.cpp)
add_library(sortIpV4HW2 ip_filter.cpp)

set_target_properties(sortIpV4HW2_cli sortIpV4HW2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(sortIpV4HW2_cli PRIVATE
    sortIpV4HW2
)

if (MSVC)
    target_compile_options(sortIpV4HW2_cli PRIVATE
        /W4
    )
    target_compile_options(sortIpV4HW2 PRIVATE
        /W4
    )
else ()
    target_compile_options(sortIpV4HW2_cli PRIVATE
        -Wall -O2 -Wextra -pedantic -Werror # -O2 решает (особеено в этом задании прирост производительности в 4 раза)
    )
    target_compile_options(sortIpV4HW2 PRIVATE
        -Wall -O2 -Wextra -pedantic -Werror
    )
endif()

install(TARGETS sortIpV4HW2_cli RUNTIME DESTINATION bin)