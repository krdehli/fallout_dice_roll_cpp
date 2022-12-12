#ifndef FALLOUT_COMBAT_DICE_CPP_CONVENIENCE_HPP_565822CC_61AE_47C2_B1AE_9C7512C16662
#define FALLOUT_COMBAT_DICE_CPP_CONVENIENCE_HPP_565822CC_61AE_47C2_B1AE_9C7512C16662

#include <cstddef>
#include <cstdint>
#include <random>

#include <randutils.hpp>

#include "combat_dice.hpp"
#include "d20_test.hpp"

namespace fallout_dicelib {

template <
	std::uniform_random_bit_generator Rng = std::mt19937,
	std::size_t Seed_count = 8,
	typename Seed_rep = std::uint32_t
>
class dice_roller 
{
public:
	using generator_type = Rng;
	using seed_seq_type = randutils::seed_seq_fe<Seed_count, Seed_rep>;

	dice_roller() : 
		rng_{randutils::auto_seeded<seed_seq_type>{}.base()} 
	{}

	[[nodiscard]] 
	auto roll_combat_dice(unsigned int num) { 
		return combat_dice::roll(rng_, num);
	}

	[[nodiscard]] 
	auto roll_d20() { 
		return fallout_dicelib::roll_d20(rng_);
	}

	[[nodiscard]] 
	auto roll_d20s(unsigned int num) { 
		return fallout_dicelib::roll_d20s(rng_, num);
	}

	[[nodiscard]] 
	auto d20_test(
		const test_parameters& params, 
		unsigned int num_rolls = 2
	) {
		return fallout_dicelib::d20_test(rng_, params, num_rolls);
	}

	[[nodiscard]] 
	auto roll_hit_location() { 
		return fallout_dicelib::roll_hit_location(rng_);
	}

private:
	generator_type rng_;
};

}

#endif