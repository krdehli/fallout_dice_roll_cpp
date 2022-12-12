#include "combat_dice.hpp"

namespace fallout_dicelib {

[[nodiscard]] 
auto to_string(const roll_result& r) -> std::string {
	std::string result;
	result.reserve(25);
	result += "Damage: ";
	result += std::to_string(r.damage);
	result += ", Effects: ";
	result += std::to_string(r.effects);
	return result;
}

auto operator<<(std::ostream& os, const roll_result& r) -> std::ostream& { 
	return os << to_string(r); 
}

[[nodiscard]] 
auto to_string(const accumulated_roll_result& ar) -> std::string { return to_string(ar.total()); }


auto operator<<(std::ostream& os, const accumulated_roll_result& ar) -> std::ostream& { 
	return os << to_string(ar); 
}

[[nodiscard]]
auto to_report_string(const accumulated_roll_result& ar) -> std::string { 
	std::string report = "Rolls: "; 
	for (const auto& r : ar.rolls()) {
		report += std::format("[{} {}] ", r.damage, r.effects);
	}
	report += '\n';
	report += std::format("Total damage: {}\n", ar.total().damage);
	if (ar.effect_triggered()) {
		report += std::format("Total effects: {}", ar.total().effects);
	}
	else {
		report += "No effects";
	}
	return report;
}

}

auto std::formatter<fallout_dicelib::roll_result, char>::parse(std::format_parse_context& ctx) 
	-> decltype(ctx.begin()) 
{ 
	return formatter.parse(ctx); 
}

auto std::formatter<fallout_dicelib::roll_result, char>::format(
	const fallout_dicelib::roll_result& r, 
	std::format_context& ctx
) const -> decltype(ctx.out()) {
	return formatter.format(fallout_dicelib::to_string(r), ctx);
}

auto std::formatter<fallout_dicelib::accumulated_roll_result, char>::parse(std::format_parse_context& ctx) 
	-> decltype(ctx.begin()) 
{
	return formatter.parse(ctx);
}

auto std::formatter<fallout_dicelib::accumulated_roll_result, char>::format(
	const fallout_dicelib::accumulated_roll_result& ar, 
	std::format_context& ctx
) const -> decltype(ctx.out()) {
	return formatter.format(ar.total(), ctx);
}