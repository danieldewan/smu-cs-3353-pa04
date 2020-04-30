cmake_minimum_required(VERSION 3.15)
project(smu_cs_3353_pa04)

set (EXE_NAME Project4)

##########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "train_data.csv")
#   set (input02 "train_target.csv")
# etc....

set (input01 "control.txt")
set (input02 "graph_1.kml")
set (input03 "graph_2.kml")
set (input04 "graph_3.kml")
set (input05 "graph_4.kml")
set (input06 "graph_5.kml")
set (input07 "graph_6.kml")
set (input08 "graph_7.kml")
set (input09 "graph_8.kml")
set (input10 "graph_9.kml")
set (input11 "graph_10.kml")


#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03 input04 input05 input06 input07 input08 input09 input10 input11)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)
add_executable(${EXE_NAME} main.cpp)