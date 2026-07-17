include(FetchContent)

#File CMakeLists.txt for dependencies

#Generation logs 
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

#Disassembler library
FetchContent_Declare(zydis
    GIT_REPOSITORY https://github.com/zyantific/zydis.git
    GIT_TAG v4.1.0
)
FetchContent_MakeAvailable(zydis)