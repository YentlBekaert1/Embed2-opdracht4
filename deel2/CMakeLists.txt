cmake_minimum_required(VERSION 3.10)
# set the project name
project(Opdracht4)

#link PJI_RPI library
add_library(PJ_RPI PJ_RPI.c)
install (TARGETS PJ_RPI DESTINATION lib)
install (FILES PJ_RPI.h DESTINATION include)
link_libraries(PJ_RPI)

# add the executables
add_executable(a.out http_server.c)



