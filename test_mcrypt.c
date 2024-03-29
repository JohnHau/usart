//AES.c
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
/*
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 */
#include <mcrypt.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int encrypt(
		void* buffer,
		int buffer_len, /* Because the plaintext could include null bytes*/
		char* IV, 
		char* key,
		int key_len 
	   ){
	MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
	int blocksize = mcrypt_enc_get_block_size(td);
	if( buffer_len % blocksize != 0 ){return 1;}
	mcrypt_generic_init(td, key, key_len, IV);
	mcrypt_generic(td, buffer, buffer_len);
	mcrypt_generic_deinit (td);
	mcrypt_module_close(td);
	return 0;
}




int decrypt(
		void* buffer,
		int buffer_len,
		char* IV, 
		char* key,
		int key_len 
	   ){
	MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
	int blocksize = mcrypt_enc_get_block_size(td);
	if( buffer_len % blocksize != 0 ){return 1;}
	mcrypt_generic_init(td, key, key_len, IV);
	mdecrypt_generic(td, buffer, buffer_len);
	mcrypt_generic_deinit (td);
	mcrypt_module_close(td);
	return 0;
}




void display(char* ciphertext, int len){
	int v;
	for (v=0; v<len; v++){
		printf("%d ", ciphertext[v]);
	}
	printf("\n");
}






int main()
{
	MCRYPT td, td2;
	char * plaintext = "test text 123";
	char* IV = "AAAAAAAAAAAAAAAA";
	char *key = "0123456789abcdef";
	int keysize = 16; /* 128 bits */
	char* buffer;
	int buffer_len = 16;
	buffer = calloc(1, buffer_len);
	strncpy(buffer, plaintext, buffer_len);
	printf("==C==\n");
	printf("plain: %s\n", plaintext);
	encrypt(buffer, buffer_len, IV, key, keysize); 
	printf("cipher: "); display(buffer , buffer_len);
	decrypt(buffer, buffer_len, IV, key, keysize);
	printf("decrypt: %s\n", buffer);

	return 0;
}





//================================================
#if 0
mcrypt(3) - Linux man page
Name
libmcrypt - encryption/decryption library
Synopsis
[see also mcrypt.h for more information]

Description
The libmcrypt is a data encryption library.
The library is thread safe and provides encryption and decryption functions.
This version of the library supports many encryption algorithms and encryption modes. 
Some algorithms which are supported: 
SERPENT, RIJNDAEL, 3DES, GOST, SAFER+, CAST-256, RC2, XTEA, 
3WAY, TWOFISH, BLOWFISH, ARCFOUR, WAKE and more.


OFB, CBC, ECB, nOFB, nCFB and CFB are the modes that all algorithms may function. 
ECB, CBC, encrypt in blocks but CTR, nCFB, nOFB, CFB and OFB in bytes (streams).
Note that CFB and OFB in the rest of the document represent the "8bit CFB or OFB" mode. 
nOFB and nCFB modes represents a n-bit OFB/CFB mode, n is used to represent the algorithm's block size. 
The library supports an extra STREAM mode to include some stream algorithms like WAKE or ARCFOUR.


In this version of the library all modes and algorithms are modular, 
which means that the algorithm and the mode is loaded at run-time. 
This way you can add algorithms and modes faster, and much easier.

LibMcrypt includes the following symmetric (block) algorithms:

DES: 
The traditional DES algorithm designed by IBM and US NSA. 
Uses 56 bit key and 64 bit block. 
It is now considered a weak algorithm, due to its small key size (it was never intended for use with classified data).

3DES or Triple DES: DES but with multiple (triple) encryption. 
It encrypts the plaintext once, then decrypts it with the second key, 
and encrypts it again with the third key (outer cbc mode used for cbc). 
Much better than traditional DES since the key is now 168 bits 
(actually the effective key length is 112 bits due to the meet-in-the-middle attack).


CAST-128: 
CAST was designed in Canada by Carlisle Adams and Stafford Tavares. 
The original algorithm used a 64bit key and block. 
The algorithm here is CAST-128 (also called CAST5) which has a 128bit key and 64bit block size.


CAST-256: 
CAST-256 was designed by Carlisle Adams. 
It is a symmetric cipher designed in accordance with the CAST design procedure. 
It is an extention of the CAST-128, having a 128 bit block size, and up to 256 bit key size.


xTEA: TEA stands for the Tiny Encryption Algorithm. 
It is a feistel cipher designed by David Wheeler & Roger M. Needham. 
The original TEA was intended for use in applications where code size is at a premium, 
or where it is necessary for someone to remember the algorithm and code it on an arbitrary machine at a later time. 
The algorithm used here is extended TEA and has a 128bit key size and 64bit block size.

