#include "fallout_dicelib/combat_dice.hpp"

#include <string>
#include <ostream>
#include <format>

namespace fallout_dicelib {

	auto operator<<(std::ostream& out, const combat_die_face& dc_face) -> std::ostream&
	{
		return out << to_string(dc_face);
	}

	auto to_string(const combat_die_face& dc_face) -> std::string
	{
		return std::format("[{} {}]", dc_face.damage, dc_face.effects);
	}

}

auto std::formatter<fallout_dicelib::combat_die_face, char>::parse(std::format_parse_context& ctx) -> decltype(ctx.begin()) {
	return formatter.parse(ctx);
}

auto std::formatter<fallout_dicelib::combat_die_face, char>::format(
	const fallout_dicelib::combat_die_face& dc_face,
	std::format_context& ctx
) const -> decltype(ctx.out()) 
{
	return formatter.format(fallout_dicelib::to_string(dc_face), ctx);
}