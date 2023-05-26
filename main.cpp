#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>

using namespace std;

const int size_m = 1024;

const float Max = 100;

const float Min = 1;

float M1[size_m][size_m];

float M2[size_m][size_m];

float M3[size_m][size_m];
// Функция для транспонирования матрицы
void trans(float A[size_m][size_m])
{

    int i, j;

    float s;

#pragma loop(hint_parallel(8))

#pragma loop(no_vector)
// Цикл по матрице и замена местами элементов по диагонали
    for (i = 0; i < size_m; i++)

#pragma loop(hint_parallel(8))

        for (j = i + 1; j < size_m; j++)
        {

            s = A[i][j];

            A[i][j] = A[j][i];

            A[j][i] = s;
        }
}
// Функция для генерации случайного значения с плавающей точкой между Min и Max
float Random()
{

    return Min + (rand() / (RAND_MAX / (Max - Min)));
}
// Функция для печати матрицы
void print_m(float **A)

{

#pragma loop(no_vector)

#pragma loop(hint_parallel(8))

    for (int i = 0; i < size_m; i++)
    {

#pragma loop(hint_parallel(8))

        for (int j = 0; j < size_m; j++)
        {

            cout << A[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;
}
// Функция для умножения двух матриц без транспонирования
void pmultiply_p(float a1[size_m][size_m], float a2[size_m][size_m], float a3[size_m][size_m])

{

    for (int i = 0; i < size_m; i++)

    {

        for (int j = 0; j < size_m; j++)

        {

            a3[i][j] = 0;

            for (int k = 0; k < size_m; k++)

                a3[i][j] += a1[i][k] * a2[k][j];
        }
    }
}
// Функция для умножения двух матриц с транспонированием
void pmultiply_t(float a1[size_m][size_m], float a2[size_m][size_m], float a3[size_m][size_m])

{
    // Транспонировать вторую матрицу
    trans(a2);

#pragma loop(hint_parallel(8))

#pragma loop(no_vector)
    // Перебор матрицы и умножанеие элементов
    for (int i = 0; i < size_m; i++)

    {

#pragma loop(hint_parallel(8))

        for (int j = 0; j < size_m; j++)

        {

            a3[i][j] = 0;

#pragma loop(hint_parallel(8))

            for (int k = 0; k < size_m; k++)

            {

                a3[i][j] += a1[i][k] * a2[j][k];
            }
        }
    }
}
// Функция для печати матрицы
void printt(float A[size_m][size_m])

{

    for (int i = 0; i < size_m; i++)
    {

        for (int j = 0; j < size_m; j++)
        {

            cout << A[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;
}
// Основная функция
int main()

{
    // Запуск генератора случайных чисел
    srand(time(0));

    setlocale(LC_ALL, "RUSSIAN");
    // Инициализируем матрицы случайными значениями
    for (int i = 0; i < size_m; i++)
    {

        for (int j = 0; j < size_m; j++)
        {

            M1[i][j] = Random();
        }
    }

    for (int i = 0; i < size_m; i++)
    {

        for (int j = 0; j < size_m; j++)
        {

            M2[i][j] = Random();
        }
    }

    auto start = chrono::high_resolution_clock::now();

    // pmultiply_p(M1, M2, M3);

    auto end = chrono::high_resolution_clock::now();
    // Вычисление времени, затраченное на умножение матриц без транспонирования
    chrono::duration<float> long_time_1 = end - start;

    cout << "time prosto » " << long_time_1.count() << endl;

    // printt(M3);
    // Умножение матрицы с помощью транспонирования
    start = chrono::high_resolution_clock::now();

    pmultiply_t(M1, M2, M3);

    end = chrono::high_resolution_clock::now();
    // Вычисление времени, затраченное на умножение матриц с помощью транспонирования
    chrono::duration<float> long_time_2 = end - start;

    long_time_2 = end - start;

    cout << "time with trans » " << long_time_2.count() << endl;

    // printt(M3);
    // Расчёт соотношения производительности двух методов
    float p1, p2;

    p1 = (2 * size_m) * (size_m / long_time_1.count()) * (size_m * 0.000001);

    p2 = (2 * size_m) * (size_m / long_time_2.count()) * (size_m * 0.000001);

    cout << "Соотношение времени =" << (float)long_time_1.count() / long_time_2.count() << endl;

    cout << "Р прост. = " << p1 << endl;

    cout << "P улуч. = " << p2 << endl;

    cout << "Работу выполнил: Ставничий Даниил Сергеевич, ФИТУ РПИб" << endl;

    return 0;
}