3-WAY: 
The 3way algorithm designed by Joan Daemen. It uses key and block size of 96 bits.

SKIPJACK: 
SKIPJACK was designed by the US NSA. 
It was part of the ill-fated "Clipper" Escrowed Encryption Standard (EES) (FIPS 185) proposal. 
It operates on 64bit blocks and uses a key of 80 bits. SKIPJACK is provided only as an extra module to libmcrypt.

BLOWFISH: 
The Blowfish algorithm designed by Bruce Schneier. 
It is better and faster than DES. It can use a key up to 448 bits.



TWOFISH: 
Twofish was designed by Bruce Schneier, Doug Whiting, John Kelsey, Chris Hall, David Wagner for Counterpane systems. 
Intended to be highly secure and highly flexible. 
It uses a 128bit block size and 128,192,256 bit key size. (Twofish is the default algorithm)

LOKI97: 
LOKI97 was designed by Lawrie Brown and Josef Pieprzyk. 
It has a 128-bit block length and a 256bit key schedule, which can be initialized using 128, 192 or 256 bit keys. 
It has evolved from the earlier LOKI89 and LOKI91 64-bit block ciphers, 
with a strengthened key schedule and a larger keyspace.

RC2: 
RC2 (RC stands for Rivest Cipher) was designed by Ron Rivest. 
It uses block size of 64 bit and a key size from 8 to 1024 bits. 
It is optimized for 16bit microprocessors (reflecting its age). It is described in the RFC2268.




ARCFOUR: 
RC4 was designed by Ron Rivest. 
For several years this algorithm was considered a trade secret and details were not available. 
In September 1994 someone posted the source code in the cypherpunks mailing list. 
Although the source code is now available RC4 is trademarked by RSADSI 
so a compatible cipher named ARCFOUR is included in the mcrypt distribution. 
It is a stream cipher and has a maximum key of 2048 bits.

RC6: 
RC6 was designed by Ron Rivest for RSA labs. 
In mcrypt it uses block size of 128 bit and a key size of 128/192/256 bits. 
Refer to RSA Labs and Ron Rivest for any copyright, patent or license issues for the RC6 algorithm. 
RC6 is provided only as an extra module to libmcrypt.

RIJNDAEL: 
Rijndael is a block cipher, designed by Joan Daemen and Vincent Rijmen, 
and was approved for the USA's NIST Advanced Encryption Standard, FIPS-197. 
The cipher has a variable block length and key length. 
Rijndael can be implemented very efficiently on a wide range of processors and in hardware. 
The design of Rijndael was strongly influenced by the design of the block cipher Square. 
There exist three versions of this algorithm, namely: 
RIJNDAEL-128 (the AES winner) , RIJNDAEL-192 , RIJNDAEL-256 
The numerals 128, 192 and 256 stand for the length of the block size.

MARS: 
MARS is a 128-bit block cipher designed by IBM as a candidate for the Advanced Encryption Standard. 
Refer to IBM for any copyright, patent or license issues for the MARS algorithm. 
MARS is provided only as an extra module to libmcrypt.

PANAMA: 
PANAMA is a cryptographic module that can be used both as a cryptographic hash function and as a stream cipher. 
It designed by Joan Daemen and Craig Clapp. PANAMA (the stream cipher) is included in libmcrypt.

WAKE: 
WAKE stands for Word Auto Key Encryption, 
and is an encryption system for medium speed encryption of blocks and of high security. 
WAKE was designed by David J. Wheeler. 
It is intended to be fast on most computers and relies on repeated table use and having a large state space.



SERPENT: 
Serpent is a 128-bit block cipher designed by Ross Anderson, 
Eli Biham and Lars Knudsen as a candidate for the Advanced Encryption Standard. 
Serpent's design was limited to well understood mechanisms, 
so that could rely on the wide experience of block cipher cryptanalysis, 
and achieve the highest practical level of assurance that no shortcut attack will be found. 
Serpent has twice as many rounds as are necessary, to block all currently known shortcut attacks. 
Despite these exacting design constraints, Serpent is faster than DES.


IDEA: 
IDEA stands for International Data Encryption Algorithm and was designed by Xuejia Lai and James Massey. 
It operates on 64bit blocks and uses a key of 128 bits. 
Refer to Ascom-Tech AG for any copyright, patent or license issues for the IDEA algorithm. 
IDEA is provided only as an extra module to libmcrypt.

