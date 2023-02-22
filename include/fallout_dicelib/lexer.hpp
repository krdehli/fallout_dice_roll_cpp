#ifndef FALLOUT_DICE_ROLL_CPP_FALLOUT_DICELIBLEXER_HPP_2F898519_FFF2_4346_A1E1_2E98B4CDAAB1
#define FALLOUT_DICE_ROLL_CPP_FALLOUT_DICELIBLEXER_HPP_2F898519_FFF2_4346_A1E1_2E98B4CDAAB1

#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <utility>
#include <variant>
#include <locale>
#include <format>

#include <ctre.hpp>

#include "token.hpp"

namespace fallout_dicelib {

namespace regex_patterns {
	constexpr static auto integer_pattern = ctll::fixed_string{"-?\\d+"};
	constexpr static auto dice_roll_pattern = ctll::fixed_string{"(\\d*)[dD](?:(\\d+)|([cC])|([hH]))"};
	constexpr static auto identifier_pattern = ctll::fixed_string{"[a-zA-Z][a-zA-Z0-9_]*"};
}

[[nodiscard]] constexpr
auto to_token(std::string_view str) noexcept -> token
{
	using namespace regex_patterns;
	using namespace token_types;
	if (str == "\n") {
		return newline{};
	}
	if (const auto m = ctre::match<dice_roll_pattern>(str)) {
		token_types::dice_roll roll{};
		if (m.get<1>().size() > 0) {
			const auto count = m.get<1>().to_view();
			std::from_chars(count.data(), count.data() + count.length(), roll.count);
		}
		if (m.get<2>().size() > 0) {
			const auto num_faces = m.get<2>().to_view();
			roll.face_type = 0U;
			std::from_chars(num_faces.data(), num_faces.data() + num_faces.length(), std::get<unsigned>(roll.face_type));
		}
		else if (m.get<3>().size() > 0) {
			roll.face_type = dice_roll::combat_die{};
		}
		else if (m.get<4>().size() > 0) {
			roll.face_type = dice_roll::hit_location_die{};
		}
		return roll;
	}
	if (const auto m = ctre::match<integer_pattern>(str)) {
		const auto view = m.to_view();
		integer value{};
		std::from_chars(view.data(), view.data() + view.length(), value);
		return value;
	}
	if (ctre::match<identifier_pattern>(str)) {
		return identifier{str};
	}
	return invalid{};
}

[[nodiscard]] constexpr
auto lex(std::string_view source_code) noexcept -> std::vector<token> {
	std::vector<token> tokens;

	std::string current;
	for (const char character : source_code) {
		if (character == ' ' || character == '\t' || character == '\n') {
			if (current.size() > 0) {
				tokens.push_back(to_token(current));
				current.clear();
			}
			if (character == '\n') {
				tokens.push_back(token_types::newline{});
			}
			continue;
		}
		current.push_back(character);
	}
	tokens.push_back(to_token(current));

	return tokens;
}

}

#endif

