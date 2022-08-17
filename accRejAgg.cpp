#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <unistd.h>


constexpr int arrSize = 1024;

inline double kernel (int i, int j)
{
    return i * j;
}

inline int min(int a, int b)
{
    if (a < b)
        return a;

    return b;
}

inline int max(int a, int b)
{
    if (a < b)
        return b;

    return a;
}


int main()

{
    int* sizes = new int [arrSize + 1];
    for (int it = 0; it < arrSize; it++)
        sizes[it] = 1;

    int i, j, k, l;
    i = j = k = l = 0;
    int currentMaxSize = 1;
    double maxTime = 2 * log(2);
    double doubleTime = 0.0;
    double currentTime = 0.0;
    double kernelMax = kernel(currentMaxSize, currentMaxSize);
    int volume = arrSize;
    int N = arrSize;

    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_real_distribution<> floatDist(-1, 0);
    // std::uniform_int_distribution<> intDist(1, N);
    int resizeFactor = 1;

    srand(time(NULL));

    auto start = std::chrono::high_resolution_clock::now();
    while(currentTime < maxTime)
    {
        // printf("time = %lf\n", currentTime);
        // printf("volume = %d, N = %d\n", volume, N);
        do
        {
            currentTime += 2.0 * volume / (kernelMax * N * (N - 1));

            k = (rand() % N); // k = intDist(gen) * resizeFactor;
            do
            {
                l = (rand() % N); // l = intDist(gen) * resizeFactor;
            } while(l == k);
            //printf("k = %d l = %d, kernel(i, j) = %lf, time = %lf\n", k, l, kernel(sizes[k], sizes[l]), currentTime);
            i = sizes[k];
            j = sizes[l];
        } while ((kernelMax * (rand() + 1.0) / RAND_MAX) > kernel(i, j) );
        // while (kernelMax * (-1 * floatDist(gen)) > kernel(i, j));
        // printf("time after = %lf\n", currentTime);


        int minIndex = min(k, l);
        int maxIndex = max(k, l);

        sizes[minIndex] += sizes[maxIndex];
        sizes[maxIndex] = sizes[N];
        sizes[N] = 1;
        N--;

        if (N <= arrSize / 2)
        {
            for (int it = 0; it < N; it++)
                sizes[N + it] = sizes[it];

            // std::cout << "N = " << N << " arrsize = " << arrSize << " updated volume" << std::endl;
            N *= 2;
            volume *= 2;
            sleep(1);
            resizeFactor *= 2;
            doubleTime = currentTime;
        }

        if (sizes[minIndex] > currentMaxSize)
        {
            currentMaxSize = sizes[minIndex];
            kernelMax = kernel(currentMaxSize, currentMaxSize);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "kmax = " << currentMaxSize << " currentTime = " << currentTime << " volume = " << volume
              << " worked for " << duration.count()  << " double time = " << doubleTime << " Moment 0 " << N << std::endl;

    delete [] sizes;
}