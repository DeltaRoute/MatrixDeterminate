#pragma once
#include "Fraction.h"

using namespace std;

class Polinomial
{
private:
	unsigned int power_x;
	vector<Fraction> coefficients;//попробуйте найти кол-во коэффициентов побольше
	void ToInt();
	static vector<Fraction> SolveInt(Polinomial equation);
public:
	Polinomial(int Power, vector<Fraction> Coefficient);
	int GetPower();
	Fraction GetCoefficient(int i);
	void Sum(Polinomial argument);//
	void Sub(Polinomial argument);//
	void Mul(Polinomial argument);
	void Div(Polinomial denominator);
	Polinomial();
	Polinomial(const Polinomial& TMP);
	~Polinomial();
	string Show();
	Fraction Calculate(Fraction x);
	static vector<Fraction> Solve(Polinomial equation);
};