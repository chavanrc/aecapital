namespace aecapital {
    template <typename CompareT, typename ContT>
    BookSideManager<CompareT, ContT>::BookSideManager(int32_t max_levels, size_t capacity) : max_levels_{max_levels} {
        levels_.reserve(capacity);
    }

    template <typename CompareT, typename ContT>
    BookSideManager<CompareT, ContT>::BookSideManager(BookSideManager&& other) noexcept
        : levels_{std::move(other.levels_)}, max_levels_{other.max_levels_} {
    }

    template <typename CompareT, typename ContT>
    BookSideManager<CompareT, ContT>& BookSideManager<CompareT, ContT>::operator=(BookSideManager&& other) noexcept {
        if (this != &other) {
            levels_     = std::move(other.levels_);
            max_levels_ = std::move(other.max_levels_);
        }
        return *this;
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::Size() const -> size_t {
        return levels_.size();
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::Clear() -> void {
        levels_.Clear();
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::GetLevel(const int32_t level_no, value_type& return_value) const -> bool {
        assert(level_no >= 0);
        if (levels_.size() > (level_no)) {
            return_value = levels_[level_no];
            return true;
        }
        return false;
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::Add(Quantity quantity, Price price) -> int64_t {
        const value_type data{quantity, price};
        return AddLevel(data);
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::Modify(Quantity quantity, Price price) -> int64_t {
        const value_type data{quantity, price};
        return ModifyLevel(data);
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::Delete(Quantity quantity, Price price) -> int64_t {
        const value_type data{quantity, price};
        return DeleteLevel(data);
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::FillEmptyLevels() -> void {
        while (!levels_.empty() && levels_.size() < 3) {
            auto temp_order_info = levels_.back();
            temp_order_info.qty_ = 0;
            CompareT cmp;
            if (cmp(1, 2)) {
                temp_order_info.price_ += TICK_SIZE;
            } else {
                temp_order_info.price_ -= TICK_SIZE;
            }
            levels_.emplace_back(temp_order_info);
        }
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::AddLevel(const value_type& order_info) -> int64_t {
        CompareT cmp;
        if (Size() >= max_levels_) {
            if (!((cmp(order_info.price_, levels_[max_levels_ - 1].price_)) ||
                  (order_info.price_ == levels_[max_levels_ - 1].price_))) {
                return -1;
            }
        }
        auto    itr = std::lower_bound(levels_.begin(), levels_.end(), order_info,
                                    [&cmp](const auto& level, const auto& container_level) {
                                        return cmp(level.price_, container_level.price_);
                                    });
        int64_t distance;
        if (levels_.end() != itr) {
            if (itr->price_ == order_info.price_) {
                itr->qty_ += order_info.qty_;
                distance = std::distance(levels_.begin(), itr);
            } else {
                distance = std::distance(levels_.begin(), levels_.insert(itr, order_info));
            }
        } else {
            levels_.emplace_back(order_info);
            distance = levels_.size();
        }
        return distance;
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::ModifyLevel(const value_type& order_info) -> int64_t {
        CompareT cmp;
        auto     itr = std::lower_bound(levels_.begin(), levels_.end(), order_info,
                                    [&cmp](const auto& level, const auto& container_level) {
                                        return cmp(level.price_, container_level.price_);
                                    });
        int64_t  distance;
        if (levels_.end() != itr) {
            if (itr->price_ == order_info.price_) {
                itr->qty_ = order_info.qty_;
                distance  = std::distance(levels_.begin(), itr);
            } else {
                distance = std::distance(levels_.begin(), levels_.insert(itr, order_info));
            }
        } else {
            levels_.emplace_back(order_info);
            distance = levels_.size();
        }
        return distance;
    }

    template <typename CompareT, typename ContT>
    auto BookSideManager<CompareT, ContT>::DeleteLevel(const value_type& order_info) -> int64_t {
        CompareT cmp;
        int64_t  distance = -1;
        auto     itr      = std::lower_bound(levels_.begin(), levels_.end(), order_info,
                                    [&cmp](const auto& level, const auto& container_level) {
                                        return cmp(level.price_, container_level.price_);
                                    });
        if (levels_.end() != itr) {
            distance = std::distance(levels_.begin(), itr);
            levels_.erase(itr);
        }
        return distance;
    }
}    // namespace aecapital
