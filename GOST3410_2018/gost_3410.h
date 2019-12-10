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

struct EParams {
	mpz_class p, q, a, b;
	EPoint xy;
	EParams(mpz_class p, mpz_class q, mpz_class a, mpz_class b, mpz_class x, mpz_class y):
		p(p), q(q), a(a), b(b), xy(x, y) {}
};


EPoint public_key(const mpz_class& prv_key, const EParams& prm);

mpz_class private_key(int keysize);

EPoint get_signature(const mpz_class& prv_key, const EParams& prm, const mpz_class& mes);

bool verify(const EPoint& pbl_key, const EPoint& sign, const EParams& prm, const mpz_class& mes);