#include <string>	  //для вывода полученного многочлена	  
#include <vector>	  //для хранения многочленов, т.к. они имеют переменный размер
#include "Fraction.h" //собственный класс для определния дробей

using namespace std;

int gcd(int a, int b) {//наибольший общий делитель для поиска НОК
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
int lcn(int a, int b) {//НОК для приведения многчлена к целочисленному виду
	return (a / gcd(a, b)) * b;
}

class Polinomial//собственно, класс многочлена
{
private:
	unsigned int power_x;//наивысшая степень
	vector<Fraction> coefficients;//коэффициенты многочлена в порядке возрастания степени
	void ToInt();//приведение многочлена к целочисленному виду
	static vector<Fraction> SolveInt(Polinomial equation);//решение многочлена, приведённого к целочисленному виду
	double Calculate(double x);// вычисление многочлена
public:
	Polinomial(int Power, vector<Fraction> Coefficient);//конструктор многочлена
	int GetPower();//получение степени многочлена
	Fraction GetCoefficient(int i);//получение некоторого коэффициента из многочлена
	void Sum(Polinomial argument);//сложение двух полиномов
	void Sub(Polinomial argument);//вычитание полиномов
	void Mul(Polinomial argument);//умножение полиномов
	void Div(Polinomial denominator);//деление полиномов
	Polinomial();//стандартный конструктор для многочлена
	Polinomial(const Polinomial& TMP);//конструктор копирования многчлена
	~Polinomial();//деструктор многочлена
	string Show();//вывести многочлен в консоль
	Fraction Calculate(Fraction x);//вычисление многочлена в виде дроби
	static vector<Fraction> Solve(Polinomial equation);// решение многочлена
};

vector<int> GetNumerators(int value) {//получение делителей для числителя - зачем, обьясню ниже
	value = abs(value);
	vector<int> result;
	int i;
	for (i = 1; i <= value; i++)
		if (value % i == 0) {//если i является делителем
			result.push_back(i);//то заносим этот делитель в список
			result.push_back(-i);//и его отрицательное значение тоже
		}
	return result;
}
vector<int> GetDenominators(int value) {//получение делителей для знаменателя - аналогично делителям числителя
	value = abs(value);
	vector<int> result;
	int i;
	for (i = 1; i <= value; i++)
		if (value % i == 0)// если i является делителем
			result.push_back(i);//то заносим его в список, но без его отрицатеотного значения
			// из-за того, что рациональное число имеет вид p/q, где p - целое число, а q - натуральное, то знаменатель не должен быть отрицательным
	return result;
}
Polinomial::Polinomial(int Power, vector<Fraction> Coefficient)//конструктор
{
	power_x = Power;
	coefficients = Coefficient;
}
Polinomial::Polinomial()//тоже конструктор
{
	power_x = 0;
	coefficients = vector<Fraction>(1) = {Fraction()};
}
Polinomial::Polinomial(const Polinomial& TMP) {//конструктор копирования
	power_x = TMP.power_x;
	coefficients = TMP.coefficients;
}
Polinomial::~Polinomial()//деструктор
{
	coefficients.clear();
}
Fraction Polinomial::Calculate(Fraction x)//вычисление многочлена в виде дроби
{
	Fraction result;
	unsigned int l;
	for (l = 0; l <= power_x; l++) {//проходимся по всем коэффициентам
		Fraction tmp = Fraction(coefficients[l]);
		tmp.Mul(Fraction::Power(x,l));//умножаем на текущее значение х^l
		result.Sum(tmp);//и прибавляем к результату
	}
	return result;
}

vector<Fraction> Polinomial::Solve(Polinomial equation) {//решение многчлена
	int i;
	double x, y;
	bool integer = false;
	for (i = 0; i <= equation.power_x; i++) {//анализируем многочлен на наличие нецелых коэффициентов
		x = equation.coefficients[i].Calculate();
		y = (int)x;
		integer |= (x != y);
	}
	if (integer)//если многочлен не в целых коэффициентах
		equation.ToInt();//то приводим его к таковому
	return SolveInt(equation);//и решаем уже многочлен с целыми коэффициентами
}
vector<Fraction> Polinomial::SolveInt(Polinomial equation)//решение многочлена, приведённого к целым числам
{
	vector<Fraction> tmp_result;//промежуточный результат поиска корней
	int i;
	for (i = 0; i <= equation.power_x; i++)//сокращение дробей в многочлене
		equation.coefficients[i].Simplify();
	vector<Fraction> results;//конечный результат поиска корней - пока пуст
	if (equation.power_x == 1) {//если остался последний корень
		Fraction tmp = equation.coefficients[0];//свободный член полинома
		tmp.Div(Fraction(-equation.coefficients[1].GetNumerator(),equation.coefficients[1].GetDenominator()));// делим свободный член полинома на коэффициент при наивысшей степени
		tmp_result.push_back(tmp);//записываем корень в промежуточный результат
	}
	if (equation.power_x > 1) {//если уравнение нелинейное
		Polinomial temporary_equation;//промежуточный многочлен
		//корни можно найти по следущему алгоритму: если корни многочлена представить в виде дроби, 
		//где числитель - один из делителей свободного члена, а знаменатель - один из делителей коэффициента при наивысшей степени,
		//то подставив эти варианты дробей, хотя бы одна из дробей окажется корнем многочлена
		vector<int> numerators = GetNumerators(equation.coefficients[0].GetNumerator());//получаем делители для числтеля
		vector<int> denominators = GetDenominators(equation.coefficients[equation.power_x].GetNumerator());//получаем делители для знаенателя
		int i, j;
		bool exist = true;//ограничение на один корень за одно деление
		for (i = 0; i < numerators.size(); i++) {//проходимся по всем вариантам делителей свободного члена
			for (j = 0; j < denominators.size(); j++) {//проходимся по всем вариантам делителей коэффициента при наивысшей степени
				Fraction root = Fraction(numerators[i], denominators[j]);//инициализируем дробь
				if (equation.Calculate(root).GetNumerator() == 0 && exist) {// в случае, если при подстановке вышел ноль и корень еще не найден
					results.push_back(root);//заносим корень в список корней
					exist = false;//корень найден
				}
			}
		}
		if (results.size() != 0) {//если рациональный корень обнаружен
			equation.Div(Polinomial(1, vector<Fraction>(2) = { results[0], Fraction(1,1) }));//делим многочлен на двучлен вида: х-а, где а - корень
			tmp_result = SolveInt(equation);//решаем полученный многочлен снова
			tmp_result.push_back(results[0]);//после поиска корней заносим текущий корень в промежуточный список
		}
	}
	return tmp_result;//выводим найденные корни
}
double Polinomial::Calculate(double x)//вычисление многочлена в вещественном виде - по итогу осталось без нужды
{
	unsigned int l;
	double multiplier = 0;//результат вычисления
	for (l = 0; l <= power_x; l++) {//проходимся по коэффициентам
		multiplier += coefficients[l].Calculate()*pow(x,l);//вычисляем каждый коэффициент при данном значении х
	}
	return multiplier;//выводим полученный результат
}
void Polinomial::ToInt()// приведение многочлена к целым числам
{
	int c_lcn = lcn(abs(coefficients[0].GetDenominator()),abs(coefficients[1].GetDenominator())), i;//начало поиска общего знаменателя
	for (i = 2; i < power_x; i++)
		c_lcn = lcn(c_lcn, abs(coefficients[i].GetDenominator()));//ищем общий знаменатель для всех дробей в многочлене
	Mul(Polinomial(0, vector<Fraction>(1) = {Fraction(c_lcn,1)}));//умножаем многочлен на дробь вида: 1/(общий знаменатель)
	for (i = 0; i < power_x; i++)
		coefficients[i].Simplify();//упрощаем дроби в случае необходимости
}
void Polinomial::Sum(Polinomial argument)// сложение многочленов
{
	int tmp = argument.GetPower() + 1;
	int second_power = power_x + 1;
	int i;
	int max_power = max(tmp, second_power);//получаем степень конечного многочлена
	vector<Fraction> full_coefficient = vector<Fraction>(max_power);//создаем конечный массив заполненный нулями
	for (i = 0; i < tmp; i++)
		full_coefficient[i].Sum(argument.GetCoefficient(i));//прибавляем коэффициенты входного многочлена
	for (i = 0; i < second_power; i++)
		full_coefficient[i].Sum(coefficients[i]);//прибавляем коэффициенты текущего многочлена
	power_x = max_power - 1;
	coefficients = full_coefficient;//теперь текущий многочлен является конечным
}
void Polinomial::Sub(Polinomial argument)//вычитание многочленов
{
	int tmp = argument.GetPower() + 1;
	int second_power = power_x + 1;
	int i;
	int max_power = max(tmp, second_power);//получаем степень конечного многочлена - что-то может быть нулевым
	vector<Fraction> full_coefficient = vector<Fraction>(max_power);//создаем конечный массив заполненный нулями
	for (i = 0; i < tmp; i++)
		full_coefficient[i].Sub(argument.GetCoefficient(i));//вычитаем коэффициенты входного многочлена
	for (i = 0; i < second_power; i++)
		full_coefficient[i].Sum(coefficients[i]);//прибавляем коэффициенты текущего многочлена
	power_x = max_power - 1;
	coefficients = full_coefficient;//теперь текущий многочлен является конечным
}
void Polinomial::Mul(Polinomial argument)//умножение многочленов
{
	int i, l, first_power = power_x, second_power = argument.GetPower();
	int max_power = first_power + second_power;//получаем степень конечного многочлена
	vector<Fraction> full_coefficient = vector<Fraction>(max_power + 1);//массив дробей-коэффициентов многочлена
	for (i = 0; i <= second_power; i++)
		for (l = 0; l <= first_power; l++) {
			//просто перемножаем коэффициенты
			Fraction tmp = Fraction(coefficients[l]);
			tmp.Mul(argument.GetCoefficient(i));
			full_coefficient[i + l].Sum(tmp);
		}
	this->power_x = max_power;
	this->coefficients = full_coefficient;//теперь текущий многочлен является конечным
}
void Polinomial::Div(Polinomial denominator)//деление многочленов
{
	vector<Fraction> new_coefficients = vector<Fraction>(power_x);// массив дробей-коэффициентов многочлена
	int new_power = power_x - (denominator.power_x);//новое значение степени многочлена после деления
	int i;
	Fraction old_result = Fraction();//результат от деления предыдущей степени - схема горнера в помощь
	for (i = power_x; i != 0; i--) {// проходимся по всем коэффициентам
		Fraction tmp = Fraction(old_result);//промежуточный результат
		tmp.Mul(denominator.coefficients[0]);
		tmp.Sum(coefficients[i]);
		new_coefficients[i - 1] = Fraction(tmp);// новое значение коэффициента нового многочлена
		old_result = Fraction(new_coefficients[i - 1]);// записываем остаток
	}
	//переназначаем переменные
	this->power_x = new_power;
	this->coefficients = new_coefficients;
}

int Polinomial::GetPower()//получение степени многочлена
{
	return power_x;
}
Fraction Polinomial::GetCoefficient(int i)//получение некоторого коэффициента многочлена
{
	return coefficients[i];
}
string Polinomial::Show()//вывод многочлена в консоль
{
	string result;
	unsigned int i;
	for (i = power_x; i != 0; i--)
	{
		if (coefficients[i].Calculate() != 0) {//если значение коэффициента не равно нулю - ибо ноль выводить незачем
			if (coefficients[i].Calculate() > 0 && i != power_x)//если значение больше нуля и это не первый коэффициент по порядку вывода
				result += "+";
			if (coefficients[i].Calculate() != 1)//если коэффициент не является целым числом
				result += coefficients[i].Show() + ' ';
			result += "x";
			if (i > 1)//если степень необходимо указать
				result += "^" + to_string(i);
		}
	}
	if (coefficients[0].Calculate() != 0) {//запись последнего коэффициента - свободного члена
		if (coefficients[0].Calculate() > 0)
			result += '+';
		result += coefficients[0].Show();
	}
	return result;
}