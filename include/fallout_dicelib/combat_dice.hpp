#ifndef FALLOUT_COMBAT_DICE_CPP_COMBAT_DICE_HPP_217973CF_1F39_4181_AB51_E47DCD4460B5
#define FALLOUT_COMBAT_DICE_CPP_COMBAT_DICE_HPP_217973CF_1F39_4181_AB51_E47DCD4460B5

#include <cassert>
#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <concepts>
#include <iterator>
#include <ranges>
#include <numeric>
#include <random>

#include <krsd/die.hpp>

namespace fallout_dicelib {

struct combat_die_face {
	unsigned damage;
	unsigned effects;

	constexpr 
	explicit combat_die_face(unsigned damage = 0, unsigned effects = 0) noexcept : 
		damage{damage}, 
		effects{effects} 
	{}

	[[nodiscard]] constexpr
	auto eyes() const noexcept -> unsigned {
		return damage;
	}

	[[nodiscard]] constexpr 
	friend auto operator+(const combat_die_face& lhs, const combat_die_face& rhs) noexcept
	    -> combat_die_face {
		return combat_die_face{lhs.damage + rhs.damage, lhs.effects + rhs.effects};
	}

	constexpr 
	auto operator+=(const combat_die_face& rhs) noexcept -> combat_die_face& {
		damage += rhs.damage;
		effects += rhs.effects;
		return *this;
	}

	friend auto operator<=>(const combat_die_face&, const combat_die_face&) = default;

	friend auto operator<<(std::ostream& out, const combat_die_face& dc_face) -> std::ostream&;
};

[[nodiscard]] 
auto to_string(const combat_die_face& dc_face) -> std::string;

using combat_die = krsd::die<
	combat_die_face, 
	combat_die_face{1, 0},
	combat_die_face{2, 0},
	combat_die_face{0, 0},
	combat_die_face{0, 0},
	combat_die_face{1, 1},
	combat_die_face{1, 1}
>;
}


template <>
class std::formatter<fallout_dicelib::combat_die_face, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin());
	auto format(
		const fallout_dicelib::combat_die_face& dc_face, 
		std::format_context& ctx
	) const -> decltype(ctx.out());

private:
	mutable std::formatter<std::string> formatter;
};


#endif