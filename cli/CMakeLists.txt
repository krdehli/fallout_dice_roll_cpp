cmake_minimum_required(VERSION 3.21)

find_package(Boost REQUIRED 
	COMPONENTS
		program_options
)
find_package(spdlog CONFIG REQUIRED)

add_executable(fallout_dice_roll
    fallout_dice_roll.cpp
)
target_compile_features(fallout_dice_roll
    PRIVATE
        cxx_std_23
)
target_include_directories(fallout_dice_roll
    PRIVATE
        ${randutils_SOURCE_DIR}
)
target_link_libraries(fallout_dice_roll
    PRIVATE
        common_warnings
		fallout_dice_roll_lib
        Boost::boost
        Boost::program_options
        spdlog::spdlog
)