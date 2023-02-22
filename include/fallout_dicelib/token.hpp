#ifndef FALLOUT_DICELIB_FALLOUT_DICELIBTOKEN_HPP_4B3E78CE_1369_4892_8B7C_89AAF2C3EE87
#define FALLOUT_DICELIB_FALLOUT_DICELIBTOKEN_HPP_4B3E78CE_1369_4892_8B7C_89AAF2C3EE87

#include <variant>
#include <string>
#include <format>

namespace fallout_dicelib {

namespace token_types {
	struct invalid {};
	struct end_of_file{};
	struct newline{};
	using identifier = std::string;
	using integer = int;
	struct dice_roll
	{
		struct combat_die {};
		struct hit_location_die {};
		unsigned count = 1;
		std::variant<unsigned, combat_die, hit_location_die> face_type = 1U;
	};
}

using token = std::variant<
	token_types::invalid,
	token_types::end_of_file,
	token_types::newline,
	token_types::identifier,
	token_types::integer,
	token_types::dice_roll
>;

}

template <>
class std::formatter<fallout_dicelib::token_types::dice_roll, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin());
	auto format(
		const fallout_dicelib::token_types::dice_roll& dice_roll, 
		std::format_context& ctx
	) const -> decltype(ctx.out());

private:
	mutable std::formatter<std::string> formatter;
};

template <>
class std::formatter<fallout_dicelib::token, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin());
	auto format(
		const fallout_dicelib::token& token, 
		std::format_context& ctx
	) const -> decltype(ctx.out());

private:
	mutable std::formatter<std::string> formatter;
};


#endif

