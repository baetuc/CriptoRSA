#pragma once
#include<NTL/ZZ.h>

#define PRIMES_LENGTH 512
#define PUBLIC_KEY "publicKey.txt"
#define PRIVATE_KEY "privateKey.txt"
#define MESSAGE "message.txt"
#define ENCRYPTED_TEXT "cryptotext.txt"
#define DECRYPTED_TEXT "decryptedText.txt"

using namespace NTL;

ZZ compositeNumber, firstPrimeNumber, secondPrimeNumber;
ZZ phiCompositeNumber;
ZZ privateKey, partOfPublicKey;
ZZ cryptotext;
ZZ decryptedText;