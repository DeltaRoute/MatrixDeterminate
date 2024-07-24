#include <string>	  //��� ������ ����������� ����������	  
#include <vector>	  //��� �������� �����������, �.�. ��� ����� ���������� ������
#include "Fraction.h" //����������� ����� ��� ���������� ������

using namespace std;

int gcd(int a, int b) {//���������� ����� �������� ��� ������ ���
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
int lcn(int a, int b) {//��� ��� ���������� ��������� � �������������� ����
	return (a / gcd(a, b)) * b;
}

class Polinomial//����������, ����� ����������
{
private:
	unsigned int power_x;//��������� �������
	vector<Fraction> coefficients;//������������ ���������� � ������� ����������� �������
	void ToInt();//���������� ���������� � �������������� ����
	static vector<Fraction> SolveInt(Polinomial equation);//������� ����������, ����������� � �������������� ����
	double Calculate(double x);// ���������� ����������
public:
	Polinomial(int Power, vector<Fraction> Coefficient);//����������� ����������
	int GetPower();//��������� ������� ����������
	Fraction GetCoefficient(int i);//��������� ���������� ������������ �� ����������
	void Sum(Polinomial argument);//�������� ���� ���������
	void Sub(Polinomial argument);//��������� ���������
	void Mul(Polinomial argument);//��������� ���������
	void Div(Polinomial denominator);//������� ���������
	Polinomial();//����������� ����������� ��� ����������
	Polinomial(const Polinomial& TMP);//����������� ����������� ���������
	~Polinomial();//���������� ����������
	string Show();//������� ��������� � �������
	Fraction Calculate(Fraction x);//���������� ���������� � ���� �����
	static vector<Fraction> Solve(Polinomial equation);// ������� ����������
};

vector<int> GetNumerators(int value) {//��������� ��������� ��� ��������� - �����, ������� ����
	value = abs(value);
	vector<int> result;
	int i;
	for (i = 1; i <= value; i++)
		if (value % i == 0) {//���� i �������� ���������
			result.push_back(i);//�� ������� ���� �������� � ������
			result.push_back(-i);//� ��� ������������� �������� ����
		}
	return result;
}
vector<int> GetDenominators(int value) {//��������� ��������� ��� ����������� - ���������� ��������� ���������
	value = abs(value);
	vector<int> result;
	int i;
	for (i = 1; i <= value; i++)
		if (value % i == 0)// ���� i �������� ���������
			result.push_back(i);//�� ������� ��� � ������, �� ��� ��� �������������� ��������
			// ��-�� ����, ��� ������������ ����� ����� ��� p/q, ��� p - ����� �����, � q - �����������, �� ����������� �� ������ ���� �������������
	return result;
}
Polinomial::Polinomial(int Power, vector<Fraction> Coefficient)//�����������
{
	power_x = Power;
	coefficients = Coefficient;
}
Polinomial::Polinomial()//���� �����������
{
	power_x = 0;
	coefficients = vector<Fraction>(1) = {Fraction()};
}
Polinomial::Polinomial(const Polinomial& TMP) {//����������� �����������
	power_x = TMP.power_x;
	coefficients = TMP.coefficients;
}
Polinomial::~Polinomial()//����������
{
	coefficients.clear();
}
Fraction Polinomial::Calculate(Fraction x)//���������� ���������� � ���� �����
{
	Fraction result;
	unsigned int l;
	for (l = 0; l <= power_x; l++) {//���������� �� ���� �������������
		Fraction tmp = Fraction(coefficients[l]);
		tmp.Mul(Fraction::Power(x,l));//�������� �� ������� �������� �^l
		result.Sum(tmp);//� ���������� � ����������
	}
	return result;
}

vector<Fraction> Polinomial::Solve(Polinomial equation) {//������� ���������
	int i;
	double x, y;
	bool integer = false;
	for (i = 0; i <= equation.power_x; i++) {//����������� ��������� �� ������� ������� �������������
		x = equation.coefficients[i].Calculate();
		y = (int)x;
		integer |= (x != y);
	}
	if (integer)//���� ��������� �� � ����� �������������
		equation.ToInt();//�� �������� ��� � ��������
	return SolveInt(equation);//� ������ ��� ��������� � ������ ��������������
}
vector<Fraction> Polinomial::SolveInt(Polinomial equation)//������� ����������, ����������� � ����� ������
{
	vector<Fraction> tmp_result;//������������� ��������� ������ ������
	int i;
	for (i = 0; i <= equation.power_x; i++)//���������� ������ � ����������
		equation.coefficients[i].Simplify();
	vector<Fraction> results;//�������� ��������� ������ ������ - ���� ����
	if (equation.power_x == 1) {//���� ������� ��������� ������
		Fraction tmp = equation.coefficients[0];//��������� ���� ��������
		tmp.Div(Fraction(-equation.coefficients[1].GetNumerator(),equation.coefficients[1].GetDenominator()));// ����� ��������� ���� �������� �� ����������� ��� ��������� �������
		tmp_result.push_back(tmp);//���������� ������ � ������������� ���������
	}
	if (equation.power_x > 1) {//���� ��������� ����������
		Polinomial temporary_equation;//������������� ���������
		//����� ����� ����� �� ��������� ���������: ���� ����� ���������� ����������� � ���� �����, 
		//��� ��������� - ���� �� ��������� ���������� �����, � ����������� - ���� �� ��������� ������������ ��� ��������� �������,
		//�� ��������� ��� �������� ������, ���� �� ���� �� ������ �������� ������ ����������
		vector<int> numerators = GetNumerators(equation.coefficients[0].GetNumerator());//�������� �������� ��� ��������
		vector<int> denominators = GetDenominators(equation.coefficients[equation.power_x].GetNumerator());//�������� �������� ��� ����������
		int i, j;
		bool exist = true;//����������� �� ���� ������ �� ���� �������
		for (i = 0; i < numerators.size(); i++) {//���������� �� ���� ��������� ��������� ���������� �����
			for (j = 0; j < denominators.size(); j++) {//���������� �� ���� ��������� ��������� ������������ ��� ��������� �������
				Fraction root = Fraction(numerators[i], denominators[j]);//�������������� �����
				if (equation.Calculate(root).GetNumerator() == 0 && exist) {// � ������, ���� ��� ����������� ����� ���� � ������ ��� �� ������
					results.push_back(root);//������� ������ � ������ ������
					exist = false;//������ ������
				}
			}
		}
		if (results.size() != 0) {//���� ������������ ������ ���������
			equation.Div(Polinomial(1, vector<Fraction>(2) = { results[0], Fraction(1,1) }));//����� ��������� �� ������� ����: �-�, ��� � - ������
			tmp_result = SolveInt(equation);//������ ���������� ��������� �����
			tmp_result.push_back(results[0]);//����� ������ ������ ������� ������� ������ � ������������� ������
		}
	}
	return tmp_result;//������� ��������� �����
}
double Polinomial::Calculate(double x)//���������� ���������� � ������������ ���� - �� ����� �������� ��� �����
{
	unsigned int l;
	double multiplier = 0;//��������� ����������
	for (l = 0; l <= power_x; l++) {//���������� �� �������������
		multiplier += coefficients[l].Calculate()*pow(x,l);//��������� ������ ����������� ��� ������ �������� �
	}
	return multiplier;//������� ���������� ���������
}
void Polinomial::ToInt()// ���������� ���������� � ����� ������
{
	int c_lcn = lcn(abs(coefficients[0].GetDenominator()),abs(coefficients[1].GetDenominator())), i;//������ ������ ������ �����������
	for (i = 2; i < power_x; i++)
		c_lcn = lcn(c_lcn, abs(coefficients[i].GetDenominator()));//���� ����� ����������� ��� ���� ������ � ����������
	Mul(Polinomial(0, vector<Fraction>(1) = {Fraction(c_lcn,1)}));//�������� ��������� �� ����� ����: 1/(����� �����������)
	for (i = 0; i < power_x; i++)
		coefficients[i].Simplify();//�������� ����� � ������ �������������
}
void Polinomial::Sum(Polinomial argument)// �������� �����������
{
	int tmp = argument.GetPower() + 1;
	int second_power = power_x + 1;
	int i;
	int max_power = max(tmp, second_power);//�������� ������� ��������� ����������
	vector<Fraction> full_coefficient = vector<Fraction>(max_power);//������� �������� ������ ����������� ������
	for (i = 0; i < tmp; i++)
		full_coefficient[i].Sum(argument.GetCoefficient(i));//���������� ������������ �������� ����������
	for (i = 0; i < second_power; i++)
		full_coefficient[i].Sum(coefficients[i]);//���������� ������������ �������� ����������
	power_x = max_power - 1;
	coefficients = full_coefficient;//������ ������� ��������� �������� ��������
}
void Polinomial::Sub(Polinomial argument)//��������� �����������
{
	int tmp = argument.GetPower() + 1;
	int second_power = power_x + 1;
	int i;
	int max_power = max(tmp, second_power);//�������� ������� ��������� ���������� - ���-�� ����� ���� �������
	vector<Fraction> full_coefficient = vector<Fraction>(max_power);//������� �������� ������ ����������� ������
	for (i = 0; i < tmp; i++)
		full_coefficient[i].Sub(argument.GetCoefficient(i));//�������� ������������ �������� ����������
	for (i = 0; i < second_power; i++)
		full_coefficient[i].Sum(coefficients[i]);//���������� ������������ �������� ����������
	power_x = max_power - 1;
	coefficients = full_coefficient;//������ ������� ��������� �������� ��������
}
void Polinomial::Mul(Polinomial argument)//��������� �����������
{
	int i, l, first_power = power_x, second_power = argument.GetPower();
	int max_power = first_power + second_power;//�������� ������� ��������� ����������
	vector<Fraction> full_coefficient = vector<Fraction>(max_power + 1);//������ ������-������������� ����������
	for (i = 0; i <= second_power; i++)
		for (l = 0; l <= first_power; l++) {
			//������ ����������� ������������
			Fraction tmp = Fraction(coefficients[l]);
			tmp.Mul(argument.GetCoefficient(i));
			full_coefficient[i + l].Sum(tmp);
		}
	this->power_x = max_power;
	this->coefficients = full_coefficient;//������ ������� ��������� �������� ��������
}
void Polinomial::Div(Polinomial denominator)//������� �����������
{
	vector<Fraction> new_coefficients = vector<Fraction>(power_x);// ������ ������-������������� ����������
	int new_power = power_x - (denominator.power_x);//����� �������� ������� ���������� ����� �������
	int i;
	Fraction old_result = Fraction();//��������� �� ������� ���������� ������� - ����� ������� � ������
	for (i = power_x; i != 0; i--) {// ���������� �� ���� �������������
		Fraction tmp = Fraction(old_result);//������������� ���������
		tmp.Mul(denominator.coefficients[0]);
		tmp.Sum(coefficients[i]);
		new_coefficients[i - 1] = Fraction(tmp);// ����� �������� ������������ ������ ����������
		old_result = Fraction(new_coefficients[i - 1]);// ���������� �������
	}
	//������������� ����������
	this->power_x = new_power;
	this->coefficients = new_coefficients;
}

int Polinomial::GetPower()//��������� ������� ����������
{
	return power_x;
}
Fraction Polinomial::GetCoefficient(int i)//��������� ���������� ������������ ����������
{
	return coefficients[i];
}
string Polinomial::Show()//����� ���������� � �������
{
	string result;
	unsigned int i;
	for (i = power_x; i != 0; i--)
	{
		if (coefficients[i].Calculate() != 0) {//���� �������� ������������ �� ����� ���� - ��� ���� �������� �������
			if (coefficients[i].Calculate() > 0 && i != power_x)//���� �������� ������ ���� � ��� �� ������ ����������� �� ������� ������
				result += "+";
			if (coefficients[i].Calculate() != 1)//���� ����������� �� �������� ����� ������
				result += coefficients[i].Show() + ' ';
			result += "x";
			if (i > 1)//���� ������� ���������� �������
				result += "^" + to_string(i);
		}
	}
	if (coefficients[0].Calculate() != 0) {//������ ���������� ������������ - ���������� �����
		if (coefficients[0].Calculate() > 0)
			result += '+';
		result += coefficients[0].Show();
	}
	return result;
}