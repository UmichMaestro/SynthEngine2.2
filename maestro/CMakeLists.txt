include_directories(..)
if (WIN32)
    include_directories(../include)
endif (WIN32)

add_library(maestro_synth_lib MSEngine.h MSEngine.cpp MSInstrument.h MSInstrument.cpp MSModel.h MSModel.cpp)
add_executable(maestro_synth_test synth_main.cpp)

target_link_libraries(maestro_synth_test rtaudio_static ${LINKLIBS} maestro_synth_lib)
