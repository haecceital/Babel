#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <optional>


std::optional<size_t> KMP(const std::string &str, const std::string &pattern) {
    const size_t len = pattern.length();
    std::vector<size_t> next(len, 0);

    size_t prev = 0, cur = 1;
    while(cur < len) {
        if (pattern[cur] == pattern[prev]) {
            next[cur] = prev + 1;
            prev++, cur++;
        } else if (prev == 0) cur++;
        else prev = next[prev - 1];
    }

    size_t j = 0;
    for(size_t i = 0; i < str.length(); ++i) {
        while (j > 0 && str[i] != pattern[j]) j = next[j - 1];
    	if (str[i] == pattern[j]) j++;

    	if (j == len) return i - len + 1;
    }

    return std::nullopt;
}