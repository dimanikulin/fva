#include <iostream>

#include "logging_definition.inl"
#include "mocks.h"

LOGGER_GLOBAL

int main() {
    std::cerr << "Starting unit tests..." << std::endl;
    LOG_INIT("tests.txt")
    std::cerr << "Unit tests completed." << std::endl;
    return 0;
}
