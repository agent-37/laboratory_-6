#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <string>
#include "matrix.h"
typedef float** d_mat;
//���������  �������� ������� nXn
void create_mat(d_mat(&fA), long n) {
	for (int i = 0; i < n; i++)
		fA[i] = new float[n];
	//�������������� 0
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			fA[i][j] = 0;
}

//���������  ������ �� �����
void read_f_mat(FILE* fin,  d_mat(&a), long n) {
	float d = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			fscanf(fin, "%f", (&a[i][j]));
		}
}

//��������� ������ ������� � ����
void write_f_mat(FILE* fout, d_mat(&a), long n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fprintf(fout, "%f", a[i][j]);
			fprintf(fout, "%c", ' ');
		}
		fprintf(fout, "%c", '\n');
	}
	fprintf(fout, "%c", '\n');
}

//��������� ������ 2 ������ � ������� �������
void swap(d_mat(&mat), long n, int i, int i1) {
	int x;
	for (int j = 0; j < n; j++) {
		x = mat[i][j];
		mat[i][j] = mat[i1][j];
		mat[i1][j] = x;
	}
}

//��������� ������������ ������� �� ����� q
void mult_numXmat(d_mat a, long n, long q,  d_mat(&b)) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			b[i][j] = a[i][j] * q;
}

//��������� ����� ������
void sum_mat(d_mat a, d_mat b, long n, d_mat(&c)) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			c[i][j] = a[i][j] + b[i][j];

}

//��������� ������������ ������� �� ������� (����������)
void mult_matXmat(d_mat a, d_mat b, long n, d_mat(&c)) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int h = 0; h < n; h++)
				c[i][j] += a[i][h] * b[h][j];

}

//��������� ���������������� ���������� ������� 
void mat_transp( d_mat a, long n, d_mat(&c)) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			c[j][i] = a[i][j];
}

//��������� ������� ��������� �������
void create_unit_mat(d_mat(&e), long(&n)) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i == j)
				e[i][j] = 1;
			else
				e[i][j] = 0;
}

//��������� ������� ��������� ������� ���� ��������� �������� � ������� 
int find_nozero_element_mat(d_mat(&mat), long n, int i, int j)
{
	for (int i1 = i + 1; i1 < n; i1++)
		if (mat[i1][j] != 0)
			return i1;
	return -1;
}

//��������� ������ ��� ������ (���� �� ������ ��� ����� ��� �����, ����� ������� ������ �� ��������)
void step_gause(d_mat(&mat), long n, int i, int j) {
	float k;
	for (int h = i + 1; h < n; h++)
		if (mat[h][j] != 0) {
			k = mat[h][j] / mat[i][j];
			for (int l = j; l < n; l++)
				mat[h][l] -= k * mat[i][l];
		}

}

//��������� ������ ������� �����������
void make_triangle_mat(d_mat(&mat), long n) {
	int i = 0, i1;
	for (int i = 0; i < n; i++) {
		//������� ���������� �������
		if (mat[i][i] != 0) {
			step_gause(mat, n, i, i);
		}
		else {
			//���� ���������� ������� 0, �� �������� �������� ��� ������ � �����-�� ������ ��� �� �� 0
			i1 = find_nozero_element_mat(mat, n, i, i);
			if (i1 != -1) {
				swap(mat, n, i, i1);
				step_gause(mat, n, i, i);
			}
		}
	}

}

//��������� �������� ������������  � ������������ �������
float count_det_A(d_mat(&mat), long n) {
	float det = 1;
	for (int i = 0; i < n; i++)
		det *= mat[i][i];
	return det;
}

//��������� ������� �������������� ����������� �� i ������ � j �������
void find_alg_dop(d_mat new_A, long n, int x, int y,  d_mat(&alg_dop)) {
	int  x1, y1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (x != i && y != j) {
				x1 = i;
				y1 = j;
				//���������� ������ ����������� ������� ��������� ������� � ������  
				if (i > x) x1--;
				if (j > y) y1--;
				alg_dop[x1][y1] = new_A[i][j];
			}
		}
}

//��������� ������� �������� ������� (���� ��� ����������)
void find_reverse_mat(d_mat new_A, long n, d_mat(&reverse_A)) {
	if (n > 1){
		double det_A;
		d_mat A = new float*[n], alg_dop = new float*[n - 1], reverse_A_T = new float*[n];
		create_mat(A, n);
		create_mat(reverse_A_T, n);
		create_mat(alg_dop, n - 1);
		mult_numXmat(new_A, n, 1, A);
		make_triangle_mat(A, n);
		det_A = count_det_A(A, n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				//������� ���������������  ���������� 
				find_alg_dop(new_A, n, i, j, alg_dop);
				//������� ��  ������������ � ����������� � ������ i,  j (���� ������� �� �������� (i+j))
				make_triangle_mat(alg_dop, n - 1);
				reverse_A_T[i][j] = count_det_A(alg_dop, n - 1) / det_A;
				if ((i + j) % 2 == 1)
					reverse_A_T[i][j] *= -1;
			}
		//���������� ��������������� ��� �������
		mat_transp(reverse_A_T, n, reverse_A);
		delete A;
		delete reverse_A_T;
		delete alg_dop;
	}
	else {
		reverse_A[0][0] = 1 / new_A[0][0];
	}
}

