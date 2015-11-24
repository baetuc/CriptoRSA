#pragma once
#include<NTL/ZZ.h>
#include<fstream>
#include<iostream>
#include"Declarations.h"
#include <stdlib.h>
#include <windows.h>

using namespace std;
using namespace NTL;

class Decryption {
private: 
	static void initializeCryptotextFromFile();
	static void initializePrivateKeyFromFile();
	static void decrypt();
	static void decryptUsingCRT();
	static void writeDecryptedMessageInFile();

public:
	static void decryptText(bool ultraMode);
};

void Decryption::initializeCryptotextFromFile() {
	ifstream criptotextReader(ENCRYPTED_TEXT);
	criptotextReader >> cryptotext;
	criptotextReader.close();
}

void Decryption::initializePrivateKeyFromFile() {
	ifstream privateKeyReader(PRIVATE_KEY);
	privateKeyReader >> compositeNumber;
	privateKeyReader >> privateKey;
	privateKeyReader >> firstPrimeNumber;
	privateKeyReader >> secondPrimeNumber;
	privateKeyReader.close();
}

void Decryption::decrypt() {
	// We measure the time spent decrypting.
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	double interval;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	decryptedText = PowerMod(cryptotext, privateKey, compositeNumber);

	QueryPerformanceCounter(&end);
	interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	cout << "Time spent using basic decryption: " << interval * 1000 << "milliseconds.\n";
}

void Decryption::decryptUsingCRT() {

	// We measure the time spent decrypting.
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	double interval;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	// We compute c^d mod n by using CRT:
	// First we compute	d mod (p-1), d mod (q - 1) and p ^ (-1) mod q.

	ZZ firstModularExpression = privateKey % (firstPrimeNumber - 1);
	ZZ secondModularExpression = privateKey % (secondPrimeNumber - 1);
	ZZ firstInvModularSecond = InvMod(firstPrimeNumber % secondPrimeNumber, secondPrimeNumber);

	ZZ x1, x2;
	// We compute x1 = (d mod p) ^ n_1  mod p and x2 = (d mod q) ^ n_2 mod q
	x1 = PowerMod(cryptotext % firstPrimeNumber, firstModularExpression, firstPrimeNumber);
	x2 = PowerMod(cryptotext % secondPrimeNumber, secondModularExpression, secondPrimeNumber);

	// The result of c ^ d mod n is: x1 + p((x2 - x1)(p ^ (-1) mod q) mod q).

	decryptedText = x1 + firstPrimeNumber * (((x2 - x1) * firstInvModularSecond) % secondPrimeNumber);


	QueryPerformanceCounter(&end);
	interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	cout << "Time spent using CRT decryption: " << interval * 1000 << "milliseconds.\n";
}

void Decryption::writeDecryptedMessageInFile() {
	ofstream writeResult(DECRYPTED_TEXT, fstream::trunc | fstream::out);
	writeResult << decryptedText;
	writeResult.close();
}

void Decryption::decryptText(bool ultraMode) {
	initializeCryptotextFromFile();
	initializePrivateKeyFromFile();
	if (ultraMode) {
		decryptUsingCRT();
	}
	else {
		decrypt();
	}
	writeDecryptedMessageInFile();
}
