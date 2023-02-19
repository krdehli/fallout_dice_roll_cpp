#ifndef FALLOUT_DICE_ROLL_CPP_INTERPRETER_HPP_2A891C49_A68A_415E_AE8A_FC3662B6D113
#define FALLOUT_DICE_ROLL_CPP_INTERPRETER_HPP_2A891C49_A68A_415E_AE8A_FC3662B6D113

#include <string>
#include <vector>
#include <variant>

#include <krsd/die.hpp>

#include "combat_dice.hpp"
#include "d20_test.hpp"

namespace fallout_dicelib {

using combat_die_roll_result = krsd::accumulated_roll_result<fallout_dicelib::combat_die>;
using d20_roll_result = krsd::accumulated_roll_result<fallout_dicelib::d20>;
using hit_location_roll_result = krsd::roll_result<fallout_dicelib::hit_location_die>;

using fallout_roll_result = std::variant<combat_die_roll_result, d20_roll_result, hit_location_roll_result>;

enum class command_name { 
	ROLL, 
	REROLL,
	CONVERT_D6_TO_DC
};

enum class command_argument_type {
	INTEGER,
	POSITIVE_INTEGER,
	DICE_ROLL
};

struct command_argument {
	command_argument_type type;
	std::size_t count;
	static constexpr std::size_t infinite_count = 0;
};

struct command
{
	command_name name;
	std::vector<command_argument> arguments;
};

class interpreter {
public:

private:
	std::vector<fallout_roll_result> history_;
};

}

#endif

