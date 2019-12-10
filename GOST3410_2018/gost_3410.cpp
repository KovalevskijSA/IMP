#include "gost_3410.h"
using namespace std;

mpz_class modinvert(const mpz_class &v, const mpz_class &m) {
	mpz_class temp;
	mpz_invert(temp.get_mpz_t(), v.get_mpz_t(), m.get_mpz_t());
	return temp;
}

mpz_class normed(const mpz_class &x, const mpz_class &p) {
	if (x < 0)
		return x + (-x/p + 1) * p;
	return x;
}

EPoint sum(const EPoint &p1, const  EPoint& p2, const EParams &parm) {
	mpz_class lambda, p = parm.p;
	EPoint res;
	if (p1 == p2) {
		lambda = ((3 * p1.x * p1.x + parm.a) * modinvert(2 * p1.y, p)) % p;
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

EPoint multiply(const mpz_class &mul, const EPoint &p_in, const EParams &prm) {
	EPoint r = p_in;
	EPoint t = p_in;
	mpz_class c = mul;
	c = c - 1;
	while (c != 0) {
		if ((c % 2) != 0) {
			r = sum(r, t, prm);
		}
		c = c / 2;
		t = sum(t, t, prm);
	}
	return r;
}


EPoint get_signature(const mpz_class &prv_key, const EParams &prm, const mpz_class &mes) {
	mpz_class q = prm.q, k, r, s;
	mpz_class e = mes % prm.q;
	EPoint xy = prm.xy;
	if (e == 0) {
		e = 1;
	}

	std::random_device rd;
	gmp_randclass rnd(gmp_randinit_default);
	rnd.seed(rd());

	while (true) {
		k = rnd.get_z_range(q - 2) + 1;
		r = multiply(k, xy, prm).x % q;
		if (r == 0)
			continue;
		s = (prv_key * r + k*e) % q;
		if (s == 0) continue;
		break;
	}
	return { r, s };
}

bool verify(const EPoint& pbl_key, const EPoint& sign, const EParams& prm, const mpz_class& mes) {//mes - хэш от mes
	mpz_class e, r=sign.x, s = sign.y, q = prm.q, p = prm.p, v, z1, z2, m;
	EPoint ppnt, qpnt, c, xy = prm.xy;
	if (r <= 0 || r >= q || s <= 0 || s >= q)
		return false;
	e = mes % q;
	if (e == 0)
		e = 1;
	v = modinvert(e, q);
	z1 = (s * v) % q;
	z2 =  normed((- r * v) % q, q);
	ppnt = multiply(z1, xy, prm);
	qpnt = multiply(z2, pbl_key, prm);
	c = sum(ppnt, qpnt, prm);
	return c.x % q == r;
}

EPoint public_key(const mpz_class& prv_key, const EParams& prm) {
	EPoint pp = prm.xy;
	return multiply(prv_key, pp, prm);
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
