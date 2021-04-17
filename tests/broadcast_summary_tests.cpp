#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include <fstream>
#include <logger.hpp>
#include <message.hpp>
#include <order_book.hpp>
#include <ostream>

std::string filename;

int32_t main(int32_t argc, char* argv[]) {
    Catch::Session session;
    using namespace Catch::clara;
    auto cli = session.cli() | Opt(filename, "file name")["-g"]["--file_name"]("input file name");
    session.cli(cli);
    int32_t return_code = session.applyCommandLine(argc, argv);
    if (return_code != 0) {
        return return_code;
    }
    return session.run();
}

TEST_CASE("aecapital file test", "[unit]") {
    std::ifstream infile(filename.c_str(), std::ios::in | std::ios::binary);
    if (infile.good()) {
        std::ofstream                              out_file{"file.out"};
        int32_t                                    token = 0;
        aecapital::OrderBook<aecapital::MAX_LEVEL> order_book;
        while (infile.good() && !infile.eof()) {
            aecapital::MessageHeader message_header{};
            infile.read((char*)&message_header, sizeof(aecapital::MessageHeader));
            bool result = false;
            while (message_header.message_count_--) {
                aecapital::Message message{};
                infile.read((char*)&message, sizeof(aecapital::Message));
                auto type = static_cast<aecapital::TYPE>(message.type_);
                switch (type) {
                    case aecapital::TYPE::NEW:
                        result |= order_book.ProcessNewOrder(message.quantity_, message.price_, message.side_);
                        break;
                    case aecapital::TYPE::CHANGE:
                        result |= order_book.ProcessModifyOrder(message.quantity_, message.price_, message.side_);
                        break;
                    case aecapital::TYPE::DELETE:
                        result |= order_book.ProcessDeleteOrder(message.quantity_, message.price_, message.side_);
                        break;
                }
            }
            if (result) {
                order_book.PrintOrderbook(token, aecapital::Print);
                order_book.LogOrderbook(out_file, aecapital::Log);
            }
        }
        if (out_file.good()) {
            out_file.close();
        }
    }
    infile.close();
}