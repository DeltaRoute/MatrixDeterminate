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

int _gcd(int a, int b) {//���������� ����� �������� - ��� �����, ��� ���������� ������ �������
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

Fraction::Fraction()//����������� ��� ����������
{
	numerator = 0;
	denominator = 1;
}

Fraction::Fraction(int _numerator, int _denomiantor)//����������� ��� ����� - ����, �� ��� �������
{
	numerator = _numerator;
	denominator = _denomiantor;
}

Fraction::Fraction(const Fraction& TMP)//����������� �����������
{
	numerator = TMP.numerator;
	denominator = TMP.denominator;
}

void Fraction::Simplify() {//�� ������, ���� ����� ����� ���������
	int divided = _gcd(denominator,numerator);
	denominator /= divided;
	numerator /= divided;
}
void Fraction::Simplify(Fraction a) {//�� ������, ���� ����� ����� ���������
	int divided = _gcd(a.denominator, a.numerator);
	a.denominator /= divided;
	a.numerator /= divided;
}

Fraction Fraction::Power(Fraction value, int count)//���������� ����� � ������� - ��� ����� � ��� ������������
{
	if (count == 0)
		return Fraction(1, 1);
	int i;
	Fraction tmp = Fraction(value);
	for (i = 1; i < count; i++)
		tmp.Mul(value);
	return tmp;
}
void Fraction::Sum(Fraction argument)//�������� ���� ������
{
	if (this->denominator != argument.denominator) {//���� ����������� �� �����, �� �������� � ������ �����������
		int max_denominator = max(this->denominator, argument.denominator);
		int min_denominator = min(this->denominator, argument.denominator);
		if (max_denominator % min_denominator != 0) {// ���� ���� ����������� �� ������ �������, �� ������ ����������� �����������
			this->numerator *= argument.denominator;
			this->denominator *= argument.denominator;
			argument.numerator *= this->numerator;
			argument.denominator *= this->denominator;
		}
		else {//����� �������� �� ������� ������ ����������� � �������� �����������
			this->numerator *= (max_denominator / this->denominator);
			this->denominator = max_denominator;
			argument.numerator *= (max_denominator / argument.denominator);
			argument.denominator = max_denominator;
		}
	}
	this->numerator += argument.numerator;//����� ����� ��������� ���������
	this->Simplify();
}

void Fraction::Sub(Fraction argument)//�������� ������ - �������� ���������� �����, ������ ����������� ��� �� ����
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

void Fraction::Mul(Fraction argument)//��������� ������ - ������ �� ����������
{
	numerator *= argument.numerator;
	denominator *= argument.denominator;
}

void Fraction::Div(Fraction argument)//������� ������ - ���� �������� ������
{
	numerator *= argument.denominator;
	denominator *= argument.numerator;
}

string Fraction::Show()//����� � �������
{
	Simplify();//����� ������� �������� ����� ��� �������������
	string result = "";
	result += to_string(numerator);
	if (denominator != 1)
		result += '/' + to_string(denominator);
	return result;//������� ���������
}

double Fraction::Calculate()
{
	return (double)numerator / denominator;//����� � ���� ���������� �����
}

int Fraction::GetNumerator()
{
	return numerator;
}

int Fraction::GetDenominator()
{
	return denominator;
}
