local function ror32(x, n)
    n = n & 31
    return ((x >> n) | (x << (32 - n))) & 0xFFFFFFFF
end


local K = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
}

local function str_to_words(str)
    local words = {}
    local n = #str
    for i = 1, n, 4 do
        local b1 = string.byte(str, i) or 0
        local b2 = string.byte(str, i+1) or 0
        local b3 = string.byte(str, i+2) or 0
        local b4 = string.byte(str, i+3) or 0
        table.insert(words, b1 << 24 | b2 << 16 | b3 << 8 | b4)
    end
    return words
end

local function sha256(msg)
    local H = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}            
    local msg_len = #msg
    local bit_len = msg_len * 8
    msg = msg .. "\128"
    local pad_len = (56 - (#msg % 64)) % 64
    msg = msg .. string.rep("\0", pad_len)

    msg = msg .. string.char(
        0, 0, 0, 0,
        bit_len >> 24 & 0xFF,
        bit_len >> 16 & 0xFF,
        bit_len >> 8 & 0xFF,
        bit_len & 0xFF
    )

    local words = str_to_words(msg)
    local W = {}

    for chunk = 1, #words, 16 do
        for i = 1, 16 do W[i] = words[chunk + i - 1] end
        for i = 17, 64 do
            local s0 = ror32(W[i-15], 7) ~ ror32(W[i-15], 18) ~ (W[i-15] >> 3)
            local s1 = ror32(W[i-2], 17) ~ ror32(W[i-2], 19) ~ (W[i-2] >> 10)
            W[i] = (W[i-16] + s0 + W[i-7] + s1) & 0xFFFFFFFF
        end

        local a, b, c, d, e, f, g, h = H[1], H[2], H[3], H[4], H[5], H[6], H[7], H[8]

        for i = 1, 64 do
            local S1 = ror32(e, 6) ~ ror32(e, 11) ~ ror32(e, 25)
            local ch = (e & f) ~ (~e & g)
            local temp1 = (h + S1 + ch + K[i] + W[i]) & 0xFFFFFFFF
            local S0 = ror32(a, 2) ~ ror32(a, 13) ~ ror32(a, 22)
            local maj = (a & b) ~ (a & c) ~ (b & c)
            local temp2 = (S0 + maj) & 0xFFFFFFFF

            h, g, f = g, f, e
            e = (d + temp1) & 0xFFFFFFFF
            d, c, b = c, b, a
            a = (temp1 + temp2) & 0xFFFFFFFF
        end

        H[1] = (H[1] + a) & 0xFFFFFFFF
        H[2] = (H[2] + b) & 0xFFFFFFFF
        H[3] = (H[3] + c) & 0xFFFFFFFF
        H[4] = (H[4] + d) & 0xFFFFFFFF
        H[5] = (H[5] + e) & 0xFFFFFFFF
        H[6] = (H[6] + f) & 0xFFFFFFFF
        H[7] = (H[7] + g) & 0xFFFFFFFF
        H[8] = (H[8] + h) & 0xFFFFFFFF
    end

    local result = ""
    for i = 1, 8 do
        result = result .. string.format("%08x", H[i])
    end
    return result
end

sha256("1")