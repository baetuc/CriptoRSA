// CriptoRSA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"Decryption.h"
#include"Encryption.h"
#include"RandomGenerator.h"
#include"RandomGeneratorForWiener.h"
#include"WienerAttack.h"
#include<iostream>

int main() {
	int response = 4;
	bool validOption = true;
	while (response) {
		cout << "Please enter 1 for being on the good side, 2 if planning an attack or 0 for exiting the program: ";
		cin >> response;
		switch (response) {

		case 0:
			return 0;

		case 2:
			cout << "Welcome to the evil side. ";
			cout << "We will first generate proper keys for the attack, then perform the attack. ";
			cout << "Please be patient.\n";
			WienerAttack::prepareAttack();
			cout << "Keys were succesfully generated and text encrypted.\n";
			WienerAttack::attack();
			cout << "RSA was sucessfully attacked. You can verify it in \"decryptedText.txt\" file.\n\n";
			break;
	
		case 1:
			cout << "Thank you for being on the right side. ";
			cout << "Please enter 1 for key generation, 2 for message encryption or 3 for cryptotext decryption: ";
			cin >> response;
			switch (response) {

			case 1:
				RandomGenerator::generateData();
				cout << "Keys were generated. Check it out in the specific file.\n\n";
				break;
			case 2:
				Encryption::encryptMessage();
				cout << "Message was encrypted. Check it out in \"cryptotext.txt\" file.\n\n";
				break;
			case 3:
				cout << "Please enter 1 for basic decryption or 2 for decryption using CRT: ";
				cin >> response;
				switch (response) {
				case 1:
					Decryption::decryptText(false);
					break;
				case 2:
					Decryption::decryptText(true);
					break;
				default:
					validOption = false;
					cout << "Please enter a valid option.\n\n";
				}
				response = 3;
				if (validOption) {
					cout << "Message was decrypted. Check it out in \"decryptedText.txt\" file.\n\n";
				}
				else {
					cout << "Please enter a valid option.\n\n";
				}
				break;
			default:
				cout << "Please enter a valid option.\n\n";
			}
			break;
		default: 
			cout << "Please enter a valid option.\n\n";
		}
	} 
	return 0;
}



