#include <message.hpp>

namespace aecapital {
#pragma pack(push, 1)
    Message::Message(Quantity quantity, Price price, Side side, Type type)
        : quantity_{quantity}, price_{price}, side_{side}, type_{type} {
    }

    std::ostream &operator<<(std::ostream &os, const MessageHeader &header) {
        os << "packet_size: " << header.packet_size_ << " message_count: " << (unsigned)header.message_count_;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Message &message) {
        os << "quantity: " << message.quantity_ << " price: " << message.price_ << " side: " << (unsigned)message.side_
           << " type: " << (unsigned)message.type_;
        return os;
    }
#pragma pack(pop)
}    // namespace aecapital