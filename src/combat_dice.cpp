#include "combat_dice.hpp"

namespace fallout {

[[nodiscard]] 
auto to_string(const roll& r) -> std::string {
	std::string result;
	result.reserve(25);
	result += "Damage: ";
	result += std::to_string(r.damage);
	result += ", Effects: ";
	result += std::to_string(r.effects);
	return result;
}

auto operator<<(std::ostream& os, const roll& r) -> std::ostream& { 
	return os << to_string(r); 
}

[[nodiscard]] 
auto to_string(const roll_result& rr) -> std::string {
	return to_string(rr.total()); 
}

auto operator<<(std::ostream& os, const roll_result& rr) -> std::ostream& { 
	return os << to_string(rr); 
}

[[nodiscard]]
auto to_report_string(const roll_result& rr) -> std::string { 
	std::string report = "Rolls: "; 
	for (const auto& r : rr.rolls_) {
		report += std::format("[{} {}] ", r.damage, r.effects);
	}
	report += '\n';
	report += std::format("Total damage: {}\n", rr.total_.damage);
	if (rr.effect_triggered()) {
		report += std::format("Total effects: {}", rr.total_.effects);
	}
	else {
		report += "No effects";
	}
	return report;
}

}

auto std::formatter<fallout::roll, char>::parse(std::format_parse_context& ctx) 
	-> decltype(ctx.begin()) 
{ 
	return formatter.parse(ctx); 
}

auto std::formatter<fallout::roll, char>::format(
	const fallout::roll& r, 
	std::format_context& ctx
) const -> decltype(ctx.out()) {
	return formatter.format(to_string(r), ctx);
}

auto std::formatter<fallout::roll_result, char>::parse(std::format_parse_context& ctx) 
	-> decltype(ctx.begin()) 
{
	return formatter.parse(ctx);
}

auto std::formatter<fallout::roll_result, char>::format(
	const fallout::roll_result& rr, 
	std::format_context& ctx
) const -> decltype(ctx.out()) {
	return formatter.format(rr.total(), ctx);
}