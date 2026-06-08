#include "fvalogger.inl"

LOGGER_GLOBAL

int main() {
    FvaConfiguration cfg;
    const std::string cfgPath = (appDir / "fvaParams.csv").string();
    FVA_EXIT_CODE exitCode = cfg.load(cfgPath);
    if (exitCode != FVA_NO_ERROR) {
        return exitCode;
    }

    LOG_INIT("#logs#/tests.txt")
    return 0;
}
