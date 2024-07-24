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

int _gcd(int a, int b) {//наибольший общий делитель - как никак, всю математику писать вручную
	int tmp;
	a = abs(a);
	b = abs(b);
	while (b > 0) {
		tmp = a % b;
		a = b;
		b = tmp;
	}
	return a;
}

Fraction::Fraction()//конструктор без параметров
{
	numerator = 0;
	denominator = 1;
}

Fraction::Fraction(int _numerator, int _denomiantor)//конструктор для дроби - бред, но так удобнее
{
	numerator = _numerator;
	denominator = _denomiantor;
}

Fraction::Fraction(const Fraction& TMP)//конструктор копирования
{
	numerator = TMP.numerator;
	denominator = TMP.denominator;
}

void Fraction::Simplify() {//на случай, если дробь можно сократить
	int divided = _gcd(denominator,numerator);
	denominator /= divided;
	numerator /= divided;
}
void Fraction::Simplify(Fraction a) {//на случай, если дробь можно сократить
	int divided = _gcd(a.denominator, a.numerator);
	a.denominator /= divided;
	a.numerator /= divided;
}

Fraction Fraction::Power(Fraction value, int count)//возведение дроби в степень - тут можно и без подробностей
{
	if (count == 0)
		return Fraction(1, 1);
	int i;
	Fraction tmp = Fraction(value);
	for (i = 1; i < count; i++)
		tmp.Mul(value);
	return tmp;
}
void Fraction::Sum(Fraction argument)//сложение двух дробей
{
	if (this->denominator != argument.denominator) {//если знаменатели не равны, то приводим к общему знаменателю
		int max_denominator = max(this->denominator, argument.denominator);
		int min_denominator = min(this->denominator, argument.denominator);
		if (max_denominator % min_denominator != 0) {// если один знаменатель не кратен другому, то просто перемножаем знаменатели
			this->numerator *= argument.denominator;
			this->denominator *= argument.denominator;
			argument.numerator *= this->numerator;
			argument.denominator *= this->denominator;
		}
		else {//иначе умножаем на частное общего знаменателя и текущего знаменателя
			this->numerator *= (max_denominator / this->denominator);
			this->denominator = max_denominator;
			argument.numerator *= (max_denominator / argument.denominator);
			argument.denominator = max_denominator;
		}
	}
	this->numerator += argument.numerator;//после всего суммируем числители
	this->Simplify();
}

void Fraction::Sub(Fraction argument)//разность дробей - алгоритм аналогичен сумме, потому расписывать его не буду
{
	if (this->denominator != argument.denominator) {
		int max_denominator = max(this->denominator, argument.denominator);
		int min_denominator = min(this->denominator, argument.denominator);
		if (max_denominator % min_denominator != 0) {
			this->numerator *= argument.denominator;
			this->denominator *= argument.denominator;
			argument.numerator *= this->numerator;
			argument.denominator *= this->denominator;
		}
		this->numerator *= (max_denominator / this->denominator);
		this->denominator = max_denominator;
		argument.numerator *= (max_denominator / argument.denominator);
		argument.denominator = max_denominator;
	}
	this->numerator -= argument.numerator;
	this->Simplify();
}

void Fraction::Mul(Fraction argument)//умножение дробей - просто до безобразия
{
	numerator *= argument.numerator;
	denominator *= argument.denominator;
}

void Fraction::Div(Fraction argument)//деление дробей - тоже довольно просто
{
	numerator *= argument.denominator;
	denominator *= argument.numerator;
}

string Fraction::Show()//вывод в консоль
{
	Simplify();//перед показом упрощаем дробь при необходимости
	string result = "";
	result += to_string(numerator);
	if (denominator != 1)
		result += '/' + to_string(denominator);
	return result;//выводим результат
}

double Fraction::Calculate()
{
	return (double)numerator / denominator;//вывод в виде десятичной дроби
}

int Fraction::GetNumerator()
{
	return numerator;
}

int Fraction::GetDenominator()
{
	return denominator;
}
