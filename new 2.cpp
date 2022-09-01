#include "stdafx.h"
#include <gmpxx.h>
#include <mpirxx.h>
#include <mpir.h>
#include <mpfr.h>
#include <iostream>
#include <Windows.h>
#include <windows.h>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <fstream>
#include <chrono>
#include <ctime> 
#include <vector>
#include <iomanip>
#include <bitset>
#include <stdio.h>

using namespace std;

int keyGen();
int exit();

mpz_class x;
mpz_class y;
string encryptionKey; 
string decryptionKey; 
string encryptionBinary; 

mpz_class gcd(mpz_class firstNum, mpz_class secNum) 
{
	if (secNum == 0) 
	{
		return firstNum; 
	}
	return gcd(secNum, firstNum % secNum); 
}

mpz_class modPower(mpz_class a, mpz_class b, mpz_class c) 
{
	mpz_class res = 1; 
	a = a % c; 
	while (b > 0) 
	{
		if (b % 2 != 0) 
		{
			res = (res*a) % c; 
		}
		b = b >> 1; 
		a = (a*a) % c;  
	}
	return res; 
}

bool millerTest(mpz_class d, mpz_class n) 
{
	mpz_class a = 2 + rand() % (n - 4);
	mpz_class x = modPower(a, d, n);

	if (x == 1 || x == n - 1) 
	{
		return true; 
	}
	while (d != n - 1) 
	{
		x = (x * x) % n;
		d *= 2;

		if (x == 1)      return false; 
		if (x == n - 1)    return true; 
	}
	return false;
}

bool isPrime(mpz_class n, mpz_class k) 
{
	if (n <= 1 || n == 4)  return false; 
	if (n <= 3) return true;  

	mpz_class d = n - 1; 
	while (d % 2 == 0) 
		d /= 2; 

	for (mpz_class i = 0; i < k; i++) 
		if (!millerTest(d, n)) 
			return false;

	return true;
}

mpz_class ranPrime() 
{
	mpz_class ranNum; 
	gmp_randclass rnum(gmp_randinit_default); 
	Sleep(1000); 
	rnum.seed(time(NULL)); 
	ranNum = rnum.get_z_bits(1025);
	if (ranNum % 2 == 0)
	{
		ranNum = ranNum + 1;
	}
	do
	{
		ranNum = ranNum + 2; 
	} while (isPrime(ranNum, 100) == false);
	return ranNum;
}

