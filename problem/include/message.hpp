#pragma once

#include <limits>
#include <ostream>

#include "types.hpp"

namespace aecapital {
#pragma pack(push, 1)

    struct MessageHeader {
        PacketSize   packet_size_{0};
        MessageCount message_count_{0};

        friend std::ostream &operator<<(std::ostream &os, const MessageHeader &header) {
            os << "packet_size: " << header.packet_size_ << " message_count: " << (unsigned)header.message_count_;
            return os;
        }
    };

    struct Message {
        Quantity quantity_{0};
        Price    price_{std::numeric_limits<Price>::max()};
        Side     side_{std::numeric_limits<Side>::max()};
        Type     type_{std::numeric_limits<Type>::max()};

        Message() = default;

        Message(Quantity quantity, Price price, Side side, Type type)
            : quantity_{quantity}, price_{price}, side_{side}, type_{type} {
        }

        friend std::ostream &operator<<(std::ostream &os, const Message &message) {
            os << "quantity: " << message.quantity_ << " price: " << message.price_
               << " side: " << (unsigned)message.side_ << " type: " << (unsigned)message.type_;
            return os;
        }
    };

#pragma pack(pop)
}    // namespace aecapital