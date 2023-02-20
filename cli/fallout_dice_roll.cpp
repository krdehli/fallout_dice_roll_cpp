#include <cstdlib>
#include <cstdio>
#include <random>
#include <iostream>
#include <format>
#include <exception>
#include <typeinfo>

#include <randutils.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include <krsd/bounded_numbers.hpp>
#include <krsd/die.hpp>
#include <fallout_dicelib/combat_dice.hpp>
#include <fallout_dicelib/d20_test.hpp>
#include <fallout_dicelib/interpreter.hpp>
#include <fallout_dicelib/lexer.hpp>

namespace krsd {
	BOOST_DESCRIBE_ENUM(bounded_number_error, VALUE_LESS_THAN_LOWER_BOUND, VALUE_GREATER_THAN_UPPER_BOUND);
	BOOST_DESCRIBE_ENUM(roll_result_error, INVALID_DIE_FACE);
}

namespace fallout_dicelib {
	BOOST_DESCRIBE_ENUM(skill_tag, NOT_TAGGED, TAGGED);
}

auto main() -> int {
	fallout_dicelib::interpreter interpreter;
	const auto result = fallout_dicelib::lex("roll 1dc");
	return EXIT_SUCCESS;
}