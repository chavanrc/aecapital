#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include <iostream>
#include <message.hpp>
#include <order_book.hpp>

TEST_CASE("create order book test", "[unit]") {
    REQUIRE_NOTHROW(std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>());
}

TEST_CASE("new order test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 0, 0};
    REQUIRE(order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("new order sell test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 1, 0};
    REQUIRE(order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("new order buy sell test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message_buy{1'000, 940, 0, 0};
    REQUIRE(order_book->ProcessNewOrder(message_buy.quantity_, message_buy.price_, message_buy.side_));
    aecapital::Message message_sell{1'000, 960, 1, 0};
    REQUIRE(order_book->ProcessNewOrder(message_sell.quantity_, message_sell.price_, message_sell.side_));
}

TEST_CASE("change order test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 0, 1};
    REQUIRE(order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_));
    REQUIRE(order_book->ProcessModifyOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("change order sell test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 1, 1};
    REQUIRE(order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_));
    REQUIRE(order_book->ProcessModifyOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("delete order test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 0, 2};
    REQUIRE(order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_));
    REQUIRE(order_book->ProcessDeleteOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("delete order negative test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 0, 2};
    REQUIRE_FALSE(order_book->ProcessDeleteOrder(message.quantity_, message.price_, message.side_));
}

TEST_CASE("order book benchmark test", "[unit]") {
    auto               order_book = std::make_unique<aecapital::OrderBook<aecapital::MAX_LEVEL>>();
    aecapital::Message message{1'000, 940, 0, 0};
    BENCHMARK("new order") {
        order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_);
    };
    BENCHMARK("change order") {
        order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_);
        order_book->ProcessModifyOrder(message.quantity_, message.price_, message.side_);
    };
    BENCHMARK("delete order") {
        order_book->ProcessNewOrder(message.quantity_, message.price_, message.side_);
        order_book->ProcessDeleteOrder(message.quantity_, message.price_, message.side_);
    };
    std::random_device                     random_device;
    std::mt19937                           random_engine(random_device());
    std::uniform_int_distribution<int32_t> distribution_1_100(1, 10);
    BENCHMARK("random new order") {
        for (int32_t i = 0; i < 10; i++) {
            auto random_number = distribution_1_100(random_engine);
            for (int32_t j = 0; j < random_number; j++) {
                aecapital::Quantity quantity = distribution_1_100(random_engine);
                aecapital::Price    price    = distribution_1_100(random_engine);
                aecapital::Message  message_buy{quantity, price, 0, 0};
                aecapital::Message  message_sell{quantity, price, 1, 0};
                order_book->ProcessNewOrder(message_buy.quantity_, message_buy.price_, message_buy.side_);
                order_book->ProcessNewOrder(message_sell.quantity_, message_sell.price_, message_sell.side_);
            }
        }
    };

    BENCHMARK("random change order") {
        for (int32_t i = 0; i < 10; i++) {
            auto random_number = distribution_1_100(random_engine);
            for (int32_t j = 0; j < random_number; j++) {
                aecapital::Quantity quantity = distribution_1_100(random_engine);
                aecapital::Price    price    = distribution_1_100(random_engine);
                aecapital::Message  message_buy{quantity, price, 0, 1};
                aecapital::Message  message_sell{quantity, price, 1, 1};
                order_book->ProcessNewOrder(message_buy.quantity_, message_buy.price_, message_buy.side_);
                order_book->ProcessModifyOrder(message_buy.quantity_ + distribution_1_100(random_engine),
                                               message_buy.price_, message_buy.side_);
                order_book->ProcessNewOrder(message_sell.quantity_, message_sell.price_, message_sell.side_);
                order_book->ProcessModifyOrder(message_sell.quantity_ + distribution_1_100(random_engine),
                                               message_sell.price_, message_sell.side_);
            }
        }
    };
}