from collections.abc import Generator


btoi = lambda b: int.from_bytes(b, byteorder = "little")

def rol64(val: int, shift: int) -> int:
    shift %= 64
    mask = (1 << 64) - 1
    return ((val << shift) | (val >> (64 - shift))) & mask

def generate_round_constants() -> list[int]:
    constants = []
    lfsr_state = 0x01

    for _ in range(24):
        constant = 0
        for j in range(7):
            bit_pos = (1 << j) - 1
            
            if lfsr_state & 1: constant ^= (1 << bit_pos)
            
            bit0 = (lfsr_state >> 0) & 1
            bit4 = (lfsr_state >> 4) & 1
            bit5 = (lfsr_state >> 5) & 1
            bit6 = (lfsr_state >> 6) & 1
            feedback = bit0 ^ bit4 ^ bit5 ^ bit6
            
            lfsr_state = (lfsr_state >> 1) | (feedback << 7)
            
        constants.append(constant)
        
    return constants
round_constants = generate_round_constants()

def slice_and_pad(data: bytes, byte_rate: int, end: int) -> Generator[list[int], None, None]:
    buf = bytearray(data)
    buf.append(end)

    pad_len = byte_rate - (len(buf) % byte_rate)
    if pad_len == byte_rate: pad_len = 0

    buf.extend(b"\x00" * pad_len)
    buf[-1] |= 0x80
    data = bytes(buf)

    r8 = byte_rate // 8
    for bl in range(len(data) // byte_rate):
        bl_id = bl * byte_rate
        yield [btoi(data[bl_id + 8 * i:bl_id + 8 * (i + 1)]) for i in range(r8)] + [0] * (25 - r8)


slice_and_pad_sha = lambda d, br: slice_and_pad(d, br, 0x06)
slice_and_pad_shake = lambda d, br: slice_and_pad(d, br, 0x1F)

def absorb(state: list[int], block: list[int]):
    for i in range(len(state)):
        state[i] ^= block[i]

def theta(state: list[int]):
    C = [0] * 5
    for x in range(5):
        for y in range(5):
            C[x] ^= state[x + (5 * y)]

    D = [C[(x - 1) % 5] ^ rol64(C[(x + 1) % 5], 1) for x in range(5)]
    for x in range(5):
        for y in range(5):
            state[x + 5 * y] ^= D[x]

def rho(state: list[int]):
    x, y = 1, 0
    for t in range(24):
        offset = ((t + 1) * (t + 2) // 2) % 64
        state[x + 5 * y] = rol64(state[x + 5 * y], offset)
        x, y = y, (2 * x + 3 * y) % 5

def pi(state: list[int]):
    copied = state.copy()
    for x in range(5):
        for y in range(5):
            new_x = y
            new_y = (2 * x + 3 * y) % 5
            state[new_x + 5 * new_y] = copied[x + 5 * y]

def chi(state: list[int]):
    copied = state.copy()
    for y in range(5):
        for x in range(5):
            x1 = (x + 1) % 5
            x2 = (x + 2) % 5
            idx = x + 5 * y

            not_x1 = (~copied[x1 + 5 * y]) & 0xFFFFFFFFFFFFFFFF
            state[idx] = copied[idx] ^ (not_x1 & copied[x2 + 5 * y])

def iota(state: list[int], round_idx: int):
    state[0] ^= round_constants[round_idx]


def keccak_f1600(state: list[int]):
    for round_idx in range(24):
        theta(state)
        rho(state)
        pi(state)
        chi(state)
        iota(state, round_idx)


def sha3_n(data: bytes, n: int) -> bytes:
    byte_rate = (1600 - 2 * n) // 8
    output_len = n // 8

    state = [0] * 25

    for block_words in slice_and_pad_sha(data, byte_rate):
        absorb(state, block_words)
        keccak_f1600(state)

    out = bytearray()
    for word in state:
        out.extend(word.to_bytes(8, byteorder = "little"))
        if len(out) >= output_len: break

    return bytes(out[:output_len])


def sha3_224(data: bytes) -> bytes:
    return sha3_n(data, 224)

def sha3_256(data: bytes) -> bytes:
    return sha3_n(data, 256)

def sha3_384(data: bytes) -> bytes:
    return sha3_n(data, 384)

def sha3_512(data: bytes) -> bytes:
    return sha3_n(data, 512)


def shake_n(data: bytes, n: int, output_len: int) -> bytes:
    byte_rate = (1600 - 2 * n) // 8
    state = [0] * 25

    for block_words in slice_and_pad_shake(data, byte_rate):
        absorb(state, block_words)
        keccak_f1600(state)

    out = bytearray()
    r8 = byte_rate // 8
    while len(out) < output_len:
        for i in range(r8):
            out.extend(state[i].to_bytes(8, byteorder = "little"))
            if len(out) >= output_len: break
        
        if len(out) < output_len: keccak_f1600(state)

    return bytes(out[:output_len])


def shake_128(data: bytes, output_len) -> bytes:
    return shake_n(data, 128, output_len)

def shake_256(data: bytes, output_len) -> bytes:
    return shake_n(data, 256, output_len)