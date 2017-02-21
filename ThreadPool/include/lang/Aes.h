

#if !defined TAES_H
#define TAES_H

#include <string>

namespace lang {

class TAES

{
public:
    static TAES *NewObject ();
    static TAES *NewObject (unsigned char *AKey);
    static void DelObject (TAES *Object);
	unsigned char* Cipher(unsigned char* input);
	unsigned char* InvCipher(unsigned char* input);
	void* Cipher(void* input, int length=0);
	void* InvCipher(void* input, int length);
    void Encrypt (std::string &inout);
    void Decrypt (std::string &inout);
private:
	TAES (unsigned char* key);
	virtual ~TAES();
	unsigned char Sbox[256];
	unsigned char InvSbox[256];
	unsigned char w[11][4][4];

	void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][4]);
	void ShiftRows(unsigned char state[][4]);
	void MixColumns(unsigned char state[][4]);
	void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

	void InvSubBytes(unsigned char state[][4]);
	void InvShiftRows(unsigned char state[][4]);
	void InvMixColumns(unsigned char state[][4]);
};

};	


#endif
