#ifndef FALLOUT_DICE_ROLL_CPP_FALLOUT_DICELIBLEXER_HPP_2F898519_FFF2_4346_A1E1_2E98B4CDAAB1
#define FALLOUT_DICE_ROLL_CPP_FALLOUT_DICELIBLEXER_HPP_2F898519_FFF2_4346_A1E1_2E98B4CDAAB1

#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <utility>
#include <regex>
#include <variant>

namespace fallout_dicelib {

namespace token_types {

	struct invalid {};
	struct end_of_file{};
	struct newline{};
	using indentifier = std::string;
	using integer = int;
	struct dice_roll
	{
		struct combat_die {};

		unsigned count;
		std::variant<unsigned, combat_die> face_type;
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

[[nodiscard]] constexpr
auto to_token(std::string_view str) noexcept -> token
{
	static const std::regex integer_regex{"-?\\d"};
	static const std::regex dice_roll_regex{"(\\d)?d(\\d+|c|C)"};
	static const std::regex identifier_regex{"[a-zA-Z][a-zA-Z0-9_]*"};

	if (str == '\n') {
		return token_types::newline{};
	}
	else if (std::smatch match; std::regex_match(str, match, dice_roll_regex)) {

	}
}

[[nodiscard]] constexpr
auto lex(std::string_view source_code) noexcept -> std::vector<token> {
	return std::ranges::split_view(source_code, ' ')
		| std::ranges::filter_view([](auto str) { return str.size() > 0 && str != ' '; })
		| std::ranges::transform_view(to_token)
		| std::ranges_to<std::vector>;
}

}

#endif

