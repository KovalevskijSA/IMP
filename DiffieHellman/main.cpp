#include <iostream>
#include <string>
#include "elliptic.h"

using namespace std;
/*
info
https://en.wikipedia.org/wiki/Elliptic-curve_Diffie%E2%80%93Hellman
*/


int main(int argc, char** argv) {
	mpz_class p("57896044618658097711785492504343953926634992332820282019728792003956564821041");
	mpz_class q("57896044618658097711785492504343953927082934583725450622380973592137631069619");
	mpz_class a = 7;
	mpz_class b("43308876546767276905765904595650931995942111794451039583252968842033849580414");
	mpz_class x = 2;
	mpz_class y("4018974056539037503335449422937059775635739389905545080690979365213431566280");

	mpz_class d_a, d_b;
	EPoint G(x, y), q_a, q_b;
	Elliptic el(p, a, b);
	//key 1 (d_a, q_a)
	d_a = private_key(256);	
	q_a = el.multiply(d_a, G);

	//key 2 (d_b, q_b)
	d_b = private_key(256);
	q_b = el.multiply(d_b, G);

	//shared secret key
	EPoint t = el.multiply(d_a, q_b);
	mpz_class x_a = t.x;

	cout << "Alice" << endl;
	cout << "d " << d_a.get_str() << endl;
	cout << "Q :\n" << q_a.x.get_str() << endl << q_a.y.get_str() << endl;
	cout << "Bob" << endl;
	cout << "d " << d_b.get_str() << endl;
	cout << "Q :\n" << q_b.x.get_str() << endl << q_b.y.get_str() << endl;
	cout << "shared secret key: \n" << x_a.get_str() << endl;
	return 0;
}