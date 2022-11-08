#ifndef FALLOUT_COMBAT_DICE_CPP_COMBAT_DICE_HPP_217973CF_1F39_4181_AB51_E47DCD4460B5
#define FALLOUT_COMBAT_DICE_CPP_COMBAT_DICE_HPP_217973CF_1F39_4181_AB51_E47DCD4460B5

#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <concepts>
#include <iterator>
#include <ranges>
#include <numeric>
#include <random>

namespace fallout {

//! Represents the result of rolling one or more Fallout Combat Dice.
struct roll_result
{
	int damage;
	int effects;

	constexpr 
	roll_result(int damage = 0, int effects = 0) noexcept : 
		damage{damage}, 
		effects{effects} 
	{}

	[[nodiscard]] friend constexpr 
	auto operator+(const roll_result& lhs, const roll_result& rhs) noexcept -> roll_result {
		return {
			lhs.damage + rhs.damage, 
			lhs.effects + rhs.effects
		};
	}

	constexpr 
	auto operator+=(const roll_result& rhs) noexcept -> roll_result& {
		damage += rhs.damage;
		effects += rhs.effects;
		return *this;
	}

	[[nodiscard]]
	auto to_string() const -> std::string;
	friend auto operator<<(std::ostream& os, const roll_result& r) -> std::ostream&;
};

[[nodiscard]] 
auto to_string(const roll_result& r) -> std::string;

//! Groups multiple rolls of combat dice together to allow for inspection of both the total rolled
//! and the individual rolls.
class accumulated_roll_result
{
public:
	template <std::input_iterator I, std::sentinel_for<I> S>
	requires std::indirectly_copyable<I, std::ranges::iterator_t<std::vector<roll_result>>>
	constexpr 
	accumulated_roll_result(I first_roll, S last_roll)
	{
		std::ranges::copy(first_roll, last_roll, std::back_inserter(rolls_));
		total_ = std::reduce(
			std::ranges::begin(rolls_), 
			std::ranges::end(rolls_)
		);
	}

	template <std::ranges::input_range R>
	requires std::indirectly_copyable<
		std::ranges::iterator_t<R>, 
		std::ranges::iterator_t<std::vector<roll_result>>
	>
	constexpr 
	accumulated_roll_result(R&& r) : accumulated_roll_result{std::ranges::begin(r), std::ranges::end(r)} {}

	[[nodiscard]] constexpr 
	auto total() const noexcept -> const roll_result& { return total_; }

	[[nodiscard]] constexpr 
	auto rolls() const noexcept -> const std::vector<roll_result>& { return rolls_; }

	//! True if the roll had any effect triggered, false otherwise.
	[[nodiscard]] constexpr 
	auto effect_triggered() const noexcept -> bool { return total_.effects > 0; }

	auto to_string() const -> std::string;
	friend auto operator<<(std::ostream& os, const accumulated_roll_result& rr) -> std::ostream&;
	
	//! Produces a string listing of all individual dice rolls and the total result.
	auto to_report_string() const -> std::string;

private:
	std::vector<roll_result> rolls_;
	roll_result total_;
};

auto to_string(const accumulated_roll_result& rr) -> std::string;

//! Static class used for rolling Fallout Combat Dice and converting regular d6 rolls.
class combat_dice
{
public:
	combat_dice() = delete;

	//! Rolls a given number of combat dice and returns the result.
	template <std::uniform_random_bit_generator Rng>
	[[nodiscard]] 
	static auto roll(Rng& rng, unsigned int num) -> accumulated_roll_result {
		static std::uniform_int_distribution<std::size_t> dist{0, std::size(faces) - 1};
		std::vector<roll_result> rolls(num, roll_result{});
		std::ranges::generate(rolls, [&]{ return faces[dist(rng)]; });
		return accumulated_roll_result{rolls};
	}

	//! Converts a single roll on a d6 into its respective roll on a combat die, as defined in
	//! the conversion table in the Fallout TTRPG rulebook. 
	[[nodiscard]] constexpr 
	static auto from_dice_roll(std::size_t face) -> const roll_result& {
		return faces.at(face - 1);
	}

	//! Converts multiple d6 dice rolls to their respective rolls on combat dice and returns the total
	//! result.
	template <std::input_iterator I, std::sentinel_for<I> S>
	requires std::invocable<decltype(from_dice_roll), std::iter_value_t<I>>
	[[nodiscard]] 
	static auto from_dice_rolls(I first_face, S last_face) noexcept -> accumulated_roll_result {
		return accumulated_roll_result{
			std::ranges::views::transform(
				std::ranges::subrange(first_face, last_face), 
				from_dice_roll
			)
		};
	}

	//! Converts multiple d6 dice rolls to their respective rolls on combat dice and returns the total
	//! result.
	template <std::ranges::input_range R>
	requires std::invocable<decltype(from_dice_roll), std::ranges::range_value_t<R>>
	[[nodiscard]] 
	static auto from_dice_rolls(R&& range) noexcept -> accumulated_roll_result { 
		return from_dice_rolls(std::ranges::begin(range), std::ranges::end(range));
	}

	template <typename Int>
	requires std::convertible_to<Int, unsigned int>
	[[nodiscard]] 
	static auto from_dice_rolls(std::initializer_list<Int> ilist) noexcept -> accumulated_roll_result {
		return from_dice_rolls(ilist.begin(), ilist.end());
	}

	template <typename First_int, typename... Ints>
	requires std::convertible_to<First_int, unsigned int> && (std::convertible_to<Ints, First_int> && ...)
	[[nodiscard]] 
	static auto from_dice_rolls(First_int&& first_int, Ints&&... ints) noexcept -> accumulated_roll_result {
		return from_dice_rolls({first_int, static_cast<First_int>(std::forward<Ints>(ints))...});
	}

private:
	// The values of the faces on a Fallout Combat Dice
	constexpr static std::array faces{
		roll_result{1}, 
		roll_result{2}, 
		roll_result{0}, 
		roll_result{0}, 
		roll_result{1, 1}, 
		roll_result{1, 1}
	};
};

}

template <>
class std::formatter<fallout::roll_result, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin());
	auto format(const fallout::roll_result& r, std::format_context& ctx) const 
		-> decltype(ctx.out());

private:
	mutable std::formatter<std::string_view> formatter;
};

template <>
class std::formatter<fallout::accumulated_roll_result, char> {
public:
	auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin());
	auto format(const fallout::accumulated_roll_result& ar, std::format_context& ctx) const -> decltype(ctx.out());

private:
	mutable std::formatter<fallout::roll_result> formatter;
};

#endif