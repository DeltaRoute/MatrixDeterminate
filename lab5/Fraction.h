#pragma once
#include <string>

using namespace std;

class Fraction {
private:
	int numerator;
	int denominator;
public:
	Fraction();
	Fraction(int _numerator, int _denomiantor);
	Fraction(const Fraction& TMP);
	void Sum(Fraction argument);
	void Sub(Fraction argument);
	void Mul(Fraction argument);
	void Div(Fraction argument);
	string Show();
	double Calculate();
	int GetNumerator();
	int GetDenominator();
	void Simplify();
	static void Simplify(Fraction a);
	static Fraction Power(Fraction value, int count);
};