ENIGMA (UNIX crypt): A one-rotor machine designed along the lines of Enigma but considerable trivialized. 
Very easy to break for a skilled cryptanalyst. I suggest against using it. Added just for completeness.


GOST: 
A former soviet union's algorithm. An acronym for "Gosudarstvennyi Standard" or Government Standard. 
It uses a 256 bit key and a 64 bit block. 
The S-boxes used here are described in the Applied Cryptography book by Bruce Schneier. 
They were used in an application for the Central Bank of the Russian Federation. 
Some quotes from gost.c:
The standard is written by A. Zabotin (project leader), G.P. Glazkov, and V.B. Isaeva. 
It was accepted and introduced into use by the action of the State Standards Committee of the USSR 
on 2 June 1989 as No. 1409. 
It was to be reviewed in 1993, but whether anyone wishes to take on this obligation from the USSR is questionable. 
This code is based on the 25 November 1993 draft translation by Aleksandr Malchik, 
with Whitfield Diffie, of the Government Standard of the U.S.S.R. 
GOST 28149-89, "Cryptographic Transformation Algorithm", effective 1 July 1990. 
(Whitfield.Diffie@eng.sun.com) Some details have been cleared up by the paper "Soviet Encryption Algorithm" 
by Josef Pieprzyk and Leonid Tombak of the University of Wollongong, New South Wales. (josef/leo@cs.adfa.oz.au)

SAFER: 
SAFER (Secure And Fast Encryption Routine) is a block cipher developed by Prof. 
J.L. Massey at the Swiss Federal Institute of Technology. 
There exist four versions of this algorithm, namely: SAFER K-64 , SAFER K-128 , SAFER SK-64 and SAFER SK-128. 
The numerals 64 and 128 stand for the length of the user-selected key, 'K' stands for the original key schedule 
and 'SK' stands for the strengthened key schedule 
(in which some of the "weaknesses" of the original key schedule have been removed). 
In mcrypt only SAFER SK-64 and SAFER SK-128 are used.

SAFER+: 
SAFER+ was designed by Prof. J.L. Massey, Prof. Gurgen H. Khachatrian and Dr. Melsik K. Kuregian for Cylink. 
SAFER+ is based on the existing SAFER family of ciphers 
and provides for a block size of 128bits and 128, 192 and 256 bits key length.

A short description of the modes supported by libmcrypt:

STREAM: 
The mode used with stream ciphers. 
In this mode the keystream from the cipher is XORed with the plaintext. 
Thus you should NOT ever use the same key.

ECB: 
The Electronic CodeBook mode. 
It is the simplest mode to use with a block cipher. Encrypts each block independently.
It is a block mode so plaintext length should be a multiple of blocksize (n*blocksize).

CBC: 
The Cipher Block Chaining mode. It is better than ECB since the plaintext is XOR'ed with the previous ciphertext.
A random block should be placed as the first block (IV) so the same block or messages always encrypt to something different.
It is a block mode so plaintext length should be a multiple of blocksize (n*blocksize).

CFB: 
The Cipher-Feedback Mode (in 8bit). This is a self-synchronizing stream cipher implemented from a block cipher. 
This is the best mode to use for encrypting strings or streams. This mode requires an IV.

OFB: 
The Output-Feedback Mode (in 8bit). This is a synchronous stream cipher implemented from a block cipher. 
It is intended for use in noisy lines, because corrupted ciphertext blocks do not corrupt the plaintext blocks that follow. 
Insecure (because used in 8bit mode) so it is recommended not to use it. Added just for completeness.

nOFB: 
The Output-Feedback Mode (in nbit). 
n Is the size of the block of the algorithm. 
This is a synchronous stream cipher implemented from a block cipher. 
It is intended for use in noisy lines, 
because corrupted ciphertext blocks do not corrupt the plaintext blocks that follow. This mode operates in streams.

nCFB: 
The Cipher-Feedback Mode (in nbit). 
n Is the size of the block of the algorithm. 
This is a self synchronizing stream cipher implemented from a block cipher. This mode operates in streams.

CTR: 
The Counter Mode. This is a stream cipher implemented from a block cipher. 
This mode uses the cipher to encrypt a set of input blocks, called counters, 
to produce blocks that will be XORed with the plaintext. 
In libmcrypt the counter is the given IV which is incremented at each step. 
This mode operates in streams.

Error Recovery in these modes: If bytes are removed or lost from the file or stream in ECB, CTR, CBC and OFB modes, 
are impossible to recover, although CFB and nCFB modes will recover. 
If some bytes are altered then a full block of plaintext is affected in ECB, nOFB and CTR modes, 
two blocks in CBC, nCFB and CFB modes, but only the corresponding byte in OFB mode.

