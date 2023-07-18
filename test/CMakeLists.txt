project(LangulusModFileSystemTest)

add_executable(LangulusModFileSystemTest
	Main.cpp
	TestFileSystem.cpp
)

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