#pragma once

#include <cstdint>

namespace aecapital {
    using PacketSize   = uint16_t;
    using MessageCount = uint8_t;
    using Quantity     = uint64_t;
    using Price        = int32_t;
    using Side         = uint8_t;
    using Type         = uint8_t;

    enum class SIDE : uint8_t { BID, OFFER };

    enum class TYPE : uint8_t { NEW, CHANGE, DELETE };

    constexpr int32_t QUOTE_LEVELS = 3;
    constexpr int32_t TICK_SIZE    = 1;
    constexpr int32_t MAX_LEVEL    = 128;
}    // namespace aecapital