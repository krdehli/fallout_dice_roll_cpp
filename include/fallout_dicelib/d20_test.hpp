#ifndef FALLOUT_DICE_ROLL_CPP_D20_TEST_HPP_7F694A4D_1A49_47E2_954D_935DEEBD319C
#define FALLOUT_DICE_ROLL_CPP_D20_TEST_HPP_7F694A4D_1A49_47E2_954D_935DEEBD319C

#include <random>
#include <vector>
#include <algorithm>
#include <ranges>

#include <krsd/die.hpp>
#include <krsd/roller.hpp>

namespace fallout_dicelib {

using d20 = krsd::n_sided_die<unsigned, 20>;

enum class hit_location
{
	HEAD,
	TORSO,
	LEFT_ARM,
	RIGHT_ARM,
	LEFT_LEG,
	RIGHT_LEG
};

using hit_location_die = krsd::die<
	hit_location, 

	hit_location::HEAD,
	hit_location::HEAD,

	hit_location::TORSO,
	hit_location::TORSO,
	hit_location::TORSO,
	hit_location::TORSO,
	hit_location::TORSO,
	hit_location::TORSO,

	hit_location::LEFT_ARM,
	hit_location::LEFT_ARM,
	hit_location::LEFT_ARM,

	hit_location::RIGHT_ARM,
	hit_location::RIGHT_ARM,
	hit_location::RIGHT_ARM,

	hit_location::LEFT_LEG,
	hit_location::LEFT_LEG,
	hit_location::LEFT_LEG,

	hit_location::RIGHT_LEG,
	hit_location::RIGHT_LEG,
	hit_location::RIGHT_LEG
>;

enum class skill_tag : bool
{
	NOT_TAGGED = false,
	TAGGED = true
};

struct test_parameters {
	using enum skill_tag;

	unsigned int attribute;
	unsigned int skill;
	skill_tag tagged;

	constexpr 
	explicit test_parameters(
		unsigned int attribute = 0, 
		unsigned int skill = 0, 
		skill_tag tagged = NOT_TAGGED
	) noexcept : 
		attribute{attribute}, 
		skill{skill}, 
		tagged{tagged} 
	{}

	[[nodiscard]] constexpr 
	auto target_number() const -> unsigned int { return attribute + skill; }

	[[nodiscard]] constexpr 
	auto crit_threshold() const -> unsigned int {
		return (tagged == TAGGED) ? skill : 1; 
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
requires std::convertible_to<std::ranges::range_value_t<R>, krsd::roll_result<d20>>
[[nodiscard]] 
auto test_rolls(const test_parameters& params, R&& r) noexcept -> test_result {
	test_result result;

label:
	for (const auto& roll : r) {
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

template <krsd::random_integer_distribution Dist>
class generic_d20_test_roller 
{
public:
	generic_d20_test_roller() {}

	template <std::uniform_random_bit_generator Rng>
	[[nodiscard]]
	auto test(
		Rng& rng,
		const test_parameters& params,
		unsigned int num_rolls = 2
	) -> test_result
	{
		return test_rolls(params, roller_.roll(rng, num_rolls).rolls());
	}

private:
	krsd::generic_roller<Dist, d20> roller_;
};

using d20_test_roller = generic_d20_test_roller<std::uniform_int_distribution<std::size_t>>;

}

#endif