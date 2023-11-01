file(GLOB_RECURSE
	LANGULUS_MOD_FILESYSTEM_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModFileSystemTest ${LANGULUS_MOD_FILESYSTEM_TEST_SOURCES})

target_link_libraries(LangulusModFileSystemTest
    PRIVATE		Langulus
                Catch2
)

add_test(
    NAME		LangulusModFileSystemTest
    COMMAND		LangulusModFileSystemTest
)

langulus_copy_dlls(LangulusModFileSystemTest ON LangulusModFileSystemTest)
langulus_copy_dlls_advanced(LangulusModFileSystemTest
    TO			LangulusModFileSystemTest
    FROM		LangulusModFileSystem
)

# Make the write and read data dir for PhysFS, because it doesn't have access   
add_custom_command(
    TARGET LangulusModFileSystemTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LangulusModFileSystemTest>/data/test folder"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test.txt" "$<TARGET_FILE_DIR:LangulusModFileSystemTest>/data/test.txt"
)