Encryption can be done as follows:

A call to function: 
MCRYPT mcrypt_module_open( char *algorithm, char* algorithm_directory, char* mode, char* mode_directory);

This function associates the algorithm and the mode specified. 
The name of the algorithm is specified in algorithm, eg "twofish", 
and the algorithm_directory is the directory where the algorithm is (it may be null if it is the default). 
The same applies for the mode. The library is closed by calling mcrypt_module_close(), 
but you should not call that function if mcrypt_generic_end() is called before. 
Normally it returns an encryption descriptor, or MCRYPT_FAILED on error.

A call to function: 
int mcrypt_generic_init( MCRYPT td, void *key, int lenofkey, void *IV);

This function initializes all buffers 
for the specified thread The maximum value of lenofkey should be the one obtained by calling mcrypt_get_key_size() 
and every value smaller than this is legal. Note that Lenofkey should be specified in bytes not bits. 
The IV should normally have the size of the algorithms block size, 
but you must obtain the size by calling mcrypt_get_iv_size(). 
IV is ignored in ECB. IV MUST exist in CFB, CBC, STREAM, nOFB and OFB modes. 
It needs to be random and unique (but not secret). 
The same IV must be used for encryption/decryption. 
After calling this function you can use the descriptor for encryption or decryption (not both). 
Returns a negative value on error.

To encrypt now call:

int mcrypt_generic( MCRYPT td, void *plaintext, int len);

This is the main encryption function. 
td is the encryption descriptor returned by mcrypt_generic_init(). 
Plaintext is the plaintext you wish to encrypt and len should be the length (in bytes) of the plaintext 
and it should be k*algorithms_block_size if used in a mode which operated in blocks (cbc, ecb, nofb), 
or whatever when used in cfb or ofb which operate in streams. 
The plaintext is replaced by the ciphertext. Returns 0 on success.

To decrypt you can call:

int mdecrypt_generic( MCRYPT td, void *ciphertext, int len);

The decryption function. It is almost the same with mcrypt_generic. Returns 0 on success.

When you're finished you should call:

int mcrypt_generic_end( MCRYPT td);

This function terminates encryption specified by the encryption descriptor (td). 
Actually it clears all buffers, and closes all the modules used. 
Returns a negative value on error. This function is deprecated. 
Use mcrypt_generic_deinit() and mcrypt_module_close() instead.

int mcrypt_generic_deinit( MCRYPT td);

This function terminates encryption specified by the encryption descriptor (td). 
Actually it clears all buffers. 
The difference with mcrypt_generic_end() is that this function does not close the modules used. 
Thus you should use mcrypt_module_close(). 
Using this function you gain in speed if you use the same modules for several encryptions. 
Returns a negative value on error.

int mcrypt_module_close( MCRYPT td);

This function closes the modules used by the descriptor td.

These are some extra functions that operate on modules that have been opened: 
These functions have the prefix mcrypt_enc_*.

int mcrypt_enc_set_state(MCRYPT td, void *state, int size); 
This function sets the state of the algorithm. 
Can be used only with block algorithms and certain modes like CBC, CFB etc. 
It is usefully if you want to restart or start a different encryption quickly. 
Returns zero on success. 
The state is the output of mcrypt_enc_get_state().

int mcrypt_enc_get_state(MCRYPT td, void *state, int *size); 
This function returns the state of the algorithm. 
Can be used only certain modes and algorithms. 
The size will hold the size of the state and the state must have enough bytes to hold it. 
Returns zero on success.

int mcrypt_enc_self_test( MCRYPT td);

This function runs the self test on the algorithm specified by the descriptor td. 
If the self test succeeds it returns zero.

int mcrypt_enc_is_block_algorithm_mode( MCRYPT td);

Returns 1 if the mode is for use with block algorithms, 
otherwise it returns 0. (eg. 0 for stream, and 1 for cbc, cfb, ofb)

int mcrypt_enc_is_block_algorithm( MCRYPT td);

Returns 1 if the algorithm is a block algorithm or 0 if it is a stream algorithm.

int mcrypt_enc_is_block_mode( MCRYPT td);

Returns 1 if the mode outputs blocks of bytes or 0 if it outputs bytes. 
(eg. 1 for cbc and ecb, and 0 for cfb and stream)

int mcrypt_enc_get_block_size( MCRYPT td);

Returns the block size of the algorithm specified by the encryption descriptor in bytes. 
The algorithm MUST be opened using mcrypt_module_open().

int mcrypt_enc_get_key_size( MCRYPT td);

