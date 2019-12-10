#include "elliptic.h"
EPoint Elliptic::sum(const EPoint& p1, const  EPoint& p2) {
	mpz_class lambda;
	EPoint res;
	if (p1 == p2) {
		lambda = ((3 * p1.x * p1.x + a) * modinvert(2 * p1.y, p)) % p;
	}
	else {
		res.x = normed(p2.x - p1.x, p) % p;
		res.y = normed(p2.y - p1.y, p) % p;
		lambda = (res.y * modinvert(res.x, p)) % p;
	}
	res.x = normed(lambda * lambda - p1.x - p2.x, p) % p;
	res.y = normed(lambda * (p1.x - res.x) - p1.y, p) % p;
	return res;
}

EPoint Elliptic::multiply(const mpz_class& mul, const EPoint& p_in) {
	EPoint r = p_in;
	EPoint t = p_in;
	mpz_class c = mul;
	c = c - 1;
	while (c != 0) {
		if ((c % 2) != 0) {
			r = sum(r, t);
		}
		c = c / 2;
		t = sum(t, t);
	}
	return r;
}

mpz_class modinvert(const mpz_class& v, const mpz_class& m) {
	mpz_class temp;
	mpz_invert(temp.get_mpz_t(), v.get_mpz_t(), m.get_mpz_t());
	return temp;
}

mpz_class normed(const mpz_class& x, const mpz_class& p) {
	if (x < 0)
		return x + (-x / p + 1) * p;
	return x;
}

mpz_class private_key(int keysize) {
	std::random_device rd;
	mpz_t tmp;
	mpz_init(tmp);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, rd());

	mpz_urandomb(tmp, state, keysize);
	mpz_setbit(tmp, keysize - 1);
	mpz_class r{ tmp };
	return r;
}
