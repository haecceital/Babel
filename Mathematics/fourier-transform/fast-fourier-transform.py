from math import pi
from cmath import exp


def chop(n: complex, eps: float = 1e-9) -> complex:
    return complex(
        n.real if abs(n.real) >= eps else 0,
        n.imag if abs(n.imag) >= eps else 0
    )


def fft(xs: list) -> list:
    N = len(xs)
    
    if N <= 1: return xs
    
    even = fft(xs[0::2])
    odd = fft(xs[1::2])
    
    combined = [0] * N
    for k in range(N // 2):
        w = exp(-2j * pi * k / N)
        
        combined[k] = chop(even[k] + w * odd[k])
        combined[k + N // 2] = chop(even[k] - w * odd[k])

    return combined