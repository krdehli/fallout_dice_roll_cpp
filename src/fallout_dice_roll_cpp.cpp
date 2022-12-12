#include <cstdlib>
#include <random>
#include <iostream>
#include <format>

#include <randutils.hpp>
#include <boost/program_options.hpp>

#include "combat_dice.hpp"
#include "d20_test.hpp"

int main(int argc, char* argv[])
{
	namespace fd = fallout_dicelib;
	using enum fd::skill_tag;

	std::mt19937 rng{randutils::auto_seed_256{}.base()};
	std::cout << to_report_string(fd::combat_dice::roll(rng, 10)) << "\n";
	std::cout << to_report_string(fd::combat_dice::from_dice_rolls({1, 2, 3, 4, 5, 6})) << "\n";
	std::cout << to_report_string(fd::combat_dice::from_dice_rolls(1, 2U)) << "\n";

	for (unsigned roll : fd::roll_d20s(rng, 10)) {
		std::cout << roll << ' ';
	}
	std::cout << '\n';
	fd::test_parameters param{8, 3, NOT_TAGGED};
	std::cout << fd::d20_test(rng, param).successes();
	return EXIT_SUCCESS;
}