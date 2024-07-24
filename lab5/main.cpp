#include <iostream>		//���� �� � ����� �� ������ - ��� �� � ��� � ���� ��������
#include <fstream>		//��� ������ ��������� ������
#include <string>		//�������� ��������� ���-�� �����
#include <vector>		//���������� ��� ���������� ������ � ���������
#include "Polinomial.h" //����������� ����� ��� ���������� �����������

using namespace std;

ifstream inputFile;// ����, ���������� ������� ������
vector<Polinomial> MatrixPolinom;// ������� �
vector<Polinomial> MatrixEntity; // ��������� ������� �

Polinomial Determinant(vector<Polinomial> _array, int size) {
	Polinomial Result;//������� ������ ���������, ���� ����� �������� ���������� ���������� ������������
	if (size == 1)// ������ ����� �� ������ ����, �� ���������� ������� ����������� 1 - ���� �������
		Result.Sum(Polinomial(0, vector<Fraction>(1) = { _array[0].GetCoefficient(0) }));// ������� ��������� � ��������������� ����������
	if (size == 2) {//���� ����������� ������� == 2
		Polinomial a = Polinomial(_array[0]), b = Polinomial(_array[1]);//�������������� ������ ��� ����������
		//���������� ���������� ����:
		// |(�1 �2)|
		// |(�3 �4)|
		a.Mul(_array[3]);
		b.Mul(_array[2]);
		Result.Sum(a);
		Result.Sub(b);
	}
	if (size > 2) {//���� ����������� ������� ������ ����
		vector<Polinomial> tmp;//������������� ��������� ���������� ������������
		int new_size = size - 1;// ������ ����� ���������� ��� ������������
		int i, j, k;//��������������� ����������
		for (i = 0; i < size; i++) {//��������, ����� ������� ��������� - ������ ������ ��������� �� ���������� ������
			Polinomial temp;//������������� ��������� - ��� ���������� �������� ������ �������
			if (i % 2 == 0)// ���������� ����: (-1)^i, ��� i - ����� ������
				temp.Sum(_array[i]);
			else
				temp.Sub(_array[i]);
			tmp.push_back(temp);//������� ���������� � ������ �������
			vector<Polinomial> new_matrix;//����� ����������
			for (j = 1; j < size; j++)    //��� ������� - �.�. ��� ������������, �� ������� ������ ������� �������� �������
				for (k = 0; k < size; k++)//��� ������ - ��� ��� ������
					if (k != i)
						new_matrix.push_back(_array[j * size + k]);//������� ������� � ����������
			tmp[i].Mul(Determinant(new_matrix, new_size));//��������� ������������ ���������� ����������
			new_matrix.clear();//������� ����� ����� ����
			Result.Sum(tmp[i]);//��������� ���������� ����� � ���������� �����������
		}
	}
	return Result;//������� ���������� ������������-���������
}

int GetMatrixSize() {
	string tmp;
	int size;
	//������-�������� ������� �� 3 ������: �������� ����������-�������, ����� ��������� � ����� -> ������� �������
	inputFile >> tmp;//������ ������ ����� ������
	if (tmp == "")
		return 0;//����� �������
	inputFile >> tmp;//������ ������ ����� ������
	if (tmp == "")
		return 0;//����� �������
	try{// ��� ���������� ����� ������ ���������� ������, ������� � ����� ����������
		inputFile >> size;//������ ������� ����� �����
	}
	catch (const std::exception&){
		return 0;//����� �������
	} 
	return size;//����� ������� �������
}

void GetMatrix(int size) {
	int i, j, k, length;//��������������� ����������
	int numerator, denominator, result;//���������, ����������� � �������� ��������� ��������������
	int signum;//���� ����� - ��� ������ ��������� � ����������� ��� ����� ��������������, ��� ����� ���� �������
	bool edge;//����������, ����������� �� ������ ��������� ��� �����������
	string data;//��� ����������� ���������� �� �����
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			inputFile >> data;//��������� ����� �� �����
			length = data.size();//���������� ��� ����� ��� �����������
			edge = false;				//�� ��������� ������� ����������� ���������
			result = 0;					//������� ��������� 0
			numerator = 0;				//���������...
			denominator = 0;			//� ����������� ���� �� �����
			signum = 1;					//�� ��������� ����� ���������������
			for (k = 0; k < length; k++) {
				if (data[k] == '-')//���� ����� �������������
					signum = -1;
				else if (data[k] == '/')//���� ��������� ��������� ��������
					edge = true;
				else if (!edge) //���� ����������� ���������
 					numerator = numerator * 10 + (data[k]-'0');
				else			//���� ����������� �����������	
					denominator = denominator * 10 + (data[k]-'0');
			}
			if (denominator == 0)//���� �� ����������� ���� �� �����
				denominator = 1;
			numerator *= signum;//���� ����� �������������, �� �������� ��������� � ���������� ����
			vector<Fraction> coeff;
			if (i == j) {
				coeff = vector<Fraction>(2) = { Fraction(0,1), Fraction(1,1) };//������ ������� ��������� �������, ������� �� ������� ���������
				MatrixEntity.push_back(Polinomial(1, coeff));//���������� ���������� �������
			}
			else {
				coeff = vector<Fraction>(1) = { Fraction() };//������� ������� ��������� �������, �� ������� �� ������� ���������
				MatrixEntity.push_back(Polinomial(0, coeff));//���������� ���������� �������
			}
			coeff.clear();//����� ������������� ������� ������ �������������
			coeff = vector<Fraction>(1) = { Fraction(numerator,denominator) };//������� ������ ������������� ��� �������� ������� �
			MatrixPolinom.push_back(Polinomial(0, coeff));//���������� ���������� �������
			coeff.clear();//������� ����� ����� ����
		}
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");//����� ������ ������, ���� ��� ������ ������
	if (argc == 2) {//���� ���������� 2 - ���� �� ���������� � ���� �� �����
		inputFile = ifstream(argv[1]);//�������� ���� �� ���� �� ��������� ��� �������
		if (inputFile.is_open()) {//���� ���� ������� �������
			int matrixSize = GetMatrixSize();//�������� ����������� �������
			if (matrixSize > 0) {//���� ������� ������ ����
				int i, j;
				GetMatrix(matrixSize);//��������� ������� �� �����
				//��������� ������� ����: A = xE, ��� � - �������� �������, � - ��������� �������, x - ���� ����������� ����������� ��������
				for (i = 0; i < matrixSize; i++)
					for (j = 0; j < matrixSize; j++)
						MatrixPolinom[i * matrixSize + j].Sub(MatrixEntity[i * matrixSize + j]);
				Polinomial Result = Polinomial(Determinant(MatrixPolinom, matrixSize));//�������� ������������ �������
				cout << "Characteristic polynomial:\nF(x) = " << Result.Show() << endl;//����� ����������� ����������
				vector<Fraction> roots = Polinomial::Solve(Result);//���� ������������ ����� ����������
				for (i = 0; i < roots.size(); i++) {// ������� ��������� �����
					cout << "x_" << i + 1 << " = " << roots[i].Show() << endl;
				}
			}
		}
		else {//���� ���� �� ������� �������
			cout << "������: �� ������� ������� ����." << endl;
			return 0;
		}
	}
	else {// ���� ���-�� ���������� �� ��������� � ���������
		cout << "������: �������� ���������� ����������� ����������." << endl;
	}
	//������ ������ ����� ����
	MatrixPolinom.clear();
	MatrixEntity.clear();
	return 0;
}