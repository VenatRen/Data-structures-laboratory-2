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

void trans(float A[size_m][size_m])
{

    int i, j;

    float s;

#pragma loop(hint_parallel(8))

#pragma loop(no_vector)

    for (i = 0; i < size_m; i++)

#pragma loop(hint_parallel(8))

        for (j = i + 1; j < size_m; j++)
        {

            s = A[i][j];

            A[i][j] = A[j][i];

            A[j][i] = s;
        }
}

float Random()
{

    return Min + (rand() / (RAND_MAX / (Max - Min)));
}

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

void pmultiply_t(float a1[size_m][size_m], float a2[size_m][size_m], float a3[size_m][size_m])

{

    trans(a2);

#pragma loop(hint_parallel(8))

#pragma loop(no_vector)

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

int main()

{

    srand(time(0));

    setlocale(LC_ALL, "RUSSIAN");

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

    chrono::duration<float> long_time_1 = end - start;

    cout << "time prosto » " << long_time_1.count() << endl;

    // printt(M3);

    start = chrono::high_resolution_clock::now();

    pmultiply_t(M1, M2, M3);

    end = chrono::high_resolution_clock::now();

    chrono::duration<float> long_time_2 = end - start;

    long_time_2 = end - start;

    cout << "time with trans » " << long_time_2.count() << endl;

    // printt(M3);

    float p1, p2;

    p1 = (2 * size_m) * (size_m / long_time_1.count()) * (size_m * 0.000001);

    p2 = (2 * size_m) * (size_m / long_time_2.count()) * (size_m * 0.000001);

    cout << "Соотношение времени =" << (float)long_time_1.count() / long_time_2.count() << endl;

    cout << "Р прост. = " << p1 << endl;

    cout << "P улуч. = " << p2 << endl;

    cout << "Работу выполнил: Ставничий Даниил Сергеевич, ФИТУ РПИб" << endl;

    return 0;
}