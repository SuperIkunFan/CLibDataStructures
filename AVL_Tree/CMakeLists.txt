cmake_minimum_required(VERSION 3.1)
set(PROJ_NAME ca7_datalog)
project(${PROJ_NAME})


add_executable(datalog_decoder ${PROJECT_SOURCE_DIR}/main.cpp)