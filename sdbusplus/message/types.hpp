#pragma once

#include <tuple>
#include <string>
#include <vector>

#include <sdbusplus/utility/type_traits.hpp>

namespace sdbusplus
{

namespace message
{

namespace types
{

/** @fn type_id()
 *  @brief Get a tuple containing the dbus type character(s) for a
 *         sequence of C++ types.
 *
 *  @tparam ...Args - Types to get the type_id sequence for.
 *  @returns A tuple of characters representing the dbus types for ...Args.
 *
 *  The design uses a tuple because a tuple can, at compile-time, be easily
 *  appended to, concatenated, and converted to an array (representing a
 *  C-string).  There are other options to create a string of characters but
 *  they require runtime processing and memory allocation.  By performing all
 *  options at compile-time the use of type-deduced dbus strings is equal to
 *  the cost of hard-coded type string constants.
 */
template <typename ...Args> constexpr auto type_id();

namespace details
{

/** @struct undefined_type_id
 *  @brief Special type indicating no dbus-type_id is defined for a C++ type.
 */
struct undefined_type_id
{
    /** An empty tuple indicating no type-characters. */
    // We want this to be tuple so that we can use operations like
    // tuple_cat on it without cascading compile failures.  There is a
    // static_assert in type_id_single to ensure this is never used, but to
    // keep the compile failures as clean as possible.
    static constexpr auto value = std::make_tuple();
};

/** @struct tuple_type_id
 *  @brief Special type indicating a tuple of dbus-type_id's.
 *
 *  @tparam C1 - The first dbus type character character.
 *  @tparam ...C - The remaining sequence of dbus type characters.
 *
 *  A tuple_type_id must be one or more characters.  The C1 template param
 *  ensures at least one is present.
 */
template <char C1, char... C> struct tuple_type_id
{

    /* This version check is required because a fix for auto is in 5.2+.
     * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66421
     */
    /** A tuple containing the type-characters. */
#if (__GNUC__ > 5) || (__GNUC__ == 5 && (__GNUC_MINOR__ >= 2))
    static constexpr auto value = std::make_tuple(C1, C...);
#else
    static constexpr decltype(std::make_tuple(C1, C...)) value =
        std::make_tuple(C1, C...);
#endif
};

/** @fn type_id_single()
 *  @brief Get a tuple containing the dbus type character(s) for a C++ type.
 *
 *  @tparam T - The type to get the dbus type character(s) for.
 */
template <typename T> constexpr auto& type_id_single();

/** @fn type_id_multiple()
 *  @brief Get a tuple containing the dbus type characters for a sequence of
 *         C++ types.
 *
 *  @tparam T - The first type to get the dbus type character(s) for.
 *  @tparam ...Args - The remaining types.
 */
template <typename T, typename ...Args> constexpr auto type_id_multiple();

/** @struct type_id
 *  @brief Defined dbus type tuple for a C++ type.
 *
 *  @tparam T - C++ type.
 *
 *  Struct must have a 'value' tuple containing the dbus type.  The default
 *  value is an empty tuple, which can be used to indicate
 */
template <typename T> struct type_id : public undefined_type_id {};
    // Specializations for built-in types.
template <> struct type_id<bool> : tuple_type_id<'b'> {};
template <> struct type_id<uint8_t> : tuple_type_id<'y'> {};
// int8_t isn't supported by dbus.
template <> struct type_id<uint16_t> : tuple_type_id<'q'> {};
template <> struct type_id<int16_t> : tuple_type_id<'n'> {};
template <> struct type_id<uint32_t> : tuple_type_id<'u'> {};
template <> struct type_id<int32_t> : tuple_type_id<'i'> {};
template <> struct type_id<uint64_t> : tuple_type_id<'t'> {};
template <> struct type_id<int64_t> : tuple_type_id<'x'> {};
// float isn't supported by dbus.
template <> struct type_id<double> : tuple_type_id<'d'> {};
template <> struct type_id<const char*> : tuple_type_id<'s'> {};
template <> struct type_id<char*> : tuple_type_id<'s'> {};
template <> struct type_id<std::string> : tuple_type_id<'s'> {};

template <typename T> constexpr auto& type_id_single()
{
    static_assert(!std::is_base_of<undefined_type_id, type_id<T>>::value,
        "No dbus type conversion provided for type.");
    return type_id<T>::value;
}

template <typename T, typename ...Args> constexpr auto type_id_multiple()
{
    return std::tuple_cat(type_id_single<T>(),
        type_id_single<Args>()...);
}

/** @brief Convert some C++ types to others for 'type_id' conversion purposes.
 *
 *  Similar C++ types have the same dbus type-id, so 'downcast' those to limit
 *  duplication in type_id template specializations.
 *
 *  1. Remove references.
 *  2. Remove 'const' and 'vector'.
 *  3. Convert 'char[N]' to 'char*'.
 */
template <typename T> struct type_id_downcast
{
    using type = typename utility::array_to_ptr_t<
            char, std::remove_cv_t<std::remove_reference_t<T>>>;
};

template <typename T> using type_id_downcast_t =
        typename type_id_downcast<T>::type;

} // namespace details

template <typename ...Args> constexpr auto type_id()
{
    return std::tuple_cat(
        details::type_id_multiple<details::type_id_downcast_t<Args>...>(),
        std::make_tuple('\0') /* null terminator for C-string */ );
}

} // namespace types

} // namespace message

} // namespace sdbusplus
