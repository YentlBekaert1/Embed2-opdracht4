cmake_minimum_required(VERSION 3.10)
# set the project name
project(Opdracht4)

#target_link_libraries(client /home/pi/embed2/tlpi-dist/libtlpi.a)
#target_include_directories(server PRIVATE /home/pi/embed2/tlpi-dist/dist)

#link libtlpi.a library
link_libraries(libtlpi.a)





#link PJI_RPI library
add_library(PJ_RPI PJ_RPI.c)
install (TARGETS PJ_RPI DESTINATION lib)
install (FILES PJ_RPI.h DESTINATION include)
link_libraries(PJ_RPI)

# add the executables
add_executable(client ud_ucase_cl.c)
add_executable(server ud_ucase_sv.c)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)