class Curve:
    def __init__(self, a: int, b: int, p: int):
        self.a = a
        self.b = b
        self.p = p


class Point:
    def __init__(self, x: int | None, y: int | None):
        self.x = x
        self.y = y

    def __eq__(self, other):
        if not isinstance(other, Point):
            return NotImplemented

        return self.x == other.x and self.y == other.y


idp = Point(None, None)


def pointeadd(ec: Curve, p1: Point, p2: Point) -> Point:
    if p1 is idp or p2 is idp:
        return p1 if p2 is idp else p2

    slope = None
    if p1 == p2:
        if (2 * p1.y) % ec.p == 0:
            return idp
        slope = ((3 * p1.x**2 + ec.a) * pow(2 * p1.y, -1, ec.p)) % ec.p
    else:
        if (p2.x - p1.x) % ec.p == 0:
            return idp
        slope = ((p2.y - p1.y) * pow(p2.x - p1.x, -1, ec.p)) % ec.p

    x2 = (slope**2 - p1.x - p2.x) % ec.p
    y2 = (slope * (p1.x - x2) - p1.y) % ec.p
    return Point(x2, y2)


def fastadd(ec: Curve, p: Point, d: int) -> Point:
    if d == 0:
        return idp
    elif d == 1:
        return p
    elif d & 1:
        return pointeadd(ec, fastadd(ec, p, d - 1), p)
    else:
        return fastadd(ec, pointeadd(ec, p, p), d // 2)


def ECC(ec: Curve, G: Point, d: int) -> Point:
    if (4 * ec.a**3 + 27 * ec.b**2) % ec.p == 0:
        raise ValueError

    return fastadd(ec, G, d)


# test ↓

secp256k1 = Curve(
    a=0, b=7, p=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
)
G = Point(
    x=0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
    y=0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8,
)

d_alice = 0x123456789ABCDEF
Q_alice = ECC(secp256k1, G, d_alice)

k_bob = 0xFEDCBA987654321
R_bob = ECC(secp256k1, G, k_bob)
S_bob = ECC(secp256k1, Q_alice, k_bob)

S_alice = ECC(secp256k1, R_bob, d_alice)

if S_bob == S_alice:
    print("success!")
