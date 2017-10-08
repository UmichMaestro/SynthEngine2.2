include_directories(..)
if (WIN32)
    include_directories(../include)
endif (WIN32)

add_executable(maestro synth.cpp)
target_link_libraries(maestro rtaudio_static ${LINKLIBS})
