#pragma once
#include <cassert>
#include <cstddef>
#include <ranges>
#include <vector>

#include "mbp_info.hpp"

namespace aecapital {
    using BookSide   = std::vector<MbpInfo>;
    using BidAskIter = BookSide::iterator;

    template <typename CompareT, typename ContT = BookSide>
    class BookSideManager {
        using Levels = ContT;

    public:
        using iterator               = typename Levels::iterator;
        using const_iterator         = typename Levels::const_iterator;
        using reverse_iterator       = typename Levels::reverse_iterator;
        using const_reverse_iterator = typename Levels::const_reverse_iterator;
        using value_type             = typename Levels::value_type;

        explicit BookSideManager(int32_t max_levels, size_t capacity = 64);

        BookSideManager(BookSideManager&& other) noexcept;

        BookSideManager& operator=(BookSideManager&& other) noexcept;

        auto operator[](const int32_t index) -> value_type& {
            return levels_[index];
        }

        auto operator[](const int32_t index) const -> const value_type& {
            return levels_[index];
        }

        [[nodiscard]] auto Size() const -> size_t;

        auto Clear() -> void;

        auto GetLevel(const int32_t level_no, value_type& return_value) const -> bool;

        template <typename Func, typename... Args>
        auto for_each(bool reverse, Func iter_func, Args&&... params) -> void {
            if (!levels_.empty()) {
                int32_t iter = 0;
                if (reverse) {
                    Levels temp_levels;
                    for (auto& value_ : levels_) {
                        temp_levels.emplace_back(value_);
                        if (++iter == QUOTE_LEVELS) {
                            break;
                        }
                    }
                    iter = 0;
                    for (auto& value_ : temp_levels | std::views::reverse) {
                        iter_func(value_, params...);
                        if (++iter == QUOTE_LEVELS) {
                            break;
                        }
                    }
                } else {
                    for (auto& value_ : levels_) {
                        iter_func(value_, params...);
                        if (++iter == QUOTE_LEVELS) {
                            break;
                        }
                    }
                }
                while (iter < QUOTE_LEVELS) {
                    iter_func(GetEmptyLevel(), params...);
                    if (++iter == QUOTE_LEVELS) {
                        break;
                    }
                }
            }
        }

        template <typename Func, typename... Args>
        auto for_each(Func iter_func, Args&&... params) -> void {
            if (!levels_.empty()) {
                int32_t iter = 0;
                for (auto& value_ : levels_) {
                    iter_func(value_, iter, params...);
                    if (++iter == QUOTE_LEVELS) {
                        break;
                    }
                }

                while (iter < QUOTE_LEVELS) {
                    iter_func(GetEmptyLevel(), iter, params...);
                    if (++iter == QUOTE_LEVELS) {
                        break;
                    }
                }
            }
        }

        auto Add(Quantity quantity, Price price) -> int64_t;

        auto Modify(Quantity quantity, Price price) -> int64_t;

        auto Delete(Quantity quantity, Price price) -> int64_t;

        BookSideManager(const BookSideManager&) = delete;

        BookSideManager& operator=(const BookSideManager&) = delete;

    private:
        Levels  levels_;
        int32_t max_levels_;

        auto FillEmptyLevels() -> void;

        auto GetEmptyLevel() -> value_type {
            auto temp_order_info = levels_.back();
            temp_order_info.qty_ = 0;
            CompareT cmp;
            if (cmp(1, 2)) {
                temp_order_info.price_ += TICK_SIZE;
            } else {
                temp_order_info.price_ -= TICK_SIZE;
            }
            return temp_order_info;
        }

        auto AddLevel(const value_type& order_info) -> int64_t;

        auto ModifyLevel(const value_type& order_info) -> int64_t;

        auto DeleteLevel(const value_type& order_info) -> int64_t;
    };
}    // namespace aecapital

#include "order_book_side.inl"