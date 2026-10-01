def find_primitive_root(p: int) -> int:
    if p == 2: return 1

    max_pfactor = []
    p_m1 = p - 1
    tmp = p_m1
    d = 2
    while d * d <= tmp:
        if tmp % d == 0:
            max_pfactor.append(p_m1 // d)
            while tmp % d == 0: tmp //= d
        d += 1
    if tmp > 1: max_pfactor.append(p_m1 // tmp)

    for g in range(2, p):
        is_primitive = True
        for mp in max_pfactor:
            if pow(g, mp, p) == 1:
                is_primitive = False
                break
        if is_primitive: return g

    return -1
    

def ntt(xs: list, p: int, g: int = -1) -> list:
    if g == -1: g = find_primitive_root(p)
    N = len(xs)
    
    if N <= 1: return xs
    
    even = ntt(xs[0::2], p, g)
    odd = ntt(xs[1::2], p, g)

    wn = pow(g, (p - 1) // N, p)
    w = 1
    
    combined = [0] * N
    N_half = N // 2
    for k in range(N_half):
        odd_part = (w * odd[k]) % p
        
        combined[k] = (even[k] + odd_part) % p
        combined[k + N_half] = (even[k] - odd_part) % p
        
        w = (w * wn) % p

    return combined


def intt(A: list, p: int, g: int = -1) -> list:
    if g == -1:  g = find_primitive_root(p)
        
    N = len(A)
    g_inv = pow(g, p - 2, p)
    
    res = ntt(A, p, g_inv)
    
    N_inv = pow(N, p - 2, p)
    return [(x * N_inv) % p for x in res]