Returns the maximum supported key size of the algorithm specified by the encryption descriptor in bytes. 
The algorithm MUST be opened using mcrypt_module_open().

int* mcrypt_enc_get_supported_key_sizes( MCRYPT td, int* sizes)

Returns the key sizes supported by the algorithm specified by the encryption descriptor. 
If sizes is zero and returns NULL then all key sizes between 1 and mcrypt_get_key_size() are supported by the algorithm. 
If it is 1 then only the mcrypt_get_key_size() size is supported and sizes[0] is equal to it. 
If it is greater than 1 then that number specifies the number of elements in sizes 
which are the key sizes that the algorithm supports. 
The returned value is allocated with malloc, so you should not forget to free it.

int mcrypt_enc_get_iv_size( MCRYPT td);

Returns size of the IV of the algorithm specified by the encryption descriptor in bytes. 
The algorithm MUST be opened using mcrypt_module_open(). 
If it is '0' then the IV is ignored in that algorithm. 
IV is used in CBC, CFB, OFB modes, and in some algorithms in STREAM mode.

int mcrypt_enc_mode_has_iv( MCRYPT td);

Returns 1 if the mode needs an IV, 0 otherwise. 
Some 'stream' algorithms may need an IV even if the mode itself does not need an IV.

char* mcrypt_enc_get_algorithms_name( MCRYPT td);

Returns a character array containing the name of the algorithm. 
The returned value is allocated with malloc, so you should not forget to free it.

char* mcrypt_enc_get_modes_name( MCRYPT td);

Returns a character array containing the name of the mode. 
The returned value is allocated with malloc, so you should not forget to free it.

These are some extra functions that operate on modules: 
These functions have the prefix mcrypt_module_*.

int mcrypt_module_self_test (char* algorithm, char* directory);

This function runs the self test on the specified algorithm. 
If the self test succeeds it returns zero.

int mcrypt_module_is_block_algorithm_mode( char* algorithm, char* directory);

Returns 1 if the mode is for use with block algorithms, 
otherwise it returns 0. (eg. 0 for stream, and 1 for cbc, cfb, ofb)

int mcrypt_module_is_block_algorithm( char* mode, char* directory);

Returns 1 if the algorithm is a block algorithm or 0 if it is a stream algorithm.

int mcrypt_module_is_block_mode( char* mode, char* directory);

Returns 1 if the mode outputs blocks of bytes or 0 if it outputs bytes. 
(eg. 1 for cbc and ecb, and 0 for cfb and stream)

int mcrypt_module_get_algo_block_size( char* algorithm, char* directory);

Returns the block size of the algorithm.

int mcrypt_module_get_algo_key_size( char* algorithm, char* directory);

Returns the maximum supported key size of the algorithm.

int* mcrypt_module_get_algo_supported_key_sizes( char* algorithm, char* directory, int* sizes);

Returns the key sizes supported by the algorithm. 
If sizes is zero and returns NULL then all key sizes between 1 and mcrypt_get_key_size() are supported by the algorithm. 
If it is 1 then only the mcrypt_get_key_size() size is supported and sizes[0] is equal to it. 
If it is greater than 1 then that number specifies the number of elements in sizes 
which are the key sizes that the algorithm supports. 
This function differs to mcrypt_enc_get_supported_key_sizes(), 
because the return value here is allocated (not static), thus it should be freed.

char** mcrypt_list_algorithms ( char* libdir, int* size);

Returns a pointer to a character array containing all the mcrypt algorithms located in the libdir, 
or if it is NULL, in the default directory. 
The size is the number of the character arrays. 
The arrays are allocated internally and should be freed by using mcrypt_free_p().

char** mcrypt_list_modes ( char* libdir, int *size);

Returns a pointer to a character array containing all the mcrypt modes located in the libdir, 
or if it is NULL, in the default directory. 
The size is the number of the character arrays. 
The arrays should be freed by using mcrypt_free_p().

void mcrypt_free_p (char **p, int size);

Frees the pointer to array returned by previous functions.

void mcrypt_free (void *ptr);

Frees the memory used by the pointer.

void mcrypt_perror(int err);

This function prints a human readable description of the error 'err' in the stderr. 
The err should be a value returned by mcrypt_generic_init().

const char* mcrypt_strerror(int err);

This function returns a human readable description of the error 'err'. 
The err should be a value returned by mcrypt_generic_init().

int mcrypt_mutex_register ( void (*mutex_lock)(void) , void (*mutex_unlock)(void) );

This function is only used in multithreaded application and only if compiled with dynamic module loading support. 
This is actually used internally in libltdl. Except for the dynamic module loading libmcrypt is thread safe.

