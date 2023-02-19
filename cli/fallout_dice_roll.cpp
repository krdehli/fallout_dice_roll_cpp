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

namespace krsd {
	BOOST_DESCRIBE_ENUM(bounded_number_error, VALUE_LESS_THAN_LOWER_BOUND, VALUE_GREATER_THAN_UPPER_BOUND);
	BOOST_DESCRIBE_ENUM(roll_result_error, INVALID_DIE_FACE)
}

auto main() -> int {
	boost::mp11::mp_for_each<boost::describe::describe_enumerators<krsd::bounded_number_error>>([](auto d) {
		spdlog::debug(d.name);
	});
	return EXIT_SUCCESS;
}