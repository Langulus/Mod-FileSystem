cmake_minimum_required(VERSION 3.24)
project(LangulusModFileSystem
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for reading/writing files and folders"
    HOMEPAGE_URL    https://langulus.com
)

# Configure PhysFS library, it will be statically built inside this module      
fetch_external_module(
    PhysFS
    GIT_REPOSITORY  https://github.com/icculus/physfs.git
    GIT_TAG         0cd18b8 # main
)

# Build the module                                                              
add_library(LangulusModFileSystem SHARED
    source/FileSystem.cpp
    source/File.cpp
    source/Folder.cpp
)

target_link_libraries(LangulusModFileSystem 
    PRIVATE     Langulus
                physfs-static
)

target_include_directories(LangulusModFileSystem
    PRIVATE     ${PhysFS_SOURCE_DIR}
)

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()