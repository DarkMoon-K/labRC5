//Лабораторная работа 2. RC5. Сабитова Кристина, ИТ-41.
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
typedef unsigned long int WORD; 
const int w = 32; //половина длины блока(в битах) 
const int r = 12;  //число раундов 
const int b = 16; // длина ключа(в байтах)
const int c = 4; // размер массива слов
const int t = 26; // размер таблицы S = 2*(r+l)
WORD S[t]; // таблица расширенных ключей */
WORD P = 0xb7e15163, Q = 0x9e3779b9; // магические константы 
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1))))) 
using namespace std;

void encryption(WORD* encr, WORD* decr) //функция шифрования, encr-входное сообщение, decr-зашифрованное
{
	WORD i, A = encr[0] + S[0], B = encr[1] + S[1];
	for (i = 1; i <= r; i++)
	{
		A = ROTL(A^B, B) + S[2 * i];
		B = ROTL(B^A, A)+S[2*i+1] ;
	}
	decr[0] = A; decr[1] = B;
}

void decryption(WORD* decr, WORD* encr) //функция дешифрования
{
	WORD i, B = decr[1], A = decr[0];
	for (i = r; i > 0; i--)
	{
		B = ROTR(B - S[2 * i + 1], A)^A;
		A = ROTR(A - S[2*i], B)^B;
	}
	encr[1] = B - S[1]; encr[0] = A - S[0];
}

void secretKey(unsigned char* K) 
{
	WORD i, j, k, u = w / 8, A, B, L[c];
	/* Инициализация L, затем S, затем смешиваем ключ в S */
	for (i = b - 1, L[c - 1] = 0; i != -1; i--) L[i / u] = (L[i / u] << 8) + K[i];
	for (S[0] = P, i = 1; i < t; i++) S[i] = S[i - 1] + Q;
	for (A = B = i = j = k = 0; k < 3 * t; k++, i = (i + 1) % t, j = (j + 1) % c) 
	{
		A = S[i] = ROTL(S[i] + (A + B), 3);
		B = L[j] = ROTL(L[j] + (A + B), (A + B));
	}
}

void main()
{
	setlocale(LC_ALL, "Rus");
	WORD i, j, encr2[2], decr[2] = { 0,0 };
	WORD encr1[2] = { 0xEEDBA521,0x6D8F4B15};
	unsigned char key[b] = { 0x91,0x5f,0x46,0x19,0xbe,0x41,0xb2,0x51,0x63,0x55,0xa5,0x01,0x10,0xa9,0xce,0x91 }; 
	//WORD encr1[2] = { 0x00000000,0x00000000 };
	//unsigned char key[b]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	secretKey(key);
	encryption(encr1, decr);
	decryption(decr, encr2);
		cout << "key = ";
		for (j = 0; j < b; j++) 
		{
			printf("%.2x ", key[j]); 
		}
			cout << endl;
			cout << "Входное сообщение: " << hex << encr1[0]<<" "<< encr1[1] << endl;
			cout << "Зашифрованное сообщение:  " << hex << decr[0] << " " << decr[1] << endl;
	
}