#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <optional>


std::optional<size_t> RabinKarp(const std::string &str, const std::string &pattern, long long base = 31, long long prime = 1e9+7) {
    const size_t len = pattern.length();
    if (str.length() < len) return std::nullopt;

    long long hashPattern = 0, hashStr = 0;
    for(size_t i = 0; i < len; ++i) {
        hashPattern = (hashPattern * base + (long long)pattern[i]) % prime;
        hashStr = (hashStr * base + (long long)str[i]) % prime;
    }

    auto check = [&](size_t start) -> bool {
        for (size_t k = 0; k < len; ++k) {
            if (pattern[k] != str[start + k]) return false;
        }
        return true;
    };

    if (hashStr == hashPattern && check(0)) return std::make_optional(0);

    long long hp = 1;
    for (size_t i = 0; i < len - 1; ++i) hp = (hp * base) % prime;

    for (size_t i = len; i < str.length(); ++i) {
        hashStr = (hashStr - str[i - len] * hp) % prime;
        if (hashStr < 0) hashStr += prime;
        hashStr = (hashStr * base + str[i]) % prime;

        size_t start = i - len + 1;
        if (hashStr == hashPattern && check(start)) return std::make_optional(start);
    }


    return std::nullopt;
}