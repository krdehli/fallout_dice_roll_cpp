#include "combat_dice.hpp"

namespace fallout_dicelib {

[[nodiscard]] 
auto roll_result::to_string() const -> std::string {
	std::string result;
	result.reserve(25);
	result += "Damage: ";
	result += std::to_string(damage);
	result += ", Effects: ";
	result += std::to_string(effects);
	return result;
}

[[nodiscard]] 
auto to_string(const roll_result& r) -> std::string { return r.to_string(); }

auto operator<<(std::ostream& os, const roll_result& r) -> std::ostream& { 
	return os << to_string(r); 
}

[[nodiscard]] 
auto accumulated_roll_result::to_string() const -> std::string { return total().to_string(); }

[[nodiscard]] 
auto to_string(const accumulated_roll_result& ar) -> std::string { return ar.to_string(); }

auto operator<<(std::ostream& os, const accumulated_roll_result& ar) -> std::ostream& { 
	return os << to_string(ar); 
}

[[nodiscard]]
auto accumulated_roll_result::to_report_string() const -> std::string { 
	std::string report = "Rolls: "; 
	for (const auto& r : rolls_) {
		report += std::format("[{} {}] ", r.damage, r.effects);
	}
	report += '\n';
	report += std::format("Total damage: {}\n", total_.damage);
	if (effect_triggered()) {
		report += std::format("Total effects: {}", total_.effects);
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