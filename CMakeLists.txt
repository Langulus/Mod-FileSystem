cmake_minimum_required(VERSION 3.28)
project(LangulusModFileSystem
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for reading/writing files and folders"
    HOMEPAGE_URL    https://langulus.com
)

# Configure PhysFS library, it will be statically built inside this module      
set(PHYSFS_BUILD_TEST FALSE CACHE BOOL "Don't allow tests to be built" FORCE)
set(PHYSFS_DISABLE_INSTALL TRUE CACHE BOOL "Don't allow install" FORCE)
set(PHYSFS_BUILD_DOCS FALSE CACHE BOOL "Don't allow building docs" FORCE)
set(PHYSFS_BUILD_SHARED FALSE CACHE BOOL "Don't allow building shared library" FORCE)

fetch_external_module(
    PhysFS
    GIT_REPOSITORY  https://github.com/icculus/physfs.git
    GIT_TAG         0cd18b8 # main
)

file(GLOB_RECURSE
	LANGULUS_MOD_FILESYSTEM_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp
)

# Build the module                                                              
add_langulus_mod(LangulusModFileSystem ${LANGULUS_MOD_FILESYSTEM_SOURCES})

target_link_libraries(LangulusModFileSystem PRIVATE physfs-static)

target_include_directories(LangulusModFileSystem
    PRIVATE     ${PhysFS_SOURCE_DIR}
)

# Make the write and read data dir for PhysFS, because it doesn't have access   
add_custom_command(
    TARGET LangulusModFileSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
		"$<TARGET_FILE_DIR:LangulusModFileSystem>/data"
)

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()