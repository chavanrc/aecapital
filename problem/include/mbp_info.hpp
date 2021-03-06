#pragma once
#include <ostream>

#include "types.hpp"

namespace aecapital {
    struct MbpInfo {
        Quantity qty_{0};
        Price    price_{0};

        MbpInfo(Quantity qty, Price price);

        friend std::ostream& operator<<(std::ostream& os, const MbpInfo& info);
    };
}    // namespace aecapital