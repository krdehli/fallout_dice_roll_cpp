#include <catch.hpp>

#include <limits>
#include <type_traits>
#include <random>
#include <format>

#include <randutils.hpp>

#include <krsd/die.hpp>
#include <krsd/roller.hpp>
#include <fallout_dicelib/combat_dice.hpp>
#include <fallout_dicelib/d20_test.hpp>

TEST_CASE("DC literal") {
	const auto test_roll = krsd::roll_result<fallout_dicelib::combat_die>::literal<{0, 0}>();
	REQUIRE(test_roll.value().eyes() == 0);
}

TEST_CASE("D20 Tests") {
	fallout_dicelib::d20_test_roller roller;
	std::mt19937 rng{randutils::auto_seed_256{}.base()};

	const fallout_dicelib::test_parameters params{5, 2};
	const auto result = roller.test(rng, params, 2);
	REQUIRE(result.successes() < 5);
}