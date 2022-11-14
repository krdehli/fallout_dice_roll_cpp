#ifndef FALLOUT_DICE_ROLL_CPP_D20_TEST_HPP_7F694A4D_1A49_47E2_954D_935DEEBD319C
#define FALLOUT_DICE_ROLL_CPP_D20_TEST_HPP_7F694A4D_1A49_47E2_954D_935DEEBD319C

#include <random>
#include <vector>
#include <algorithm>
#include <ranges>

namespace fallout {

template <std::uniform_random_bit_generator Rng>
[[nodiscard]] 
auto roll_d20(Rng& rng) -> unsigned int {
	static std::uniform_int_distribution dist{1, 20};
	return dist(rng);
}

template <std::uniform_random_bit_generator Rng>
[[nodiscard]] 
auto roll_d20s(Rng& rng, unsigned int num) -> std::vector<unsigned int> {
	std::vector<unsigned int> rolls(num, 0);
	std::ranges::generate(rolls, [&]{ return roll_d20(rng); });
	return rolls;
}

struct test_parameters
{
	unsigned int attribute;
	unsigned int skill;
	bool tagged;

	constexpr 
	explicit test_parameters(
		unsigned int attribute = 0, 
		unsigned int skill = 0, 
		bool tagged = false
	) noexcept : 
		attribute{attribute}, 
		skill{skill}, 
		tagged{tagged} 
	{}

	[[nodiscard]] constexpr 
	auto target_number() const -> unsigned int { return attribute + skill; }

	[[nodiscard]] constexpr 
	auto crit_threshold() const -> unsigned int {
		return tagged ? skill : 1; 
	}
};

struct test_result 
{
	unsigned int regular_successes;
	unsigned int critical_successes;
	unsigned int critical_failures;

	constexpr 
	explicit test_result(
	    unsigned int regular_successes = 0, 
		unsigned int critical_successes = 0, 
		unsigned int critical_failures = 0
	) noexcept : 
		regular_successes{regular_successes}, 
		critical_successes{critical_successes}, 
		critical_failures{critical_failures}
	{}

	[[nodiscard]] constexpr 
	auto successes() const noexcept -> unsigned int {
		return regular_successes + 2 * critical_successes;
	}

	[[nodiscard]] constexpr 
	auto has_critical_success() const noexcept -> bool { 
		return critical_successes > 0;
	}

	[[nodiscard]] constexpr 
	auto has_critical_failure() const noexcept -> bool { 
		return critical_failures > 0;
	}

};

template <std::ranges::range R>
requires std::convertible_to<std::ranges::range_value_t<R>, unsigned int>
[[nodiscard]] 
auto test_rolls(const test_parameters& params, R&& r) noexcept -> test_result {
	test_result result;

	for (unsigned int roll : r) {
		if (roll == 20) {
			result.critical_failures += 1;
		}
		else if (roll <= params.crit_threshold()) {
			result.critical_successes += 1;
		}
		else if (roll <= params.target_number()) {
			result.regular_successes += 1;
		}
	}
	return result;
}

template <std::uniform_random_bit_generator Rng>
[[nodiscard]] 
auto d20_test(
	Rng& rng,
	const test_parameters& params, 
	unsigned int num_rolls = 2
) -> test_result {
	return test_rolls(params, roll_d20s(rng, num_rolls));
}

enum class hit_location
{
	HEAD,
	TORSO,
	LEFT_ARM,
	RIGHT_ARM,
	LEFT_LEG,
	RIGHT_LEG
};

template <std::uniform_random_bit_generator Rng>
[[nodiscard]] 
auto roll_hit_location(Rng& rng) -> hit_location {
	const auto roll = roll_d6(rng);

	if (roll < 3) {
		return hit_location::HEAD;
	}
	else if (roll < 9) {
		return hit_location::TORSO;
	}
	else if (roll < 12) {
		return hit_location::LEFT_ARM;
	}
	else if (roll < 15) {
		return hit_location::RIGHT_ARM;
	}
	else if (roll < 18) {
		return hit_location::LEFT_LEG;
	}
	else {
		return hit_location::RIGHT_LEG;
	}
}

}

#endif