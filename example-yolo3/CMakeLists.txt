cmake_minimum_required(VERSION 3.3)
project(example-yolo3)
set(APP_NAME example-yolo3)

message( STATUS "---- nono building : ${APP_NAME} | ${CMAKE_BUILD_TYPE}" )

# ===================== oF Directory =====================
set(OF_DIRECTORY /home/nono/Documents/workspaces/openframeworks/of_v0.10.1_linux64gcc5_release)


# ===================== Compiler Flags =====================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# =========== Set Global Properties ==============
include(addons)
include(linux64)

# ===================== Source Files =====================
set(SOURCE_FILES
        ${ADDON_SOURCE_FILES}
        "scripts/BuildNumber.txt"
        "src/main.cpp"
        "src/ofApp.cpp"
        "src/ofApp.cpp"
        )


# ===================== System Frameworks =====================

# =============================================================

# ===================== Include Dirs =====================

include_directories(src)

# ========================================================


# ===================== Libs =====================

# ================================================

# ============ Add Build Version ==================


macro(UPDATE_BUILD_NUMBER _file)

    if(NOT EXISTS ${_file})
        set(BUILD_NUMBER "1")
    elseif()
        file (STRINGS ${_file} BUILD_NUMBER)
    endif()
    message ( STATUS "BUILD NUMBER : ${BUILD_NUMBER}")
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1" )
    message ( STATUS "BUILD NUMBER : ${BUILD_NUMBER}")
    file(WRITE ${_file} ${BUILD_NUMBER})

endmacro()

##add_custom_command(
#add_custom_target(
#        THIS_IS_IT
##        TARGET ${APP_NAME} PRE_BUILD
##        BUILD_VERSION
##        message( STATUS "OK LET'S SEE IF THIS WORKS" )
##        OUTPUT ${PROJECT_SOURCE_DIR}/BuildNumber.txt
##        PRE_BUILD
#        UPDATE_BUILD_NUMBER( ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt )
#        COMMAND ${PROJECT_SOURCE_DIR}/scripts/incrementBuildNumber.sh ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt
##        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_config_h.py ${CMAKE_CURRENT_BINARY_DIR}/config.h
#)
#
#add_custom_target(ALWAYS_DO_POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo touching ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt
#        UPDATE_BUILD_NUMBER ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt
##        COMMAND ${CMAKE_COMMAND} -E echo BUILD_NUMBER ${BUILD_NUMBER}
#        COMMAND /bin/bash ${PROJECT_SOURCE_DIR}/scripts/incrementBuildNumber.sh ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt
##        COMMAND ${CMAKE_COMMAND} -E awk '{ printf \"%04d\n\", $1+2 }'  ${PROJECT_SOURCE_DIR}/BuildNumber.txt
##        COMMAND ${CMAKE_COMMAND} "-E awk -F, '{printf(\"%s,%d\n\",$1,$2+1)}'  ${PROJECT_SOURCE_DIR}/BuildNumber.txt"
#        #        COMMAND ${CMAKE_COMMAND} -E touch ${PROJECT_SOURCE_DIR}/BuildNumber.txt
#        )

#add_custom_command ( OUTPUT BuildNumber.txt
#        file (STRINGS "BuildNumber.txt" BUILD_NUMBER)
#        message ( STATUS "BUILD NUMBER : ${BUILD_NUMBER}")
#        math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1" )
#        message ( STATUS "BUILD NUMBER : ${BUILD_NUMBER}")
#        file(WRITE "BuildNumber.txt" ${BUILD_NUMBER} )
#        )
##        COMMAND ${PROJECT_SOURCE_DIR}/scripts/incrementBuildNumber.sh
##        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#
#add_custom_target(
#        buildVersionTarget
#        BuildNumber.txt
##        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_config_h.py ${CMAKE_CURRENT_BINARY_DIR}/config.h
#)

add_custom_target(BUILDNUMBER_INCREASE
        set(BUILDNUMBER_FILE ${PROJECT_SOURCE_DIR}/scripts/BuildNumber.txt)
        COMMAND ${CMAKE_COMMAND} -E echo touching ${BUILDNUMBER_FILE}
        UPDATE_BUILD_NUMBER ${BUILDNUMBER_FILE}
        COMMAND /bin/bash ${PROJECT_SOURCE_DIR}/scripts/incrementBuildNumber.sh ${BUILDNUMBER_FILE}
        )


# ============ Compile and Link ==================


link_directories( ${OF_ADDON_LINK_DIRS} )

add_executable(${APP_NAME} ${SOURCE_FILES})
add_dependencies(${APP_NAME} of_shared)
target_link_libraries(${APP_NAME} $<TARGET_FILE:of_shared>)
target_link_libraries(${APP_NAME} ${OF_CORE_LIBS} ${OF_ADDON_LIBS} )
if (DEFINED CMAKE_BUILD_TYPE AND ${CMAKE_BUILD_TYPE} STREQUAL "Release" )
    add_dependencies(${APP_NAME} BUILDNUMBER_INCREASE)
endif()

set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
