#include <mbp_info.hpp>

namespace aecapital {
    MbpInfo::MbpInfo(Quantity qty, Price price) : qty_{qty}, price_{price} {
    }

    std::ostream& operator<<(std::ostream& os, const MbpInfo& info) {
        os << "qty: " << info.qty_ << " \tprice: " << info.price_;
        return os;
    }
}    // namespace aecapital