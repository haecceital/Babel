# Extended Euclidean Algorithm
def eea(a: int, b: int) -> tuple[int, int, int]:
    if b > a:
        a, b = b, a

    if b == 0:
        return (1, 0, a)

    x, y, gcd = eea(b, a % b)
    nx = y
    ny = x - (a // b) * y
    return (nx, ny, gcd)
