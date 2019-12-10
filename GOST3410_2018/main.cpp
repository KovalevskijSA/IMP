#include <iostream>
#include <string>
#include "gost_3410.h"

using namespace std;
/*
info 
https://files.stroyinf.ru/Data/705/70512.pdf
sample 1. 
*/

int main(int argc, char** argv) {
	mpz_class p("57896044618658097711785492504343953926634992332820282019728792003956564821041");
	mpz_class q("57896044618658097711785492504343953927082934583725450622380973592137631069619");
	mpz_class a = 7;
	mpz_class b("43308876546767276905765904595650931995942111794451039583252968842033849580414");
	mpz_class x = 2;
	mpz_class y("4018974056539037503335449422937059775635739389905545080690979365213431566280");
	//hash
	mpz_class m("57896044618658097711785492504343953927082934583725450622380973592137631069619");

	//key
	mpz_class d("55441196065363246126355624130324183196576709222340016572108097750006097525544");
	//verification key
	mpz_class x_d("57520216126176808443631405023338071176630104906313632182896741342206604859403");
	mpz_class y_d("17614944419213781543809391949654080031942662045363639260709847859438286763994");
	
	
	EParams prm(p, q, a, b, x, y);
	/*
	key generation
	d = private_key(256);
	EPoint kp = public_key(d, prm);
	cout << kp.x.get_str() << endl;
	cout << kp.y.get_str() << endl;
	cout << d.get_str() << endl;
	x_d = kp.x;
	y_d = kp.y;
	*/

	EPoint s = get_signature(d, prm, m);
	cout << "signature" << endl;
	cout << s.x.get_str() << endl << s.y.get_str() << endl;
	cout << "verification" << endl;
	cout << verify({ x_d, y_d }, s, prm, m);

	return 0;
}