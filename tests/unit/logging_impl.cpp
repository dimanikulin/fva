#include "fvaconfiguration.h"
#include "fvaexitcodes.h"
#include "fvalogger.inl"
#include "mocks.h"

LOGGER_GLOBAL

int main() {
    FvaConfiguration cfg;
    std::filesystem::path appDir = std::filesystem::current_path();
    const std::string cfgPath = (appDir / "fvaParams.csv").string();
    FVA_EXIT_CODE exitCode = cfg.load(cfgPath);
    if (exitCode != FVA_NO_ERROR) {
        return exitCode;
    }

    LOG_INIT("#logs#/tests.txt")
    return 0;
}
