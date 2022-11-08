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
	std::mt19937 rng{randutils::auto_seed_256{}.base()};
	std::cout << fallout::combat_dice::roll(rng, 10).to_report_string() << "\n";
	std::cout << fallout::combat_dice::from_dice_rolls({1, 2, 3, 4, 5, 6}).to_report_string() << "\n";
	std::cout << fallout::combat_dice::from_dice_rolls(1, 2U).to_report_string() << "\n";

	for (unsigned roll : fallout::roll_d20s(rng, 10)) {
		std::cout << roll << ' ';
	}
	std::cout << '\n';
	fallout::test_parameters param{8, 3, true};
	std::cout << fallout::d20_test(rng, param).successes();
	return EXIT_SUCCESS;
}