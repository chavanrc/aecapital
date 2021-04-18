#pragma once
#include <cstddef>
#include <fstream>
#include <functional>
#include <sstream>

#include "logger.hpp"
#include "order_book_side.hpp"

namespace aecapital {
    template <size_t capacity>
    class OrderBook {
        using BidSide = BookSideManager<std::greater<>, BookSide>;
        using AskSide = BookSideManager<std::less<>, BookSide>;

    private:
        AskSide ask_books_;
        BidSide bid_books_;

    public:
        OrderBook();

        [[nodiscard]] auto GetBestBidPrice() const -> Price;

        [[nodiscard]] auto GetBestAskPrice() const -> Price;

        template <typename Func>
        auto PrintOrderbook(int32_t token, Func print_func) -> void {
            ask_books_.for_each(true, print_func, "SELL", token);
            LOG_INFO("------------------------------------");
            bid_books_.for_each(false, print_func, "BUY", token);
            LOG_INFO("====================================");
        }

        template <typename Func>
        auto LogOrderbook(std::ofstream& out_file, Func print_func) -> void {
            out_file << "[[";
            bid_books_.for_each(print_func, "BUY", out_file);
            out_file << "], [";
            ask_books_.for_each(print_func, "SELL", out_file);
            out_file << "]]\n";
        }

        auto ProcessNewOrder(Quantity quantity, Price price, Side side) -> bool;

        auto ProcessModifyOrder(Quantity quantity, Price price, Side side) -> bool;

        auto ProcessDeleteOrder(Quantity quantity, Price price, Side side) -> bool;
    };

    static auto Print(const MbpInfo& mbp_info, const std::string& side, int32_t token) -> void {
        LOG_INFO(mbp_info);
    }

    static auto Log(const MbpInfo& mbp_info, int32_t iter, const std::string& side, std::ofstream& out_file) -> void {
        std::stringstream info;
        if (iter + 1 == QUOTE_LEVELS) {
            info << '(' << mbp_info.price_ << ", " << mbp_info.qty_ << ')';
        } else {
            info << '(' << mbp_info.price_ << ", " << mbp_info.qty_ << "), ";
        }
        out_file << info.str();
    }
}    // namespace aecapital

#include "order_book.inl"