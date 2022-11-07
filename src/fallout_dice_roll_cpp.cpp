#include <cstdlib>
#include <random>
#include <iostream>
#include <format>

#include <randutils.hpp>
#include <boost/program_options.hpp>

#include "combat_dice.hpp"

int main(int argc, char* argv[])
{
	std::mt19937 rng{randutils::auto_seed_256{}.base()};
	std::cout << to_report_string(fallout::combat_dice::roll(rng, 1));

	return EXIT_SUCCESS;
}