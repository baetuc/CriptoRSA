#pragma once
#include<NTL/ZZ.h>
#include<fstream>
#include"Declarations.h"

using namespace std;
using namespace NTL;

class RandomGenerator {
private:
	static void generatePrimes();
	static void computeCompositeAndPhi();
	static void computePartOfPublicKey();
	static void computePrivateKey();
	static void writePrivateKeyInFile();
	static void writePublicKeyInFile();

public:
	static void generateData();
};


void RandomGenerator::generatePrimes() {
	firstPrimeNumber = GenPrime_ZZ(1024);
	secondPrimeNumber = GenPrime_ZZ(1024);
	while (secondPrimeNumber == firstPrimeNumber) {
		secondPrimeNumber = GenPrime_ZZ(1024);
	}
}

 void RandomGenerator::computeCompositeAndPhi() {
	compositeNumber = firstPrimeNumber * secondPrimeNumber;
	phiCompositeNumber = (firstPrimeNumber - 1) * (secondPrimeNumber - 1);
}

void RandomGenerator::computePartOfPublicKey() {
	// First we try to see if e = 41 is valid. If not, try with e = 257. If not, try with e = 65537(2 ^ 16 + 1). Then,
	// if all those values were not correct, try every possibility after 65537, with a period of 2 (65539, 65541, ...).

	if (phiCompositeNumber % 41) {
		partOfPublicKey = 41;
	}
	else if (phiCompositeNumber % 257) {
		partOfPublicKey = 257;
	}
	else {
		partOfPublicKey = 65537;
		while (GCD(phiCompositeNumber, partOfPublicKey) != 1) {
			partOfPublicKey += 2;
		}
	}
}

void RandomGenerator::computePrivateKey() {
	privateKey = InvMod(partOfPublicKey, phiCompositeNumber);
}

void RandomGenerator::writePrivateKeyInFile() {
	ofstream out(PRIVATE_KEY, fstream::trunc | fstream::out);
	out << compositeNumber << "\n" << privateKey << '\n';
	out << firstPrimeNumber << '\n' << secondPrimeNumber;
	out.close();
}

void RandomGenerator::writePublicKeyInFile() {
	ofstream out(PUBLIC_KEY, fstream::trunc | fstream::out);
	out << compositeNumber << "\n" << partOfPublicKey;
	out.close();
}

void RandomGenerator::generateData() {
	generatePrimes();
	computeCompositeAndPhi();
	computePartOfPublicKey();
	computePrivateKey();
	writePrivateKeyInFile();
	writePublicKeyInFile();
}