Some example programs follow here. 
Compile as "cc prog.c -lmcrypt", or "cc prog.c -lmcrypt -lltdl" depending on your installation. 
Libltdl is used for opening dynamic libraries (modules).

/*First example: Encrypts stdin to stdout using TWOFISH with 128 bit key and CFB */
#include <mcrypt.h>
#include <stdio.h>
#include <stdlib.h>
/* #include <mhash.h> */
main() {
	MCRYPT td;
	int i;
	char *key;
	char password[20];
	char block_buffer;
	char *IV;
	int keysize=16; /* 128 bits */
	key=calloc(1, keysize);
	strcpy(password, "A_large_key");
	/* Generate the key using the password */
	/*  mhash_keygen( KEYGEN_MCRYPT, MHASH_MD5, key, keysize, NULL, 0, password,
	    strlen(password));
	    */
	memmove( key, password, strlen(password));
	td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
	if (td==MCRYPT_FAILED) {
		return 1;
	}
	IV = malloc(mcrypt_enc_get_iv_size(td));
	/* Put random data in IV. Note these are not real random data,
	 * consider using /dev/random or /dev/urandom.
	 */
	/*  srand(time(0)); */
	for (i=0; i< mcrypt_enc_get_iv_size( td); i++) {
		IV[i]=rand();
	}
	i=mcrypt_generic_init( td, key, keysize, IV);
	if (i<0) {
		mcrypt_perror(i);
		return 1;
	}
	/* Encryption in CFB is performed in bytes */
	while ( fread (&block_buffer, 1, 1, stdin) == 1 ) {
		mcrypt_generic (td, &block_buffer, 1);
		/* Comment above and uncomment this to decrypt */
		/*    mdecrypt_generic (td, &block_buffer, 1);  */
		fwrite ( &block_buffer, 1, 1, stdout);
	}
	/* Deinit the encryption thread, and unload the module */
	mcrypt_generic_end(td);
	return 0;
}
/* Second Example: encrypts using CBC and SAFER+ with 192 bits key */
#include <mcrypt.h>
#include <stdio.h>
#include <stdlib.h>
main() {
	MCRYPT td;
	int i;
	char *key; /* created using mcrypt_gen_key */
	char *block_buffer;
	char *IV;
	int blocksize;
	int keysize = 24; /* 192 bits == 24 bytes */
	key = calloc(1, keysize);
	strcpy(key, "A_large_and_random_key");
	td = mcrypt_module_open("saferplus", NULL, "cbc", NULL);
	blocksize = mcrypt_enc_get_block_size(td);
	block_buffer = malloc(blocksize);
	/* but unfortunately this does not fill all the key so the rest bytes are
	 * padded with zeros. Try to use large keys or convert them with mcrypt_gen_key().
	 */
	IV=malloc(mcrypt_enc_get_iv_size(td));
	/* Put random data in IV. Note these are not real random data,
	 * consider using /dev/random or /dev/urandom.
	 */
	/* srand(time(0)); */
	for (i=0; i < mcrypt_enc_get_iv_size(td); i++) {
		IV[i]=rand();
	}
	mcrypt_generic_init( td, key, keysize, IV);
	/* Encryption in CBC is performed in blocks */
	while ( fread (block_buffer, 1, blocksize, stdin) == blocksize ) {
		mcrypt_generic (td, block_buffer, blocksize);
		/*      mdecrypt_generic (td, block_buffer, blocksize); */
		fwrite ( block_buffer, 1, blocksize, stdout);
	}
	/* deinitialize the encryption thread */
	mcrypt_generic_deinit (td);
	/* Unload the loaded module */
	mcrypt_module_close(td);
	return 0;
}
The library does not install any signal handler.
Questions about libmcrypt should be sent to:

mcrypt-dev@lists.hellug.gr
or, if this fails, to the author addresses given below. The mcrypt home page is:
http://mcrypt.hellug.gr
Authors
Version 2.4 Copyright Â© 1998-1999 Nikos Mavroyanopoulos (nmav@hellug.gr).
Thanks to all the people who reported problems and suggested various improvements for mcrypt; who are too numerous to cite here.




#endif

//================================================


//-----------------------------------------------------------


//========================================================
#if 0
mcrypt(1) - Linux man page
Name
mcrypt, mdecrypt - encrypt or decrypt files
Synopsis
mcrypt [ -dLFubhvrzp ] [-a algorithm] [-c config_file] [-m mode] [-s keysize] [-o keymode] [-k key1 key2 ...]
[-f keyfile] [ filename ... ]

