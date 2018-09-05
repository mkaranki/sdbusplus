#pragma once

#include <tuple>
#include <type_traits>

namespace sdbusplus
{

namespace utility
{

/** @brief Convert T[N] to T* if is_same<Tbase,T>
 *
 *  @tparam Tbase - The base type expected.
 *  @tparam T - The type to convert.
 */
template <typename Tbase, typename T>
using array_to_ptr_t = typename std::conditional_t<
    std::is_array<T>::value,
    std::conditional_t<std::is_same<Tbase, std::remove_extent_t<T>>::value,
                       std::add_pointer_t<std::remove_extent_t<T>>, T>,
    T>;

// Small helper class for stripping off the error code from the function
// argument definitions so unpack can be called appropriately
template <typename T>
struct strip_first_arg
{
};

template <typename FirstArg, typename... Rest>
struct strip_first_arg<std::tuple<FirstArg, Rest...>>
{
    using type = std::tuple<Rest...>;
};

// matching helper class to only return the first type
template <typename T>
struct get_first_arg
{
    using type = void;
};

template <typename FirstArg, typename... Rest>
struct get_first_arg<std::tuple<FirstArg, Rest...>>
{
    using type = FirstArg;
};

// helper class to remove const and reference from types
template <typename T>
struct decay_tuple
{
};

template <typename... Args>
struct decay_tuple<std::tuple<Args...>>
{
    using type = std::tuple<typename std::decay<Args>::type...>;
};

// Small helper class for stripping off the first + last character of a char
// array
template <std::size_t N, std::size_t... Is>
constexpr std::array<char, N - 2> strip_ends(const std::array<char, N>& s,
                                             std::index_sequence<Is...>)
{
    return {(s[1 + Is])..., static_cast<char>(0)};
};

template <std::size_t N>
constexpr std::array<char, N - 2> strip_ends(const std::array<char, N>& s)
{
    return strip_ends(s, std::make_index_sequence<N - 3>{});
};

} // namespace utility

} // namespace sdbusplus
