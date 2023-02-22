#ifndef FALLOUT_DICELIB_FALLOUT_DICELIBPARSER_HPP_1E5B7D68_BDB5_4C14_81DE_3A71DB6E5FA2
#define FALLOUT_DICELIB_FALLOUT_DICELIBPARSER_HPP_1E5B7D68_BDB5_4C14_81DE_3A71DB6E5FA2

#include <vector>
#include <ranges>
#include <iterator>
#include <concepts>
#include <limits>
#include <expected>

#include "fallout_dicelib/token.hpp"

namespace fallout_dicelib {

enum class parameter_type {
    INTEGER,
    DICE_ROLL
};
struct parameter_description {
    constexpr static auto UNLIMITED{std::numeric_limits<unsigned>::max()};
    parameter_type type;
    unsigned min_count = 1;
    unsigned max_count = UNLIMITED;

    constexpr 
    parameter_description(
        parameter_type type, 
        unsigned min_count = 1, 
        unsigned max_count = UNLIMITED
    ) noexcept :
        type{type},
        min_count{min_count},
        max_count{max_count}
    {}

};

struct command_description {
    token_types::identifier name = "";
    std::vector<parameter_description> parameters = {};
};

struct command {
    token_types::identifier name = "";
    std::vector<token> parameters = {};
};

struct parser_error {
    enum class error_type {
        SYNTAX_ERROR
    };
    error_type type;
    std::string message;

    constexpr 
    parser_error(error_type type, std::string_view message = "") noexcept : 
        type{type}, message{message} 
    {}
};

class parser {
public:
    template<std::input_iterator I, std::sentinel_for<I> S>
    requires std::indirectly_copyable<I, std::ranges::iterator_t<std::vector<command_description>>>
    constexpr
    parser(I first, S last) {
        std::ranges::copy(first, last, std::back_inserter(command_descriptions_));
    }

    template<std::ranges::input_range R>
    requires std::indirectly_copyable<std::ranges::iterator_t<R>, std::ranges::iterator_t<std::vector<command_description>>>
    constexpr
    parser(R&& range) :
        parser{std::ranges::begin(range), std::ranges::end(range)} 
    {}

    template <typename T>
    requires std::convertible_to<T, command_description>
    constexpr
    parser(std::initializer_list<T> ilist) : parser{ilist.begin(), ilist.end()} {}

    template <typename T, typename... Rest>
    requires std::convertible_to<T, command_description> && (std::convertible_to<Rest, T> && ...)
    constexpr
    parser(T&& first, Rest&&... rest) :
        parser{std::initializer_list<T>{first, static_cast<T>(rest)...}}
    {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    requires std::convertible_to<std::iter_value_t<I>, token>
    [[nodiscard]] constexpr
    auto parse(I first, S last) -> std::expected<std::vector<command>, parser_error> {
        std::vector<command> commands;
        command current;
        for (const token& t : std::ranges::subrange(first, last)) {
            if (current.name == "") {
                if (not std::holds_alternative<token_types::identifier>(t)) {
                    return std::unexpected{
                        parser_error::error_type::SYNTAX_ERROR, 
                        "Statements must begin with a command name"
                    };
                }
                current.name = std::get<token_types::identifier>(t);
            }
        }
        return commands;
    }

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, token>
    [[nodiscard]] constexpr
    auto parse(R&& range) -> std::expected<std::vector<command>, parser_error> {
        return parse(std::ranges::begin(range), std::ranges::end(range));
    }
private:
    std::vector<command_description> command_descriptions_;
};

[[nodiscard]] constexpr
auto parse(const std::ranges::input_range auto& range) noexcept
    requires(std::same_as<std::ranges::range_value_t<decltype(range)>, token>)
{

}

}

#endif

