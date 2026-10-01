from math import pi
from cmath import exp


def chop(n: complex, eps: float = 1e-9) -> complex:
    return complex(
        n.real if abs(n.real) >= eps else 0,
        n.imag if abs(n.imag) >= eps else 0
    )


def dft(xs: list) -> list:
    N = len(xs)
    X = []
    
    for k in range(N):
        X.append(chop(sum(xs[n] * exp(-1j * 2 * pi / N * k * n) for n in range(N))))

    return X