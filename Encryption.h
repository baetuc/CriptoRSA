#pragma once
#include<NTL/ZZ.h>
#include"Declarations.h"
#include<fstream>

ZZ message;
ZZ outputtedCriptotext;

using namespace std;
using namespace NTL;

class Encryption {
private: 
	static void initializePublicKeyFromFile();
	static void initializeMessageFromFile();
	static void encrypt();
	static void writeEncryptedTextInFile();

public:
	static void encryptMessage();
};


void Encryption::initializePublicKeyFromFile() {
	ifstream keyReader(PUBLIC_KEY);
	keyReader >> compositeNumber;
	keyReader >> partOfPublicKey;
	keyReader.close();
}

void Encryption::initializeMessageFromFile() {
	ifstream messageReader(MESSAGE);
	messageReader >> message;
	messageReader.close();
}

void Encryption::encrypt() {
	outputtedCriptotext = PowerMod(message, partOfPublicKey, compositeNumber);
}

void Encryption::writeEncryptedTextInFile() {
	ofstream writeText(ENCRYPTED_TEXT, fstream::trunc | fstream::out);
	writeText << outputtedCriptotext;
	writeText.close();
}

void Encryption::encryptMessage() {
	initializeMessageFromFile();
	initializePublicKeyFromFile();
	encrypt();
	writeEncryptedTextInFile();
}