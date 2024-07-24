#include <iostream>		//если вы и этого не знаете - мне не о чем с вами говорить
#include <fstream>		//для чтения текстовых файлов
#include <string>		//выводить результат как-то нужно
#include <vector>		//контейнеры для стабильной работы с матрицами
#include "Polinomial.h" //собственный класс для вычисления многочленов

using namespace std;

ifstream inputFile;// файл, содержащий входные данные
vector<Polinomial> MatrixPolinom;// Матрица А
vector<Polinomial> MatrixEntity; // единичная матрица Е

Polinomial Determinant(vector<Polinomial> _array, int size) {
	Polinomial Result;//создаем пустой многочлен, куда будем заночить результаты вычислений определителя
	if (size == 1)// вообще этого не должно быть, но технически матрица размерности 1 - тоже матрица
		Result.Sum(Polinomial(0, vector<Fraction>(1) = { _array[0].GetCoefficient(0) }));// заносим результат в соответствующую переменную
	if (size == 2) {//если размерность матрицы == 2
		Polinomial a = Polinomial(_array[0]), b = Polinomial(_array[1]);//подготавливаем миноры для вычислений
		//происходит вычисление вида:
		// |(а1 а2)|
		// |(а3 а4)|
		a.Mul(_array[3]);
		b.Mul(_array[2]);
		Result.Sum(a);
		Result.Sub(b);
	}
	if (size > 2) {//если размерность матрицы больше двух
		vector<Polinomial> tmp;//промежуточный результат вычисления определителя
		int new_size = size - 1;// размер новой подматрицы для определителя
		int i, j, k;//вспомогательные переменные
		for (i = 0; i < size; i++) {//выбираем, какой столбец исключаем - первую строку исключаем на постоянной основе
			Polinomial temp;//промежкточный многочлен - для вычисления текущего минора матрицы
			if (i % 2 == 0)// вычисление вида: (-1)^i, где i - номер минора
				temp.Sum(_array[i]);
			else
				temp.Sub(_array[i]);
			tmp.push_back(temp);//заносим переменную в список миноров
			vector<Polinomial> new_matrix;//новая подматрица
			for (j = 1; j < size; j++)    //это столбцы - т.к. это определитель, то убираем первые элемены столбцов убираем
				for (k = 0; k < size; k++)//это строки - все без первой
					if (k != i)
						new_matrix.push_back(_array[j * size + k]);//заносим элемент в подматрицу
			tmp[i].Mul(Determinant(new_matrix, new_size));//вычисляем определитель полученной подматрицы
			new_matrix.clear();//очищаем мусор после себя
			Result.Sum(tmp[i]);//суммируем полученный минор с глобальным результатом
		}
	}
	return Result;//выводим полученный определитель-многочлен
}

int GetMatrixSize() {
	string tmp;
	int size;
	//строка-параметр состоит из 3 частей: название переменной-размера, знака равенства и числа -> размера матрицы
	inputFile >> tmp;//чтение первой части строки
	if (tmp == "")
		return 0;//сброс функции
	inputFile >> tmp;//чтение второй части строки
	if (tmp == "")
		return 0;//сброс функции
	try{// при считывании числа можеть возникнуть ошибка, поэтому её нужно обработать
		inputFile >> size;//чтение третьей части файла
	}
	catch (const std::exception&){
		return 0;//сброс функции
	} 
	return size;//вывод размера матрицы
}

void GetMatrix(int size) {
	int i, j, k, length;//вспомогательные переменные
	int numerator, denominator, result;//числитель, знаменатель и конечный результат соответственно
	int signum;//знак числа - при чтении числителя и знаменателя оба будут положительными, что может быть неверно
	bool edge;//определяет, считывается ли сейчас числитель или знаменатель
	string data;//для дальнейшего считывания из файла
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			inputFile >> data;//считываем число из файла
			length = data.size();//определяем его длину для расшифровки
			edge = false;				//по умолчанию сначала считывается числитель
			result = 0;					//текущий результат 0
			numerator = 0;				//числитель...
			denominator = 0;			//и знаменатель тоже по нулям
			signum = 1;					//по умолчанию число неотрицательное
			for (k = 0; k < length; k++) {
				if (data[k] == '-')//если число отрицательное
					signum = -1;
				else if (data[k] == '/')//если считвание числителя окончено
					edge = true;
				else if (!edge) //если считывается числитель
 					numerator = numerator * 10 + (data[k]-'0');
				else			//если считывается знаменатель	
					denominator = denominator * 10 + (data[k]-'0');
			}
			if (denominator == 0)//если до знаменателя дело не дошло
				denominator = 1;
			numerator *= signum;//если дробь отрицательная, то приводим числитель к требуемому виду
			vector<Fraction> coeff;
			if (i == j) {
				coeff = vector<Fraction>(2) = { Fraction(0,1), Fraction(1,1) };//создаём элемент единичной матрицы, лежащий на главной диагонали
				MatrixEntity.push_back(Polinomial(1, coeff));//записываем полученный элемент
			}
			else {
				coeff = vector<Fraction>(1) = { Fraction() };//создаем элемент единичной матрицы, не лежащий на главной диагонали
				MatrixEntity.push_back(Polinomial(0, coeff));//записываем полученный элемент
			}
			coeff.clear();//перед пересозданием очищаем массив коэффициентов
			coeff = vector<Fraction>(1) = { Fraction(numerator,denominator) };//создаем массив коэффициентов для элемента матрицы А
			MatrixPolinom.push_back(Polinomial(0, coeff));//записываем полученный элемент
			coeff.clear();//очищаем мусор после себя
		}
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");//будет больно глазам, если эту строку убрать
	if (argc == 2) {//если аргументов 2 - путь до приложения и путь до файла
		inputFile = ifstream(argv[1]);//получаем файл по пути из аргумента при запуске
		if (inputFile.is_open()) {//если файл удалось открыть
			int matrixSize = GetMatrixSize();//получаем ращмерность матрицы
			if (matrixSize > 0) {//если матрица вообще есть
				int i, j;
				GetMatrix(matrixSize);//считываем матрицу из фалйа
				//вычисляем матрицу вида: A = xE, где А - исходная матрица, Е - единичная матрица, x - пока неизвестное собственное значение
				for (i = 0; i < matrixSize; i++)
					for (j = 0; j < matrixSize; j++)
						MatrixPolinom[i * matrixSize + j].Sub(MatrixEntity[i * matrixSize + j]);
				Polinomial Result = Polinomial(Determinant(MatrixPolinom, matrixSize));//получаем определитель матрицы
				cout << "Characteristic polynomial:\nF(x) = " << Result.Show() << endl;//вывод полученного многочлена
				vector<Fraction> roots = Polinomial::Solve(Result);//ищем рациональные корни многочлена
				for (i = 0; i < roots.size(); i++) {// выводим найденные корни
					cout << "x_" << i + 1 << " = " << roots[i].Show() << endl;
				}
			}
		}
		else {//если файл не удалось открыть
			cout << "Ошибка: не удалось открыть файл." << endl;
			return 0;
		}
	}
	else {// если кол-во аргументов не совпадает с требуемым
		cout << "Ошибка: неверное количество фактических параметров." << endl;
	}
	//чистим память после себя
	MatrixPolinom.clear();
	MatrixEntity.clear();
	return 0;
}