mdecrypt [ -LFusbhvzp ] [-a algorithm] [-c config_file] [-m mode] [-s keysize] [-o keymode] [-k key1 key2 ...] 
[-f keyfile] [ filename ... ]



Description
Mcrypt is a simple crypting program, a replacement for the old unix crypt(1). 
When encrypting or decrypting a file, a new file is created with the extension .nc and mode 0600. 
The new file keeps the modification date of the original. 
The original file may be deleted by specifying the -u parameter. 
If no files are specified, the standard input is encrypted to the standard output.

Mcrypt uses all the symmetric algorithms included in libmcrypt.

Hints
By default, mcrypt , when one of these algorithms is specified, prompts something like:
Enter passphrase: ... You should then enter a passphrase long enough (512 characters is the maximum length). 
Now in order to encrypt the file, 
The passphrase is transformed using the specified (or the default) key generation algorithm, 
and a random salt. The produced value is then used as the key, which is fed to the algorithm.

Algorithm Vulnerability: Most algorithms today are designed to resist in specific attacks. 
None of them is proved not to be vulnerable to some kind of attack not as yet known.

Compression: 
By compressing your data before encryption you gain both in efficiency (faster encryption) 
and safety of your data (language redundancy is removed). 
A drawback is that most compression programs will add specific headers in the compressed file, 
thus making known plaintext attacks easier. 
Compression after encryption is useless and may result to compressed files with longer size than the original.

Error Recovery: 
There is some error recovery in mcrypt. 
If bytes are removed or lost from the file or stream in ECB, CBC and OFB modes, are impossible to recover, 
although CFB mode will recover. 
If some bytes are altered then a full block of plaintext is affected in ECB mode, two blocks in CBC and CFB modes, 
but only the corresponding byte in OFB mode. Mcrypt uses a 32 bit CRC to check for errors in the encrypted files.

Extra security: For the very paranoid, 
if mcrypt is executed with superuser privileges it ensures that no important data (keys etc.) are written to disk, 
as swap etc. Keep in mind that mcrypt was not designed to be a setuid program, so you shouldn't make it one.

Do not rely on the fact that an algorithm has a large key size, 
try to use long passphrases and try to make them unpredictable.

All the block algorithms above support these modes of encryption:

ECB: 
The Electronic CodeBook mode. 
It is the simplest mode to use with a block cipher. 
Encrypts each block independently.

CBC: 
The Cipher Block Chaining mode. 
It is better than ECB since the plaintext is XOR'ed with the previous ciphertext. 
A random block is placed as the first block so the same block or messages always encrypt to something different. 
(This is the default mode)

CFB: 
The Cipher-Feedback Mode (in 8bit). 
This is a self-synchronizing stream cipher implemented from a block cipher.

OFB: 
The Output-Feedback Mode (in 8bit). 
This is a synchronous stream cipher implemented from a block cipher. 
It is intended for use in noisy lines, 
because corrupted ciphertext blocks do not corrupt the plaintext blocks that follow. 
Insecure when used to encrypt large amounts of data, so I recommend against using it.

nOFB: 
The Output-Feedback Mode (in nbit). 
n Is the size of the block of the algorithm. 
This is a synchronous stream cipher implemented from a block cipher. 
It is intended for use in noisy lines, 
because corrupted ciphertext blocks do not corrupt the plaintext blocks that follow.

Encrypted files can be restored to their original form using mcrypt -d or mdecrypt

mdecrypt takes a list of files on its command line 
and creates a new file for each file whose name ends with .nc by removing the ".nc" 
or by adding ".dc" to the end of the file name if .nc is not in the encrypted file's name.

Options
-F --force

Force output on standard output or input from stdin if that is a terminal. 
By default mcrypt will not output encrypted data to terminal, nor read encrypted data from it.

-z --gzip
Use gzip (if it exists in your system) to compress files before encryption. 
If specified at decryption time it will decompress these files.

-p --bzip2
Use bzip2 (if it exists in your system) to compress files before encryption. 
If specified at decryption time it will decompress these files.

--openpgp-z INT
This option will enable compression in OpenPGP (RFC2440) encrypted files.

-d --decrypt
Decrypt.

--help
Display a help screen and quit.

-v --version
Version. Display the version number and quit.

-L --license
Display the mcrypt's license and quit.

-o --keymode MODE
MODE may be one of the keymodes listed by the --list-keymodes parameter. 
It actually is the conversion to the key before it is fed to the algorithm. 
It is recommended to leave it as is, if you do not know what it is. 
However if you still want to use this option, 
you might want to use the 'hex' mode which allows you to specify the key in hex (and no conversion will be applied).

