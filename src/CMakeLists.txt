set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -Wl,-rpath,./  -lpthread")

FIND_PACKAGE( OpenCV REQUIRED )
IF (${OpenCV_VERSION} VERSION_LESS 2.4.1)
    MESSAGE(FATAL_ERROR "OpenCV version is not compatible : ${OpenCV_VERSION}. FaceRec requires atleast OpenCV v2.4.1")
ENDIF()

set(SOURCE_FILES main.cpp RedisInput.h RedisInput.cpp ./lib/json.hpp Log.h Log.cpp FaceRecognize.cpp FaceRecognize.h test.h  Output.h Config.h Output.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries( ${PROJECT_NAME}  boost_system)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  ${OpenCV_LIBS} )
target_link_libraries( ${PROJECT_NAME}  libhiredis.so)

