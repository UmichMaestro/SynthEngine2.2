include_directories(..)
if (WIN32)
    include_directories(../include)
endif (WIN32)

add_library(maestro_synth_lib synth.h synth.cpp MSM.h MSM.cpp)
add_executable(maestro_synth_test synth_main.cpp)

target_link_libraries(maestro_synth_test rtaudio_static ${LINKLIBS} maestro_synth_lib)
