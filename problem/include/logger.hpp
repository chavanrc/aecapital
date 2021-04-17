#pragma once

#include <iostream>
#include <sstream>

#ifdef BENCHMARK_ENABLE
#define LOG_DEBUG(TXT)
#define LOG_INFO(TXT)
#define LOG_ERROR(TXT)
#else
#define LOG_DEBUG(TXT) std::cout << "[DEBUG] " << TXT << '\n'
#define LOG_INFO(TXT) std::cout << "[INFO] " << TXT << '\n'
#define LOG_ERROR(TXT) std::cerr << "[ERROR] " << TXT << '\n'
#endif