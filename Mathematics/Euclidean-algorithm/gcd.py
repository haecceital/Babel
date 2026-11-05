# Euclidean Algorithm
def ea(a: int, b: int) -> int:
    if b > a:
        a, b = b, a

    if b == 0:
        return a

    return ea(a % b, b)
