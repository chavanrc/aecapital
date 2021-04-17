namespace aecapital {

    template <size_t capacity>
    OrderBook<capacity>::OrderBook() : ask_books_{16, capacity}, bid_books_{16, capacity} {
    }

    template <size_t capacity>
    auto OrderBook<capacity>::GetBestBidPrice() const -> int32_t {
        if (bid_books_.Size() > 0) {
            return bid_books_[0].price_;
        }
        return -1;
    }

    template <size_t capacity>
    auto OrderBook<capacity>::GetBestAskPrice() const -> int32_t {
        if (ask_books_.Size() > 0) {
            return ask_books_[0].price_;
        }
        return -1;
    }

    template <size_t capacity>
    auto OrderBook<capacity>::ProcessNewOrder(Quantity quantity, Price price, Side side) -> bool {
        bool result       = false;
        auto message_side = static_cast<aecapital::SIDE>(side);
        switch (message_side) {
            case SIDE::BID: {
                const int64_t AFFECTED_LEVELS = bid_books_.Add(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;

            case SIDE::OFFER: {
                const int64_t AFFECTED_LEVELS = ask_books_.Add(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;
        }
        return result;
    }

    template <size_t capacity>
    auto OrderBook<capacity>::ProcessModifyOrder(Quantity quantity, Price price, Side side) -> bool {
        bool result       = false;
        auto message_side = static_cast<aecapital::SIDE>(side);
        switch (message_side) {
            case SIDE::BID: {
                const int64_t AFFECTED_LEVELS = bid_books_.Modify(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;

            case SIDE::OFFER: {
                const int64_t AFFECTED_LEVELS = ask_books_.Modify(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;
        }

        return result;
    }

    template <size_t capacity>
    auto OrderBook<capacity>::ProcessDeleteOrder(Quantity quantity, Price price, Side side) -> bool {
        bool result       = false;
        auto message_side = static_cast<aecapital::SIDE>(side);
        switch (message_side) {
            case SIDE::BID: {
                const int64_t AFFECTED_LEVELS = bid_books_.Delete(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;

            case SIDE::OFFER: {
                const int64_t AFFECTED_LEVELS = ask_books_.Delete(quantity, price);
                result                        = ((AFFECTED_LEVELS != -1) && (AFFECTED_LEVELS <= QUOTE_LEVELS));
            } break;
        }

        return result;
    }
}    // namespace aecapital