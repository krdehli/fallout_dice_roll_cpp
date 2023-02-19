cmake_minimum_required(VERSION 3.21)

find_package(Catch2 CONFIG REQUIRED)

add_executable(tests test.cpp)
target_include_directories(tests
	PRIVATE
		${randutils_SOURCE_DIR}
)
target_link_libraries(tests
	PRIVATE
		Catch2::Catch2WithMain
		fallout_dice_roll_lib
)

include(Catch)
catch_discover_tests(tests)