

#include "pch.h"
#include <iostream>

#include "BigInt.hpp"//файл с гита для работы с большими чисами
#include "constructors/constructors.hpp"
#include "functions/conversion.hpp"
#include "operators/binary_arithmetic.hpp"
#include "operators/io_stream.hpp"
#include "functions/random.hpp"


	//функции для RSA алгоритма
	BigInt mulmod(BigInt a, BigInt b, BigInt mod);//modular exponentiation
	BigInt modulo(BigInt base, BigInt exponent, BigInt mod);//Miller-Rabin primality test, iteration signifies the accuracy
	bool miler(BigInt p, int iteration);
	BigInt getRandomNum(BigInt from, int iter);
	BigInt getE(BigInt f);

	BigInt gcd(const BigInt &num1, const BigInt &num2) {
		BigInt abs_num1 = abs(num1);
		BigInt abs_num2 = abs(num2);

		// base cases:
		if (abs_num2 == 0)
			return abs_num1;    // gcd(a, 0) = |a|
		if (abs_num1 == 0)
			return abs_num2;    // gcd(0, a) = |a|

		BigInt remainder = abs_num2;
		while (remainder != 0) {
			remainder = abs_num1 % abs_num2;
			abs_num1 = abs_num2;    // previous remainder
			abs_num2 = remainder;   // current remainder
		}

		return abs_num1;
	}
	
	BigInt mod_inv(BigInt a, BigInt b);
	//функции для возведения в степень по модулю
	std::string sdvig(std::string src);
	bool chheck(std::string l);
	BigInt modpow(BigInt base, BigInt exp, BigInt modulus); 
	std::string ToBinary(BigInt b);

	//шифрация и дешифрация
	void enc_mail(std::string mail, BigInt *&mass, int count, BigInt n, BigInt e);
	std::string dec_mail(BigInt *&mass, int count, BigInt n, BigInt d);

int main()
{
	setlocale(LC_CTYPE, "rus");
	BigInt p, q, n, e, f, d;//поиск случайного N

	p = getRandomNum(big_random(20), 5);//присваиваем найденное N
	q = getRandomNum(big_random(20), 5);
	//std::cout << "p = " << p << " q = " << q << std::endl;

	n = p * q;
	//std::cout << " n = " << n << std::endl;

	f = (p - 1) * (q - 1);
	//std::cout << " f = " << f << std::endl;
	e = getE(f);
	//std::cout << " e = " << e << std::endl;

	d = mod_inv(e, f);

	std::cout << " Закрытый ключ : " << "n = " << n << " d = " << d << std::endl;
	std::cout << " Открытый ключ : " << "n = " << n << " e = " << e << std::endl;

	std::string mail = "Это сообщение отправила я.";
	BigInt *massInt = new BigInt[mail.length()];

	enc_mail(mail, massInt, mail.length(), n, e);	

	std::cout << dec_mail(massInt, mail.length(), n, d) << std::endl;
	std::cout << mulmod(2 , 5, 10) << std::endl;
	

	delete[] massInt;

	return 0;
}

void enc_mail(std::string mail, BigInt *&mass, int count, BigInt n, BigInt e) {//шифрация
	
	for (int i = 0; i < count; i++) {
		mass[i] = modpow(BigInt((int)mail[i]), e, n);
	}

	for(int i = 0; i < count; i++) {
		std::cout << i << ") " << mass[i] << std::endl;
	}
}

std::string dec_mail(BigInt *&mass, int count, BigInt n, BigInt d) {//дешифрация
	std::string mail = "";
	long long num;

	for (int i = 0; i < count; i++) {
		num = modpow(mass[i], d, n).to_long_long();
		mail = mail + (char)num;
	}

	return mail;
}

BigInt mod_inv(BigInt a, BigInt b)
{
	BigInt b0 = b, t, q;
	BigInt x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

BigInt getE(BigInt f) {
	BigInt e = 2;
	
	while (e < f && gcd(e, f) != 1) {
		e = e + 1;
	}
	return e;
}

BigInt mulmod(BigInt a, BigInt b, BigInt mod)
{
	BigInt x = 0, y = a % mod;
	while (b > 0)
	{
		if (b % 2 == 1)
		{
			x = (x + y) % mod;
		}
		y = (y * 2) % mod;
		b /= 2;
	}
	return x % mod;
	
}


BigInt modulo(BigInt base, BigInt exponent, BigInt mod)
{
	BigInt x = 1;
	BigInt y = base;
	while (exponent > 0)
	{
		if (exponent % 2 == 1)
			x = (x * y) % mod;
		y = (y * y) % mod;
		exponent = exponent / 2;
	}
	return x % mod;
}

bool miler(BigInt p, int iteration)
{
	//std::cout << "p = " << p << std::endl;
	if (p < 2)
	{
		return false;
	}
	if (p != 2 && p % 2 == 0)
	{
		return false;
	}
	BigInt s = p - 1;
	while (s % 2 == 0)
	{
		s /= 2;
	}
	for (int i = 0; i < iteration; i++)
	{
		BigInt a = rand() % (p - 1) + 1, temp = s;
		BigInt mod = modulo(a, temp, p);
		while (temp != p - 1 && mod != 1 && mod != p - 1)
		{
			mod = mulmod(mod, mod, p);
			temp *= 2;
		}
		if (mod != p - 1 && temp % 2 == 0)
		{
			return false;
		}
	}
	return true;
}

std::string ToBinary(BigInt b)
{
	std::string result = "";

	while (b != 1) {

		if (b % 2 == 1) {
			result = "1" + result;
		}
		else {
			result = "0" + result;
		}

		b = b / 2;
	}

	result = "1" + result;

	while (result.length() % 4 != 0) {
		result = "0" + result;
	}
	return result;
}

BigInt getRandomNum(BigInt from, int iter)
{
	BigInt result;

	from = from % 2 == 0 ? from + 1 : from;

	for (BigInt i = from; true; i += 2)
	{
		if (miler(i, iter))
		{
			result = i;
			break;
		}
	}

	return result;
}

std::string sdvig(std::string src)//сдвигает число вправо на 1 бит(вместо перегрузки оператора >>)
{
	std::string result;
	result = src.substr(0, src.length() - 1);
	return result;
}

bool chheck(std::string l) {//проверяет, l > 0 если да -> true, иначе -> false
	for (int i = 0; i < l.length(); i++) {
		if (l[i] == '1')
		{
			return true;
		}
	}
	return false;
}
BigInt modpow(BigInt base, BigInt exp, BigInt modulus) {
	BigInt result = 1;
	std::string bin = ToBinary(exp);
	while (chheck(bin)) {
		if (bin[bin.length() - 1] == '1') {
			result = (result * base) % modulus;
		}
		base = pow(base, 2) % modulus;
		bin = sdvig(bin);
	}
	return result;
	
}