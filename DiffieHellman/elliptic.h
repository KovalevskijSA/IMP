#include <mpirxx.h>
#include <tuple>
#include <random>

class EPoint {
public:
	mpz_class x, y;
	EPoint() {}
	EPoint(mpz_class x, mpz_class y) :x(x), y(y) {}

	EPoint& operator=(const EPoint& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	friend bool operator ==(const EPoint& p1, const EPoint& p2) {
		if (p1.x == p2.x && p1.y == p2.y)
			return true;
		return false;
	}
};

class Elliptic {
public:
	mpz_class p, a, b;
	Elliptic(mpz_class p, mpz_class a, mpz_class b) :
		p(p), a(a), b(b) {}
	EPoint sum(const EPoint& p1, const  EPoint& p2);
	EPoint multiply(const mpz_class& mul, const EPoint& p_in);
};

mpz_class modinvert(const mpz_class& v, const mpz_class& m);
mpz_class normed(const mpz_class& x, const mpz_class& p);
mpz_class private_key(int keysize);