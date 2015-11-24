#pragma once
#include"Declarations.h"
#include"NTL/ZZ.h"
#include<fstream>
#include<iostream>
#include"RandomGeneratorForWiener.h"
#include <stdlib.h>
#include <windows.h>

using namespace std;
using namespace NTL;

class WienerAttack {
private:
	static void initializePublicKeyFromFile();
	static void initializeMessageFromFile();
	static void initializeCryptotextFromFile();
	static bool findNextNumberInContinuedFraction(ZZ& result, ZZ& dividend, ZZ& divisor);
	static void findNextAlphaOrBeta(ZZ& currentGreek, ZZ& beforePreviousGreek, ZZ& previousGreek, ZZ& currentQ);
	static bool systemHasIntegerSolutions(ZZ& factorOfPhi);
	static void attackRSA();
	static void encrypt();
	static void decrypt();
	static void writeDecryptedMessageInFile();
	static void writeEncryptedTextInFile();

public:
	static void prepareAttack();
	static void attack();
};

void WienerAttack::initializePublicKeyFromFile() {
	ifstream keyReader(PUBLIC_KEY);
	keyReader >> compositeNumber;
	keyReader >> partOfPublicKey;
	keyReader.close();
}

void WienerAttack::initializeMessageFromFile() {
	ifstream reader(MESSAGE);
	reader >> message;
	reader.close();
}

void WienerAttack::initializeCryptotextFromFile() {
	ifstream criptotextReader(ENCRYPTED_TEXT);
	criptotextReader >> cryptotext;
	criptotextReader.close();
}

// The new number in continued fraction is the result, and the dividend and divisor are updated.
// Function returns true when the fraction converges to the initial (rational) number, i.e. when 
// the new divisor will be 0.
bool WienerAttack::findNextNumberInContinuedFraction(ZZ& result, ZZ& dividend, ZZ& divisor) {
	// q_n = dividend / divisor, dividend = divizor, divizor = dividend % divizor;

	result = dividend / divisor;
	ZZ remainder = dividend % divisor;
	dividend = divisor;
	divisor = remainder;
	if (remainder != 0) {
		return true;
	}
	return false;
}

// G_n = q_n * G_(n-1) + G_(n-2), where G in (alpha, beta).
void WienerAttack::findNextAlphaOrBeta(ZZ& currentGreek, ZZ& beforePreviousGreek, ZZ& previousGreek, ZZ& currentQ){
	currentGreek = currentQ * previousGreek + beforePreviousGreek;
	// update previousAlpha and beforePreviousAlpha for next iteration
	beforePreviousGreek = previousGreek;
	previousGreek = currentGreek;
}

// We decide if the system {x * y = n, (x - 1) * (y - 1) = (e * d - 1) / l has integer solutions
bool WienerAttack::systemHasIntegerSolutions(ZZ& factorOfPhi) {
	if (factorOfPhi == 0) {
		return false;
	}
	// If (e * d - 1) / l is not integer, the system can not have integer solutions
	if ((partOfPublicKey * privateKey - 1) % factorOfPhi != 0) {
		return false;
	}
	// We compute sum = x + y and product = x * y from the system
	ZZ sum = compositeNumber + 1 - (partOfPublicKey * privateKey - 1) / factorOfPhi;
	ZZ product = compositeNumber;

	// The solutions of the system will be the solutions of the quadratic equation: X^2 - S * X + P = 0, 
	// where S is the sum and P is the product of the solutions.

	ZZ delta = sum * sum - 4 * product;

	// If delta is not a quadrate number, then system can not have integer solutions 
	// (since their form is: x_(1,2) = (S +- sqrt(delta)) / 2).
	if (SqrRoot(delta) * SqrRoot(delta) != delta) {
		return 0;
	}

	if ((sum + sqr(delta)) %2 == 0) { 
		return true;
	}
	else {
		return false;
	}
}

void WienerAttack::attackRSA() {
	ZZ currentQ, dividend = partOfPublicKey, divisor = compositeNumber;
	ZZ currentAlpha; // current beta will be the private key
	ZZ beforePreviousAlpha(0), previousAlpha(1);
	ZZ beforePreviousBeta(1), previousBeta(0);
	bool foundSolution = false;

	// We are "initializing" alpha_(-1) = 0, alpha_0 = 1, beta_(-1) = 1, beta_0 = 0,
	// so that the formulae for alpha_n and beta_n can be generalized.

	while (findNextNumberInContinuedFraction(currentQ, dividend, divisor) && !foundSolution) {
		findNextAlphaOrBeta(currentAlpha, beforePreviousAlpha, previousAlpha, currentQ);
		findNextAlphaOrBeta(privateKey, beforePreviousBeta, previousBeta, currentQ);
		// We now have current Alpha and Beta. We now search if the system has integer solutions
		foundSolution = systemHasIntegerSolutions(currentAlpha);
	}
}

void WienerAttack::encrypt() {
 	outputtedCriptotext = PowerMod(message, partOfPublicKey, compositeNumber);
}

void WienerAttack::decrypt() {
	decryptedText = PowerMod(cryptotext, privateKey, compositeNumber);
}

void WienerAttack::writeDecryptedMessageInFile() {
	ofstream writeResult(DECRYPTED_TEXT, fstream::trunc | fstream::out);
	writeResult << decryptedText;
	writeResult.close();
}

void WienerAttack::writeEncryptedTextInFile() {
	ofstream writeText(ENCRYPTED_TEXT, fstream::trunc | fstream::out);
	writeText << outputtedCriptotext;
	writeText.close();
}

void WienerAttack::prepareAttack() {
	WienerRandomGenerator::generateData();
	initializePublicKeyFromFile();
	initializeMessageFromFile();
	encrypt();
	writeEncryptedTextInFile();
}

void WienerAttack::attack() {
	// We measure the time spent decrypting.
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	double interval;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	initializePublicKeyFromFile();
	initializeCryptotextFromFile();
	attackRSA();
	decrypt();
	writeDecryptedMessageInFile();

	QueryPerformanceCounter(&end);
	interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	cout << "Time spent using basic decryption: " << interval * 1000 << "milliseconds.\n";
}






