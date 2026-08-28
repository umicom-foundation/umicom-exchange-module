#-----------------------------------------------------------------------------
# Umicom Commodity Exchange Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build a thin C23 product composition over Framework-owned application
#   experience, runtime and workbench contracts.
#
# Created by: Sammy Hegab
# Organisation: Umicom Foundation
# Licence: MIT
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)

project(
    CommodityExchangeModule
    VERSION 0.1.0
    DESCRIPTION "Thin Umicom Commodity Exchange composition over Umicom Framework"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(UMICOM_EXCHANGE_BUILD_CONSOLE
       "Build the native Umicom Commodity Exchange verification console"
       ON)

include(CTest)
include(GNUInstallDirs)

if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Commodity Exchange requires Framework target Umicom::application")
endif()

if(NOT COMMAND umicom_apply_warnings)
    function(umicom_apply_warnings target)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        else()
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        endif()
    endfunction()
endif()

if(NOT COMMAND umicom_apply_sanitizers)
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_exchange_module STATIC
    src/application.c
    src/workspace.c
)
add_library(Umicom::exchange_module ALIAS umicom_exchange_module)

target_include_directories(umicom_exchange_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_exchange_module PUBLIC
    Umicom::Framework
    Umicom::application
)
umicom_apply_warnings(umicom_exchange_module)
umicom_apply_sanitizers(umicom_exchange_module)

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomThinApplicationRuntime.cmake")
umicom_extend_thin_application_runtime(
    NAME "exchange"
    TARGET "umicom_exchange_module")

if(UMICOM_EXCHANGE_BUILD_CONSOLE)
    add_executable(umicom-exchange-console
        src/console/main.c)
    target_link_libraries(umicom-exchange-console PRIVATE Umicom::exchange_module)
    umicom_apply_warnings(umicom-exchange-console)
    umicom_apply_sanitizers(umicom-exchange-console)
endif()

if(BUILD_TESTING)
    add_executable(exchange-application-test
        tests/test_application.c)
    target_link_libraries(exchange-application-test PRIVATE Umicom::exchange_module)
    umicom_apply_warnings(exchange-application-test)
    umicom_apply_sanitizers(exchange-application-test)
    add_test(NAME exchange.module.application
             COMMAND exchange-application-test)

    add_executable(exchange-workspace-test
        tests/test_workspace.c)
    target_link_libraries(exchange-workspace-test PRIVATE Umicom::exchange_module)
    umicom_apply_warnings(exchange-workspace-test)
    umicom_apply_sanitizers(exchange-workspace-test)
    add_test(NAME exchange.module.workspace
             COMMAND exchange-workspace-test)
endif()

install(TARGETS umicom_exchange_module
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if(TARGET umicom-exchange-console)
    install(TARGETS umicom-exchange-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/exchange)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    install(FILES LICENSE
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/exchange)
endif()

message(STATUS "Umicom Commodity Exchange Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Commodity Exchange uses Framework-owned application experience")

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
umicom_add_application_productisation_contribution(
    NAME "exchange"
    TARGET umicom_exchange_productisation
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")
