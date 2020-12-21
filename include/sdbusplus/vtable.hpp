#pragma once

#include <systemd/sd-bus.h>

namespace sdbusplus
{

namespace vtable
{
/** Alias typedef for sd_bus_vtable */
using vtable_t = sd_bus_vtable;

/** Create a SD_BUS_VTABLE_START entry. */
constexpr vtable_t start(decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_VTABLE_END entry. */
constexpr vtable_t end();

/** Create a SD_BUS_VTABLE_METHOD entry.
 *
 *  @param[in] member - Name of method.
 *  @param[in] signature - Signature of method.
 *  @param[in] result - Signature of result.
 *  @param[in] handler - Functor to call on method invocation.
 *  @param[in] flags - optional sdbusplus::vtable::method_ value.
 */
constexpr vtable_t method(const char* member, const char* signature,
                          const char* result, sd_bus_message_handler_t handler,
                          decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_VTABLE_METHOD_WITH_OFFSET entry.
 *
 *  @param[in] member - Name of method.
 *  @param[in] signature - Signature of method.
 *  @param[in] result - Signature of result.
 *  @param[in] handler - Functor to call on method invocation.
 *  @param[in] offset - Offset.
 *  @param[in] flags - optional sdbusplus::vtable::method_ value.
 */
constexpr vtable_t method_o(const char* member, const char* signature,
                            const char* result,
                            sd_bus_message_handler_t handler, size_t offset,
                            decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_VTABLE_METHOD_WITH_NAMES entry.
 *
 *  @param[in] member - Name of method.
 *  @param[in] signature - Signature of method.
 *  @param[in] result - Signature of result.
 *  @param[in] names - nil-delimited names of signature and result arguments.
 *  @param[in] handler - Functor to call on method invocation.
 *  @param[in] flags - optional sdbusplus::vtable::method_ value.
 */
constexpr vtable_t method_n(const char* member, const char* signature,
                            const char* result, const char *names,
                            sd_bus_message_handler_t handler,
                            decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_SIGNAL entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] flags - None supported.
 */
constexpr vtable_t signal(const char* member, const char* signature,
                          decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_SIGNAL_WITH_NAMES entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] names - nil-delimited names of signature arguments.
 * @param[in] flags - None supported.
 */
constexpr vtable_t signal_n(const char* member, const char* signature,
                            const char* names,
                            decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_PROPERTY entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] get - Functor to call on property get.
 * @param[in] flags - optional sdbusplus::vtable::property_ value.
 */
constexpr vtable_t property(const char* member, const char* signature,
                            sd_bus_property_get_t get,
                            decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_WRITABLE_PROPERTY entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] get - Functor to call on property get.
 * @param[in] set - Functor to call on property set.
 * @param[in] flags - optional sdbusplus::vtable::property_ value.
 */
constexpr vtable_t property(const char* member, const char* signature,
                            sd_bus_property_get_t get,
                            sd_bus_property_set_t set,
                            decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_PROPERTY entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] offset - Offset within object for property.
 * @param[in] flags - optional sdbusplus::vtable::property_ value.
 */
constexpr vtable_t property_o(const char* member, const char* signature,
                              size_t offset,
                              decltype(vtable_t::flags) flags = 0);

/** Create a SD_BUS_WRITABLE_PROPERTY entry.
 *
 * @param[in] member - Name of signal.
 * @param[in] signature - Signature of signal.
 * @param[in] set - Functor to call on property set.
 * @param[in] offset - Offset within object for property.
 * @param[in] flags - optional sdbusplus::vtable::property_ value.
 */
constexpr vtable_t property_o(const char* member, const char* signature,
                              sd_bus_property_set_t set, size_t offset,
                              decltype(vtable_t::flags) flags = 0);

namespace common_
{
constexpr auto deprecated = SD_BUS_VTABLE_DEPRECATED;
constexpr auto hidden = SD_BUS_VTABLE_HIDDEN;
constexpr auto unprivileged = SD_BUS_VTABLE_UNPRIVILEGED;
} // namespace common_

namespace method_
{
constexpr auto no_reply = SD_BUS_VTABLE_METHOD_NO_REPLY;
} // namespace method_

namespace property_
{
constexpr auto const_ = SD_BUS_VTABLE_PROPERTY_CONST;
constexpr auto emits_change = SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE;
constexpr auto emits_invalidation = SD_BUS_VTABLE_PROPERTY_EMITS_INVALIDATION;
constexpr auto explicit_ = SD_BUS_VTABLE_PROPERTY_EXPLICIT;
constexpr auto none = decltype(vtable_t::flags){};
} // namespace property_

constexpr vtable_t start(decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_VTABLE_START(flags);
}

constexpr vtable_t end()
{
    return vtable_t SD_BUS_VTABLE_END;
}

constexpr vtable_t method(const char* member, const char* signature,
                          const char* result, sd_bus_message_handler_t handler,
                          decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_METHOD(member, signature, result, handler, flags);
}

constexpr vtable_t method_o(const char* member, const char* signature,
                            const char* result,
                            sd_bus_message_handler_t handler, size_t offset,
                            decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_METHOD_WITH_OFFSET(member, signature, result,
                                              handler, offset, flags);
}

constexpr vtable_t method_n(const char* member, const char* signature,
                            const char* result, const char *names,
                            sd_bus_message_handler_t handler,
                            decltype(vtable_t::flags) flags)
{
    // Note: unfortunately SD_BUS_METHOD_WITH_NAMES concatenates _in_names and
    // _out_names expecting them to be string literals. All the names are
    // passed as _in_names here and _out_names is omitted.
    return vtable_t SD_BUS_METHOD_WITH_NAMES(member, signature, names,
                                             result, /* _out_names_omitted */,
                                             handler, flags);
}

constexpr vtable_t signal(const char* member, const char* signature,
                          decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_SIGNAL(member, signature, flags);
}


constexpr vtable_t signal_n(const char* member, const char* signature,
                            const char* names,
                            decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_SIGNAL_WITH_NAMES(member, signature, names, flags);
}

constexpr vtable_t property(const char* member, const char* signature,
                            sd_bus_property_get_t get,
                            decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_PROPERTY(member, signature, get, 0, flags);
}

constexpr vtable_t property(const char* member, const char* signature,
                            sd_bus_property_get_t get,
                            sd_bus_property_set_t set,
                            decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_WRITABLE_PROPERTY(member, signature, get, set, 0,
                                             flags);
}

constexpr vtable_t property_o(const char* member, const char* signature,
                              size_t offset, decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_PROPERTY(member, signature, nullptr, offset, flags);
}

constexpr vtable_t property_o(const char* member, const char* signature,
                              sd_bus_property_set_t set, size_t offset,
                              decltype(vtable_t::flags) flags)
{
    return vtable_t SD_BUS_WRITABLE_PROPERTY(member, signature, nullptr, set,
                                             offset, flags);
}

} // namespace vtable

} // namespace sdbusplus
