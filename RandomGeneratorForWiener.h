#pragma once
#include<NTL/ZZ.h>
#include<fstream>
#include"Declarations.h"

using namespace std;
using namespace NTL;

class WienerRandomGenerator {
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

void WienerRandomGenerator::generatePrimes() {
	firstPrimeNumber = GenPrime_ZZ(PRIMES_LENGTH);
	secondPrimeNumber = GenPrime_ZZ(PRIMES_LENGTH);
	while (secondPrimeNumber >= firstPrimeNumber || firstPrimeNumber >= 2 * secondPrimeNumber) {
		secondPrimeNumber = GenPrime_ZZ(PRIMES_LENGTH);
	}
}

void WienerRandomGenerator::computeCompositeAndPhi() {
	compositeNumber = firstPrimeNumber * secondPrimeNumber;
	phiCompositeNumber = (firstPrimeNumber - 1) * (secondPrimeNumber - 1);
}

void WienerRandomGenerator::computePrivateKey() {
	// We choose d < 1/3 * sqrt(sqrt(n))
	privateKey = RandomBnd(SqrRoot(SqrRoot(compositeNumber)) / 3);
	while ( GCD(phiCompositeNumber, privateKey) > 1) {
		privateKey = RandomBnd(SqrRoot(SqrRoot(compositeNumber)) / 3);
	}
}

void WienerRandomGenerator::computePartOfPublicKey() {
	partOfPublicKey = InvMod(privateKey%phiCompositeNumber, phiCompositeNumber);
}

void WienerRandomGenerator::writePrivateKeyInFile() {
	ofstream out(PRIVATE_KEY, fstream::trunc | fstream::out);
	out << compositeNumber << "\n" << privateKey << '\n';
	out << firstPrimeNumber << '\n' << secondPrimeNumber;
	out.close();
}

void WienerRandomGenerator::writePublicKeyInFile() {
	ofstream out(PUBLIC_KEY, fstream::trunc | fstream::out);
	out << compositeNumber << "\n" << partOfPublicKey;
	out.close();
}

void WienerRandomGenerator::generateData() {
	generatePrimes();
	computeCompositeAndPhi();
	computePrivateKey();
	computePartOfPublicKey();
	writePrivateKeyInFile();
	writePublicKeyInFile();
}