# fmt: off
# Babel/Mathematics/Euclidean-algorithm/exgcd.py
eea = lambda a,b:(b>a and(a:=a+b,b:=a-b,a:=a-b),1)and(b==0 and(1,0,a))or(t:=eea(b,a%b),(t[1],t[0]-(a//b)*t[1],t[2]))[1]
# fmt: on


def RSA(p: int, q: int, e: int = 65537) -> tuple[int, int]:
    n = p * q
    phi = (p - 1) * (q - 1)
    d, _, gcd = eea(e, phi)

    if gcd != 1:
        raise ValueError

    d %= phi

    return (d, n)


# decrypt + encrypt
def dencrypt(m: int, ed: int, n: int) -> int:
    return pow(m, ed, n)