//��������� ���� ������������ ������� ������� ������ � ������ ��� � ����
void gause(FILE* fin, FILE* fout) {
	long  x, t;
	long n;
	fscanf(fin, "%i", &n);
	if (n > 0) {
		d_mat  A = new float*[n];
		create_mat(A, n);
		read_f_mat(fin, A, n);
		make_triangle_mat(A, n);
		fprintf(fout, "%f", count_det_A(A, n));
		delete A;
	}
	else 
	{
		fprintf(fout, "%s", "������ ������������ �������� n");
	}
}

//���������� ���� ���� ��� ����� ����� (� ������� ���� �����) ��� ������ ������� ������� X
int solve_mat_equation(d_mat A, d_mat C, long n, float b, d_mat(&X)) {
	d_mat  new_A = new float*[n], e = new float*[n];
	create_mat(e, n);
	create_mat(new_A, n);
	//��������� ��������� � ���� X*A'=C (�������� ���������) 
	create_unit_mat(e, n);
	mult_numXmat(e, n, b,  e);
	sum_mat(A, e, n, new_A);
	//��������� ���������� ����� ������� � �, ��� ��� ��� �������� ������������
	//��� ����� ������� ������� �����������, ��� ������ �������� 
	mult_numXmat(new_A, n, 1,  A);
	//���������, ��� ���� ������ ���������
	make_triangle_mat(new_A, n);
	if (count_det_A(new_A, n) == 0)
		return -1;
	else {
		//������� �������� �������
		d_mat reverse_A = new float*[n];
		create_mat(reverse_A, n);
		find_reverse_mat(A, n,  reverse_A);
		//�������� ������� �������
		mult_matXmat(C, reverse_A, n, X);
		delete reverse_A;
	}

	delete e;
	delete new_A;
	return 1;
}

//��������� ������� f(A)=2x^2-7x-1
void f_A(FILE* fout, d_mat(&a), long n,  d_mat(&b)) {
	d_mat e = new float*[n], minus_e = new float*[n], x1 = new float*[n], x2 = new float*[n], rez = new float*[n];
	create_mat(e, n);
	create_mat(minus_e, n);
	create_mat(x1, n);
	create_mat(x2, n);
	create_mat(rez, n);
	//������� ��������� ������� � ����� ���������
	create_unit_mat(e, n);
	mult_numXmat(e, n, -1,  minus_e);
	mult_matXmat(a, a, n, x2);
	//�^2
	write_f_mat(fout, x2, n);
	mult_numXmat(x2, n, 2,  x1);
	//2x^2
	write_f_mat(fout, x1, n);
	mult_numXmat(a, n, -7,  x2);
	//-7x
	write_f_mat(fout, x2, n);
	sum_mat(x1, x2, n, rez);
	//2x^2-7x
	write_f_mat(fout, rez, n);
	sum_mat(rez, minus_e, n, b);
	//2x^2-7x-1
	write_f_mat(fout, b, n);
	delete e;
	delete x1;
	delete x2;
	delete rez;
}

//��������� ������� g(A)=2*x^2-x
void g_A(FILE* fout, d_mat(&a), long n, d_mat(&b)) {
	d_mat  x1 = new float*[n], x2 = new float*[n];
	create_mat(x1, n);
	create_mat(x2, n);

	mult_matXmat(a, a, n, x2);
	//x^2
	write_f_mat(fout, x2, n);
	mult_numXmat(x2, n, 2,  x1);
	//2x^2
	write_f_mat(fout, x1, n);
	mult_numXmat(a, n, -1,  x2);
	//-x
	write_f_mat(fout, x2, n);
	sum_mat(x1, x2, n, b);
	//2x^2-x
	write_f_mat(fout, b, n);
	delete x1;
	delete x2;
}

//��������� ������� f(A)*g(A)
void f_A_x_g_A(FILE* fin, FILE* fout) {
	//����������� ������� ����
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	long  x, t;
	long n;
	fscanf(fin, "%i", &n);
	if (n > 0) {
		d_mat fA = new float*[n], gA = new float*[n], A = new float*[n], rez = new float*[n];
		create_mat(fA, n);
		create_mat(gA, n);
		create_mat(A, n);
		create_mat(rez, n);
		read_f_mat(fin, A, n);
		f_A(fout, A, n, fA);
		g_A(fout, A, n, gA);
		mult_matXmat(fA, gA, n, rez);
		write_f_mat(fout, rez, n);
		delete fA;
		delete gA;
		delete A;
		delete rez;
	}
	else
	{
		fprintf(fout, "%s", "������ ������������ �������� n");
	}
}

//��������� ������� ������ � ��������� ���� X * A + b * X = C
void matrix_equation(FILE* fin, FILE* fout) {
	//����������� ������� ����
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	long  n;
	float b;
	//������ ����������� ������� � ������� b �� ���������
	fscanf(fin, "%i", &n);
	fscanf(fin, "%i", &b);
	if (n > 0) {
		d_mat X = new float*[n], new_A = new float*[n], A = new float*[n], C = new float*[n], e = new float*[n];
		//������� ������� 
		create_mat(X, n);
		create_mat(A, n);
		create_mat(C, n);
		//������ ������� ������������ �������� ������������� 
		read_f_mat(fin, A, n);
		read_f_mat(fin, A, n);

		if (solve_mat_equation(A, C, n, b, X) == -1)
			fprintf(fout, "%s", "� ��������� ��������� ��� �������");
		else
			write_f_mat(fout, X, n);

		//������ �� ����� �������
		delete X;
		delete A;
		delete C;
	}
	else
	{
		fprintf(fout, "%s", "������ ������������ �������� n");
	}
}