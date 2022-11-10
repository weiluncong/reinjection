cmake_minimum_required(VERSION 3.5)
project(reinjection)

string(TIMESTAMP COMPILE_TIME %m-%d-%H:%M) # 年月日时分妙: %Y%m%d%H%M%S
#message(STATUS "build time: ${COMPILE_TIME}")

# set(COMPILE_BY_DOCKER OFF)
# if(NOT COMPILE_BY_DOCKER)
#     set(PYTHON_VERSION python3.6m)
#     message("python3.6m")
# else()
#     set(PYTHON_VERSION python3.7m)
#     message("python3.7m")
link_directories(${CMAKE_SOURCE_DIR}/third_party/docker/GL)
# endif()

add_compile_options(-fPIC -g -std=c++11 -Wall)
add_definitions(-w)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third_party/Qt)
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/third_party/OpenCV)

set(CPACK_GENERATOR "DEB")
# DPKG执行顺序
# 1.正常安装: preinst->unpack->set up->postinst
# 2.正常卸载: remove->prerm->postrm
# 3.覆盖安装: prerm->preinst->unpack->postrm->set up->postinst
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/postinst;${CMAKE_SOURCE_DIR}/prerm;")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_INSTALL_PREFIX /opt/)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_CONTACT "cwl")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PROJECT_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${COMPILE_TIME}")

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)
set(QT_USE_QTOPENGL TRUE)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/mybuild)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/mybuild)

# configure_file(
#     ${CMAKE_SOURCE_DIR}/src/version.h.in
#     ${CMAKE_SOURCE_DIR}/src/version.h
# )

# 添加工程依赖库及获取依赖库地址
list(APPEND QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Charts Qt5::OpenGL OpenCV)
foreach(lib ${QT_LIBS})
    if(${lib} MATCHES ".*:.*")
        string(REGEX REPLACE ".*:(.*)" "\\1" com ${lib})
        find_package(Qt5 COMPONENTS ${com} REQUIRED)
        list(APPEND mlibs ${lib})
    else()
        find_package(${lib} REQUIRED)
        list(APPEND mlibs ${${lib}_LIBS})
        list(APPEND third_libs_include_path ${${lib}_INCLUDE_DIRS})
    endif()
endforeach()

# 循环遍历文件夹，获取cpp文件以及头文件地址用于include
Function(getRecurseFileInfo rootPath cppList dirList)
    FILE(GLOB_RECURSE srcList "${rootPath}/*.cpp") 

    FOREACH(src ${srcList})
        #use regular expressions to extract the file path 
        string(REGEX REPLACE "(.*)/.*cpp$" "\\1" dirName ${src})
        #only extract the file path under rootPath folder
        if((dirName MATCHES ".*${rootPath}.*"))
            LIST(APPEND dirList ${dirName})
            LIST(APPEND cppList ${src})
        endif()
    ENDFOREACH()

    LIST(REMOVE_DUPLICATES dirList)

    set(${dirList} PARENT_SCOPE)
    set(${cppList} PARENT_SCOPE)
endfunction()

getRecurseFileInfo(./core srcList srcDirList)

# qt5_add_resources(QRC_SOURCES  src/icon.qrc)

list(APPEND mlibs caframework protobuf gflags glog kpb zmq_cm zmq pthread yaml-cpp python3.7m )
# list(APPEND mlibs boost_filesystem boost_program_options boost_system boost_thread)# boost
list(APPEND includePath 
            ${srcDirList}   # 优先包含项目的文件
            ${CMAKE_SOURCE_DIR}/core/include
            /opt/capilot/include
            /opt/capilot/include/third_party
            /opt/capilot/include/third_party/google
            # ${CMAKE_SOURCE_DIR}/src/common/interface/horizon 
	    ${CMAKE_SOURCE_DIR}/third_party
            ${CMAKE_SOURCE_DIR}/third_party/yaml
            ${CMAKE_SOURCE_DIR}/third_party/Qt/include
        #     ${CMAKE_SOURCE_DIR}/third_party/OpenCV/include
        #     ${CMAKE_SOURCE_DIR}/third_party/eigen3
	    # ${CMAKE_SOURCE_DIR}/third_party/mdf4
            ${CMAKE_SOURCE_DIR}/third_party/pack
            # ${CMAKE_SOURCE_DIR}/third_party/Ogre
            # ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE
            # ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE/Overlay
            # ${CMAKE_SOURCE_DIR}/third_party/Boost
            # ${CMAKE_SOURCE_DIR}/third_party/pcan
            ${CMAKE_SOURCE_DIR}/third_party/python3.7m/include
            )

include_directories(${includePath})
link_directories(/opt/capilot/lib
                 /opt/${PROJECT_NAME}/lib
                 ${CMAKE_SOURCE_DIR}/third_party/yaml
                 ${CMAKE_SOURCE_DIR}/third_party/Qt/lib
                #  ${CMAKE_SOURCE_DIR}/third_party/OpenCV/lib
		#  ${CMAKE_SOURCE_DIR}/third_party/mdf4
                 ${CMAKE_SOURCE_DIR}/third_party/pack
                #  ${CMAKE_SOURCE_DIR}/third_party/Ogre
                #  ${CMAKE_SOURCE_DIR}/third_party/Boost
                #  ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE-1.9.0
                #  ${CMAKE_SOURCE_DIR}/third_party/pcan
                 ${CMAKE_SOURCE_DIR}/third_party/python3.7m/lib
                 )

list(REMOVE_ITEM srcList ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_library(${PROJECT_NAME} SHARED ${srcList} ${QRC_SOURCES})
target_link_libraries(${PROJECT_NAME} ${mlibs})

file(GLOB_RECURSE INC  "core/include/*.h")

add_executable(${PROJECT_NAME}_ ${CMAKE_SOURCE_DIR}/core/src/main.cpp ${QRC_SOURCES} ${INC})
target_link_libraries(${PROJECT_NAME}_ ${PROJECT_NAME} ${mlibs})


# FILE(GLOB_RECURSE script "${CMAKE_SOURCE_DIR}/script/*.sh")
# install(DIRECTORY src/ DESTINATION ${PROJECT_NAME}/src)
# install(DIRECTORY third_party/ DESTINATION ${PROJECT_NAME}/third_party/)
# install(DIRECTORY dbc/ DESTINATION ${PROJECT_NAME}/dbc)
# install(FILES ${script} DESTINATION ${PROJECT_NAME})
# install(FILES CMakeLists.txt DESTINATION ${PROJECT_NAME})
# install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}/third_party)
# install(TARGETS ${PROJECT_NAME}_ DESTINATION ${PROJECT_NAME})
# include(InstallRequiredSystemLibraries)
# include(CPack)