mpz_class power(mpz_class base, mpz_class exponent) 
{
	mpz_class answer;
	if (exponent == 1) 
	{
		return base;
	}
	if (exponent == 0) 
	{
		return 1;
	}
	answer = base * base; 
	for (mpz_class t = 0; t < exponent - 2; t++) 
	{
		answer = answer * base;
	}
	return answer; 

mpz_class gcdExtended(mpz_class  a, mpz_class  b, mpz_class  *x, mpz_class  *y)
{
	if (a == 0) 
	{
		*x = 0;
		*y = 1;
		return b;
	}

	mpz_class  x1, y1; 
	mpz_class  gcd = gcdExtended(b%a, a, &x1, &y1); 

	*x = y1 - (b / a) * x1; 
	*y = x1; 

	return gcd; 
}

void txtToBin(string s)  
{
	int n = s.length(); 
	stringstream buffer; 
	for (int i = 0; i <= n; i++) 
	{
		int val = int(s[i]); 
		string bin = ""; 
		while (val > 0) 
		{
			(val % 2) ? bin.push_back('1') :  
				bin.push_back('0');
			val /= 2; 
		}
		reverse(bin.begin(), bin.end()); 
		do
		{
			bin = "0" + bin;
		} while (bin.size() < 8); 
		buffer << bin.c_str(); 
	}
	encryptionBinary = buffer.str().substr(0, buffer.str().size() - 8);
}

string decToBin(mpz_class n)
{
	string r;
	while (n != 0) 
	{
		r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; 
	}
	return r;
}

mpz_class binToDec(string binText) 
{
	mpz_class value = 0; 
	mpz_class indexCounter = 0; 
	for (int i = binText.length() - 1; i >= 0; i--)
	{
		if (binText[i] == '1')
		{
			value += power(2, indexCounter); 
		}
		indexCounter++;
	}
	return value; 
}

int encrypt() 
{
	string eText; 
	mpz_class n; 
	mpz_class e; 
	mpz_class m; 
	mpz_class c; 
	size_t found;
	system("CLS"); 
	cout << "Encryption key: ";
	cin >> encryptionKey; 
	if (encryptionKey.find(",") == string::npos) 
	{
		system("CLS"); 
		cout << "[!] Not a valid format, try again."; 
		Sleep(3000); 
		return encrypt(); 
	}
	found = encryptionKey.find(","); 
	e = encryptionKey.substr(0, found); 
	n = encryptionKey.substr(found + 1);
	cout << "Text to encrypt: ";
	getline(cin >> ws, eText);
	txtToBin(eText); 
	m = binToDec(encryptionBinary); 
	if (m > n) 
	{
		system("CLS"); 
		cout << "[!] RSA can only encrypt messages whose value is under the n value." << endl;
		Sleep(5000);
		return encrypt(); 
	}
	c = modPower(m, e, n); 
	cout << "Encrypted Text: " << c << endl; 
	return exit(); 
}

int decrypt() 
{
	string dText; 
	string dTextb; 
	mpz_class n; 
	mpz_class d; 
	mpz_class m; 
	mpz_class c; 
	size_t found;
	system("CLS"); 
	cout << "Decryption key: "; 
	cin >> decryptionKey;
	if (decryptionKey.find(",") == string::npos) 
	{
		system("CLS"); 
		cout << "[!] Not a valid format, try again."; 
		Sleep(3000); 
		return decrypt(); 
	}
	found = decryptionKey.find(","); 
	d = decryptionKey.substr(0, found);
	n = decryptionKey.substr(found + 1);
	cout << "Ciphertext to decrypt: "; 
	getline(cin >> ws, dText);
	c = dText; 
	m = modPower(c, d, n); 
	dTextb = "0" + decToBin(m); 
	stringstream bOutput(dTextb); 
	string output; 
	while (bOutput.good()) 
	{
		bitset<8> bits; 
		bOutput >> bits; 
		char c = char(bits.to_ulong()); 
		output += c; 
	}
	cout << "Decrypted ciphertext: " << output << endl; 
	return exit(); 
}

int main() 
{
	SetConsoleTitle(L"2048RSA");
	char A;
	char A2;
	char A3;
	char A4;
	system("CLS");
	cout << "[?] Do you wish to (U)se or (M)ake a raw RSA key pair? (U/M): ";
	cin >> A; 
	if (A == 'M' || A == 'm')
	{
		system("CLS"); 
		cout << "[?] Do you wish to generate a raw RSA key pair? (Y/N): "; 
		cin >> A2;
		if (A2 == 'N' || A2 == 'n') 
		{
			cout << "[-] Exiting..." << endl; 
			Sleep(3000); 
			return 0;
		}
		if (A2 != 'N' && A2 != 'n' && A2 != 'Y' && A2 != 'y')
		{
			system("CLS"); 
			cout << "[!] Please use (Y/y/N/n) to answer the question." << endl; 
			cin.clear(); 
			cin.ignore(10000, '\n'); 
			Sleep(3000); 
			return main(); 
		}
		if (A2 == 'Y' || A2 == 'y') 
		{
			keyGen(); 
		}
	}
	if (A == 'U' || A == 'u') 
	{
		system("CLS"); 
		cout << "[?] Do you wish to encrypt/decrypt text? (Y/N): "; 
		cin >> A3; 
		if (A3 == 'N' || A3 == 'n') 
		{
			cout << "[-] Exiting..." << endl; 
			Sleep(3000); 
			return 0; 
		}
		if (A3 != 'N' && A3 != 'n' && A3 != 'Y' && A3 != 'y') 
		{
			system("CLS"); 
			cout << "[!] Please use (Y/y/N/n) to answer the question." << endl;
			cin.clear(); 
			cin.ignore(10000, '\n'); 
			Sleep(3000); 
			return main(); 
		}
		if (A3 == 'Y' || A3 == 'y') 
		{
			system("CLS");
			cout << "[?] Do you want to (E)ncrypt or (D)ecrypt text? (E/D): "; 
			cin >> A4; 
			if (A4 == 'E' || A4 == 'e') 
			{
				encrypt(); 
			}
			if (A4 == 'D' || A4 == 'd')
			{
				decrypt(); 
			}
			if (A4 != 'E' && A4 != 'e' && A4 != 'D' && A4 != 'd') 
			{
				system("CLS"); 
				cout << "[!] Please use (E/e/D/e) to answer the question." << endl; 
				cin.clear(); 
				cin.ignore(10000, '\n');
				Sleep(3000); 
				return main(); 
			}
		}
	}
	if (A != 'M' && A != 'm' && A != 'U' && A != 'u') 
	{
		system("CLS"); 
		cout << "[!] Please use (M/m/U/u) to answer the question." << endl;
		cin.clear();
		cin.ignore(10000, '\n'); 
		Sleep(3000); 
		return main(); 
	}
}

int exit() 
{
	char finalChoice; 
	cout << "[?] Exit? (Y/N): ";
	cin >> finalChoice; 
	if (finalChoice == 'Y' || finalChoice == 'y') 
	{
		exit(0); 
	}
	if (finalChoice == 'N' || finalChoice == 'n') 
	{
		return main(); 
	}
	if (finalChoice != 'N' && finalChoice != 'n' && finalChoice != 'Y' && finalChoice != 'y') 
	{
		cout << "[!] Please use (Y/y/N/n) to answer the question." << endl; 
		cin.clear(); 
		cin.ignore(10000, '\n'); 
		Sleep(3000);
		return exit();
	}
	return 0;
}


int keyGen() 
{
	ofstream key;
	auto end = chrono::system_clock::now();
	time_t end_time = chrono::system_clock::to_time_t(end); 
	mpz_class p;
	mpz_class q;
	mpz_class n;
	mpz_class z;
	mpz_class e;
	mpz_class d;
	mpz_class checkd;
	system("CLS");
	cout << "[-] Generating p..." << endl; 
	p = ranPrime(); 
	cout << "[+] p Generated! " << endl;
	cout << "[-] Generating q..." << endl;
	q = ranPrime();
	cout << "[+] q Generated! " << endl;
	cout << "[-] Computing n..." << endl;
	n = p * q; 
	cout << "[+] n Computed!" << endl;
	cout << "[-] Computing z..." << endl;
	z = (p - 1) * (q - 1); 
	cout << "[+] z Computed!" << endl;
	cout << "[-] Finding e..." << endl;
	if (e % 2 == 0) 
	{
		e = e + 1; 
	}
	do
	{
		e = e + 2; 
	} while (gcd(e, n) != 1 || isPrime(e, 100) == false || gcd(e, z) != 1); 
	cout << "[!] e Found..." << endl;
	cout << "[-] Finding d..." << endl;
	do
	{
		e = ranPrime(); 
		if (e % 2 == 0) 
		{
			e = e + 1; 
		}
		do
		{
			e = e + 2; 
		} while (gcd(e, n) != 1 || isPrime(e, 100) == false || gcd(e, z) != 1);  
		gcdExtended(e, z, &x, &y);
		d = x; 
		checkd = (d * e) % z; 
	} while (checkd != 1);
	cout << "[+] d & e Found!" << endl;
	string timee = ctime(&end_time); 
	timee.erase(std::remove(timee.begin(), timee.end(), '\n'), timee.end());
	do
	{
		size_t clean = timee.find(":"); 
		timee.replace(clean, 1, " "); 
	} while (timee.find(":") != string::npos); 
	string filename = "Key " + timee + ".txt"; 
	key.open(filename); 
	key << "Public Key: " << e << "," << n << endl << "Private Key: " << d << "," << n; 
	key.close(); 
	cout << "[%] public & private key generated & saved!" << endl; 
	p = 0;
	q = 0;
	n = 0;
	z = 0;
	e = 0;
	d = 0;
	Sleep(5000); 
	return main(); 
}