-h --hash HASH_ALGORITHM
HASH_ALGORITHM may be one of the algorithms listed by the --list-hash parameter. 
This is the digest that will be appended to the file to be encrypted, in order to detect file corruption. 
The default is the CRC32 checksum.

-s --keysize SIZE
SIZE is the algorithm's key size in bytes (not the size of the passphrase). 
It defaults to the maximum key supported by the algorithm. 
The maximum key sizes of the algorithms may be obtained by the --list parameter. 
It is safe not to touch this.

-g --openpgp
This option will make mcrypt to use the OpenPGP (RFC2440) file format for encrypted files. 
This will make files encrypted by mcrypt accessible from any OpenPGP compliant application.

-b --bare
No important information like the algorithm, mode, 
the bit mode and the crc32 of the original file are written in the encrypted file. 
The security lies on the algorithm not on obscurity so this is NOT the default. 
This flag must also be specified when decrypting a bare encrypted file. 
When the bare flag is specified decryption and encryption are faster. 
This may be useful when using mcrypt to encrypt a link or something like that.

--flush
Flushes the output (ciphertext or plaintext) immediately. Useful if mcrypt is used with pipes.

--time
Prints some timing information (encryption speed etc.)

--nodelete
When this option is specified mcrypt does not delete the output file, 
even if decryption failed. This is useful if you want to decrypt a corrupted file.

-q --quiet
Suppress some not critical warnings.

-u --unlink
Unlink (delete) the input file if the whole process of encryption/decryption succeeds. 
This is not the default in order to use an external program to remove sensitive data.

--list
Lists all the algorithms current supported.

--list-keymodes
Lists all the key modes current supported.

--list-hash
Lists all the hash algorithms current supported.

-r --random
Use /dev/(s)random instead of /dev/urandom. 
This may need some key input or mouse move to proceed. 
If your system does not support /dev/random or /dev/urandom, a random gatherer will be used.

-k --key KEY1 KEY2 ...
Enter the keyword(s) via the command line. 
The KEY(s) is/are then used as keyword instead of prompting for them. 
Keep in mind that someone may see the command you are executing and so your keyword(s).

-c --config FILE
Use the specified configuration file. 
The default is .mcryptrc in your home directory. 
The format of the configuration file is the same as the parameters. 
An example file is: algorithm safer+ mode cbc key a_very_secret_one

-f --keyfile FILE
Enter the keyword(s) via a file. One keyword is read per line. 
The first keyword read is used for the first file, the second for the second file etc. 
If the keywords are less than the files then the last keyword is used for the remaining. 
A limitation is that you cannot use the NULL (\0) and the Newline (\n) character in the key. 
A solution to this problem is to specify the keyword in hex mode.

-m --mode MODE
Mode of encryption and decryption. 
These modes are currently supported: ECB, CFB, OFB, nOFB, CBC and STREAM. 
CBC is the default. Unless the bare flag is specified there is no need to specify these modes for decryption. 
For stream algorithms (like WAKE) mode should be STREAM.

-a --algorithm ALGORITHM
The algorithm used to encrypt and decrypt. 
Unless the bare flag is specified there is no need to specify these for decryption.
The algorithms currently supported are shown with the --list parameter.

Examples
For mcrypt to be compatible with the solaris des(1), 
the following parameters are needed: "mcrypt -a des --keymode pkdes --bare --noiv filename".

For mcrypt to be compatible with the unix crypt(1), 
the following parameters are needed: "mcrypt -a enigma --keymode scrypt --bare filename".

To encrypt a file using a stream algorithm (eg. Arcfour), 
the following parameters are needed: "mcrypt -a arcfour --mode stream filename".

Environment
Mcrypt uses the following environment variables:

MCRYPT_KEY: to specify the key

MCRYPT_ALGO: to specify the algorithm

MCRYPT_MODE: to specify the algorithm's mode

MCRYPT_KEY_MODE: to specify the key mode

You can use these instead of using the command line (which is insecure), 
but note that only one key should be used in MCRYPT_KEY.

See Also
crypt(1), des(1) mcrypt(3)

Diagnostics
Exit status is normally 0; if an error occurs, exit status is something other than 0.

Usage: 
mcrypt [-dLFubhvrzp] [-f keyfile] [-k key1 key2 ...] [-m mode] 
[-o keymode] [-a algorithm] [-c config_file] [filename ...]

Authors
Version 2.6.0 Copyright Â© 1998,1999,2000,2001,2002 Nikos Mavroyanopoulos (nmav@gnutls.org).

Thanks to all the people who reported problems and suggested various improvements for mcrypt; 
who are too numerous to cite here.
#endif
//========================================================









