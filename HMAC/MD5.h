#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iomanip>

#define blockSize 64

typedef unsigned int uInt; // 32-bit
typedef unsigned char uChar; // 8-bit(Byte)

class MD5 {
public:
	MD5();
	MD5(const std::string&);
	void update(const uChar*, uInt);
	void update(const char*, uInt);
	MD5& finalize();
	std::string hexdigest();
	std::string get_bytes();
	friend std::ostream& operator << (std::ostream&, MD5);
private:
	bool finish; 
	uInt count[2]; 
	uInt state[4]; 
	uChar result[16];
	uChar buffer[blockSize];

	void init();
	void transform(const uChar*);
	void decode(uInt*, const uChar*, uInt);
	void encode(uChar*, const uInt*, uInt);
};
std::string md5_bytes(const std::string str);
MD5 md5(const std::string);
std::string md5_digest(const std::string str);
inline uInt F(uInt, uInt, uInt);
inline uInt G(uInt, uInt, uInt);
inline uInt H(uInt, uInt, uInt);
inline uInt I(uInt, uInt, uInt);
inline uInt rotate_left(uInt, int);
inline void FF(uInt&, uInt, uInt, uInt, uInt, uInt, uInt);
inline void GG(uInt&, uInt, uInt, uInt, uInt, uInt, uInt);
inline void HH(uInt&, uInt, uInt, uInt, uInt, uInt, uInt);
inline void II(uInt&, uInt, uInt, uInt, uInt, uInt, uInt);