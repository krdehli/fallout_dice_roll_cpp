#include "fallout_dicelib/token.hpp"

namespace {

template <typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}

auto std::formatter<fallout_dicelib::token_types::dice_roll, char>::parse(
    std::format_parse_context& ctx
) -> decltype(ctx.begin()) {
	return formatter.parse(ctx);
}

auto std::formatter<fallout_dicelib::token_types::dice_roll, char>::format(
    const fallout_dicelib::token_types::dice_roll& dice_roll, 
    std::format_context& ctx
) const -> decltype(ctx.out()) {
    return std::visit(overloaded{
        [&](unsigned faces) { 
            return formatter.format(std::format("{}d{}", dice_roll.count, faces), ctx);
        },
        [&](fallout_dicelib::token_types::dice_roll::combat_die) {
            return formatter.format(std::format("{}dc", dice_roll.count), ctx);
        },
        [&](fallout_dicelib::token_types::dice_roll::hit_location_die) {
            return formatter.format(std::format("{}dh", dice_roll.count), ctx);
        }
    },
    dice_roll.face_type);
}

auto std::formatter<fallout_dicelib::token, char>::parse(
    std::format_parse_context& ctx
) -> decltype(ctx.begin()) {
	return formatter.parse(ctx);
}

auto std::formatter<fallout_dicelib::token, char>::format(
    const fallout_dicelib::token& token, 
    std::format_context& ctx
) const -> decltype(ctx.out()) {
    using namespace fallout_dicelib::token_types;
    return std::visit(overloaded{
        [&](invalid)     { return formatter.format("INVALID", ctx); },
        [&](end_of_file) { return formatter.format("EOF", ctx); },
        [&](newline)     { return formatter.format("NEWLINE", ctx); },
        [&](const identifier& ident) { 
            return formatter.format(std::format("IDENTIFIER({})", ident), ctx); 
        },
        [&](integer i) { 
            return formatter.format(std::format("INTEGER({})", i), ctx); 
        },
        [&](dice_roll roll) { 
            return formatter.format(std::format("DICE_ROLL({})", roll), ctx); 
        }
    }, token);
}