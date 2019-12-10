#include "MD5.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <istream>
#include <fstream>
#include <iterator>

/*
hmac online
https://cryptii.com/pipes/hmac

parametrs: file_name key_file

key in hex

output: hex

*/

using namespace std;

string hex_string_to_string(const string& h_str) {
	std::stringstream ss;
	ss >> hex;
	string res(h_str.size() / 2, '\0');
	int v;
	for (int i = 0; i < h_str.size()/2; i++) {
		ss << h_str.substr(2*i, 2);
		ss >> v;
		res[i] = v;
	}
	return res;
}

string string_to_hex_string(const string& str) {
	std::stringstream oss;
	oss << std::hex;
	for (int i = 0; i < str.size(); i++) {
		oss << std::setw(2) << std::setfill('0') << (int)(unsigned char)str[i];
	}
	return oss.str();
}

string hmac(const string& b_key, const string& b_data, string(*hash)(string), int block_size) {
	string key_t(b_key);
	if (key_t.size() > block_size) key_t = hash(key_t);
	if (key_t.size() < block_size) key_t.resize(block_size, '\0');
	string ikeypad(block_size, 0x36);
	string okeypad(block_size, 0x5c);
	for (int i = 0; i < block_size; i++) {
		ikeypad[i] ^= key_t[i];
		okeypad[i] ^= key_t[i];
	}
	return hash(okeypad + hash(ikeypad + b_data));
}
//
//int main() {
//	cout << string_to_hex_string(
//		hmac(
//			hex_string_to_string("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"),
//			"Hi There", 
//			md5_bytes,
//			64));
//	return 0;
//}


int main(int argc, char** argv) {
	if (argc == 3) {
		string input_file = argv[1];
		string key_file = argv[2];
		ifstream in(input_file, ifstream::binary);
		ifstream in_key(key_file);
		string key;
		in_key >> key;
		string input_text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
		cout << string_to_hex_string(hmac(hex_string_to_string(key), input_text, md5_bytes, 64));
		in.close();
		in_key.close();
	}
	return 0;
}