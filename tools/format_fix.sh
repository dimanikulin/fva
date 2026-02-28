find tests -regex '.*\.\(cpp\|h\|cc\|cxx\)' -exec clang-format-17 -i {} \;
find src -regex '.*\.\(cpp\|h\|cc\|cxx\)' -exec clang-format-17 -i {} \;
