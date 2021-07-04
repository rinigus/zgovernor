cmake_minimum_required(VERSION 3.5.1)

project(zgovernor
    VERSION 0.1
    DESCRIPTION "Z Governor")

if(CMAKE_BUILD_TYPE STREQUAL "release")
    add_definitions(
        -QT_NO_WARNING_OUTPUT
        -QT_NO_DEBUG_OUTPUT)
endif()

set(CONFIG_FILE "/etc/zgovernor.ini" CACHE STRING "Configuration file")

set(APP_NAME zgovernor)

## C++ sources
file(GLOB SRC src/*.cpp)
file(GLOB HEADERS src/*.h)

# set requirements
set(QT_MIN_VERSION "5.6.0")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# find required packages
find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS Core DBus REQUIRED)

include(FindPkgConfig)
pkg_search_module(KEEPALIVE keepalive REQUIRED)

add_compile_options(
    -DAPP_NAME="${APP_NAME}"
    -DAPP_VERSION="${PROJECT_VERSION}"
    -DCONFIG_FILE="${CONFIG_FILE}")

include_directories(${KEEPALIVE_INCLUDE_DIRS})

include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePrintHelpers)

add_executable(${APP_NAME} ${SRC} ${HEADERS})

target_link_libraries(${APP_NAME}
    Qt5::Core
    Qt5::DBus
    ${KEEPALIVE_LDFLAGS})

install(TARGETS ${APP_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
