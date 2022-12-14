cmake_minimum_required(VERSION 3.15)

project(fallout_dice_roll_cpp
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

add_library(common_options INTERFACE)
target_compile_features(common_options
	INTERFACE
		cxx_std_20
)

add_library(common_warnings INTERFACE)
target_compile_options(common_warnings
	INTERFACE
		$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
			-Wall
            -Wextra # reasonable and standard
            -Wshadow # warn the user if a variable declaration shadows one from a parent context
            -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
                               # catch hard to track down memory errors
		    -Wold-style-cast # warn for c-style casts
            -Wcast-align # warn for potential performance problem casts
            -Wunused # warn on anything being unused
            -Woverloaded-virtual # warn if you overload (not override) a virtual function
            -Wpedantic # warn if non-standard C++ is used
            -Wconversion # warn on type conversions that may lose data
            -Wsign-conversion # warn on sign conversions
            -Wnull-dereference # warn if a null dereference is detected
            -Wdouble-promotion # warn if float is implicit promoted to double
            -Wformat=2 # warn on security issues around functions that format output (ie printf)
		>
		$<$<CXX_COMPILER_ID:GNU>:
			-Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
            -Wduplicated-cond # warn if if / else chain has duplicated conditions
            -Wduplicated-branches # warn if if / else branches have duplicated code
            -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
            -Wuseless-cast # warn if you perform a cast to the same type
		>
		$<$<CXX_COMPILER_ID:MSVC>:
			/W4 # Baseline reasonable warnings
		    /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
		    /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
		    /w14263 # 'function': member function does not override any base class virtual member function
		    /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
				    # be destructed correctly
		    /w14287 # 'operator': unsigned/negative constant mismatch
		    /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
				    # the for-loop scope
		    /w14296 # 'operator': expression is always 'boolean_value'
		    /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
		    /w14545 # expression before comma evaluates to a function which is missing an argument list
		    /w14546 # function call before comma missing argument list
		    /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
		    /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
		    /w14555 # expression has no effect; expected expression with side- effect
		    /w14619 # pragma warning: there is no warning number 'number'
		    /w14640 # Enable warning on thread un-safe static member initialization
		    /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
		    /w14905 # wide string literal cast to 'LPSTR'
		    /w14906 # string literal cast to 'LPWSTR'
		    /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
		    /permissive- # standards conformance mode for MSVC compiler.
		>
)

find_package(Boost REQUIRED 
	COMPONENTS
		program_options
)

# C/C++ Library
add_library(dicelib 
	include/fallout_dicelib/combat_dice.hpp
    src/combat_dice.cpp
	include/fallout_dicelib/d20_test.hpp
	src/d20_test.cpp
	include/fallout_dicelib/convenience.hpp
	src/convenience.cpp
)
add_library(fallout_dice_roll::dicelib ALIAS dicelib)

target_compile_features(dicelib
	PUBLIC
		cxx_std_20
)
target_include_directories(dicelib
	PUBLIC
		include/fallout_dicelib
	PRIVATE
		deps/randutils
)
target_link_libraries(dicelib 
	PRIVATE
		common_options
		common_warnings
)

# CLI exectuable
add_executable(fallout_dice_roll_cpp
	src/fallout_dice_roll_cpp.cpp
 )
target_compile_features(fallout_dice_roll_cpp
	PRIVATE
		cxx_std_20
)
target_include_directories(fallout_dice_roll_cpp
	PRIVATE
		deps/randutils
)
target_link_libraries(fallout_dice_roll_cpp
	PRIVATE
		common_options
		common_warnings
		dicelib
		Boost::boost
)

