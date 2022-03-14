cmake_minimum_required(VERSION 3.5)

project(car-plate-detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# NOTE: need correct version of libcharls-dev
# opencv >= 4.5 (debian 11) or >= 4.2 (ubuntu 20.04)
find_package(OpenCV 4.2.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# NOTE: debian 11
set(OpenCV_DATAFILES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/data/opencv4/)

# NOTE: libtesseract >= 4.1 and packages: tesseract-ocr-eng tesseract-ocr-rus
# NOTE: whitelist and blacklist not supported for LSTM
# https://github.com/tesseract-ocr/tesseract/issues/751
find_package(Tesseract 4.1)
if (NOT ${Tesseract_FOUND})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET GLOBAL tesseract lept)
    add_library(Tesseract::libresseract ALIAS PkgConfig::Tesseract)

    set(Tesseract_LIBRARIES Tesseract::libresseract)
    set(Tesseract_INCLUDE_DIRS)
endif()
# NOTE: To training tesseract data see 'create-training-data.sh'

include_directories(${Tesseract_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# NOTE: need russian locale - ru_RU.utf8
find_package(Boost 1.52 REQUIRED locale)

find_package(Qt5 5.9 COMPONENTS Widgets REQUIRED)
set(Qt5_LIBRARIES Qt5::Widgets)

set(MAIN_SRC_FILE
    src/main.cpp)

set(SRC_FILES
    src/base/carplatenumberdetector.cpp
    src/misc/ocr.cpp
    src/carplatenumberdetector_russian.cpp
)
set(HEADER_FILES
    src/base/carplatenumberdetector.h
    src/misc/logger.h
    src/misc/ocr.h
    src/carplatenumberdetector_russian.h
)
set(PROJECT_COMPILE_DEFINITIONS
    -DHAARCASCADE_CAR_PLATE_NUMBER_RUS=\"${OpenCV_DATAFILES_PATH}/haarcascades/haarcascade_russian_plate_number.xml\"
    -DSOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${MAIN_SRC_FILE} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Tesseract_LIBRARIES} ${Boost_LIBRARIES}
    ${Qt5_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILE_DEFINITIONS})

if (NOT TESTS_SKIP)
    find_package(GTest REQUIRED)

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        list(APPEND PROJECT_TEST_COMPILE_FLAGS -g)
    endif()

    if (UNIX)
        list(APPEND PROJECT_TEST_COMPILE_FLAGS -fno-omit-frame-pointer -fsanitize=address)
        list(APPEND PROJECT_TEST_LINK_FLAGS -lpthread)
    endif ()

    set(TEST_SRC_FILES
        tests/tst_carplatenumber.cpp)

    set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)

    add_executable(${PROJECT_NAME_TEST} ${SRC_FILES} ${TEST_SRC_FILES} ${HEADER_FILES})
    target_compile_options(${PROJECT_NAME_TEST} PRIVATE ${PROJECT_TEST_COMPILE_FLAGS})
    target_link_libraries(${PROJECT_NAME_TEST} -lasan # libasan preload
        ${OpenCV_LIBS} ${Tesseract_LIBRARIES} ${Boost_LIBRARIES} ${Qt5_LIBRARIES}
        ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${PROJECT_TEST_LINK_FLAGS})
    target_compile_definitions(${PROJECT_NAME_TEST} PRIVATE ${PROJECT_COMPILE_DEFINITIONS})
    target_include_directories(${PROJECT_NAME_TEST} PRIVATE src)
endif()
