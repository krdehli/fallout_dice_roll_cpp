#include <catch.hpp>

#include <limits>
#include <type_traits>
#include <random>
#include <format>
#include <string>

#include <randutils.hpp>

#include <krsd/die.hpp>
#include <krsd/roller.hpp>
#include <fallout_dicelib/combat_dice.hpp>
#include <fallout_dicelib/d20_test.hpp>
#include <fallout_dicelib/lexer.hpp>

template <typename T>
class std::formatter<std::vector<T>, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin()) {
		return formatter.parse(ctx);
	}
	auto format(
		const std::vector<T>& elements, 
		std::format_context& ctx
	) const -> decltype(ctx.out()) {
		std::string result;
		for (const auto& element : elements) {
			result += std::format("{} ", element);
		}
		return formatter.format(result, ctx);
	}

private:
	mutable std::formatter<std::string> formatter;
};

TEST_CASE("DC literal") {
	const auto test_roll = krsd::roll_result<fallout_dicelib::combat_die>::literal<{0, 0}>();
	REQUIRE(test_roll.value().eyes() == 0);
}

TEST_CASE("D20 Tests") {
	fallout_dicelib::d20_test_roller roller;
	std::mt19937 rng{randutils::auto_seed_256{}.base()};

	const fallout_dicelib::test_parameters params{5, 2};
	const auto result = roller.test(rng, params, 2);
	REQUIRE(result.successes() < 5);
}

TEST_CASE("Lexer") {
	const auto command = fallout_dicelib::to_token("command");
	const auto number = fallout_dicelib::to_token("-32");
	const auto dice_roll = fallout_dicelib::to_token("d6");

	REQUIRE(std::holds_alternative<fallout_dicelib::token_types::identifier>(command));
	REQUIRE(std::holds_alternative<fallout_dicelib::token_types::integer>(number));
	REQUIRE(std::holds_alternative<fallout_dicelib::token_types::dice_roll>(dice_roll));

	const std::string_view source_code{"hello 1 2 3\n\tgoodbye   4 d6\t\n "};
	const auto tokens = fallout_dicelib::lex(source_code);

	INFO((std::format("{}", tokens)));
	REQUIRE(tokens.size() == 10);
}