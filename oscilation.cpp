#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "unistd.h"

#include "segmentTree.hpp"

constexpr int INDEX_POISON = -1;

constexpr int arrSize = 1024 * 32;

inline double kernel (int i, int j)
{
    return 2 * std::pow(i, 0.98);
}

inline double updateU_I (double* uVec, int index, int numOfClasters)
{
    return numOfClasters * index;// * 2 * std::pow(index, 0.98);
}

inline double updateV_I (double* uVec, int index, int numOfClasters)
{
    return numOfClasters * index;
}

int* initSizes()
{
    int* sizes = (int*)calloc(arrSize + 1, sizeof(int));
    sizes[1] = arrSize;

    return sizes;
}

double* inituVec()
{
    double* uVec = (double*)calloc(arrSize + 1, sizeof(double));
    uVec[1] = arrSize; //* 2;

    return uVec;
}

double* initvVec()
{
    double* vVec = (double*)calloc(arrSize + 1, sizeof(double));
    vVec[1] = arrSize;

    return vVec;
}


int main()
{
    int* sizes = initSizes();
    double* uVec = inituVec();
    double* vVec = initvVec();

    int i, j;
    i = j = INDEX_POISON;
    int currentMaxSize = 1;
    double maxTime = 16;
    double currentTime = 0.0;
    double lambda = 0.005;
    // double kernelMax = kernel(currentMaxSize, currentMaxSize);
    int volume = arrSize;
    int N = arrSize;
    int maxSize = N;

    srand(time(NULL));

    double* uTree = segmentTreeCtor(uVec, arrSize);
    double* vTree = segmentTreeCtor(vVec, arrSize);

    std::ofstream output;
    output.open("oscilation.txt");
    if (!output)
        throw 1;

    double delta = 0.01;
    double ifToGetPoint = 0;

    while (currentTime < maxTime)
    {
        do
        {
        // printf("time = %lf\n", currentTime);
            currentTime += 2.0 * volume * (1 + lambda)/ (uTree[1] * vTree[1]);
            ifToGetPoint -= 2.0 * volume / (uTree[1] * vTree[1]);

            if (ifToGetPoint <= 0)
            {
                printf("a second passed: time = %lf\n", currentTime);
                double sum = 0.0;
                for (int i = 1; i < maxSize; i++)
                    // sum += i * i * (sizes[i] + 0.0) / volume;
                    sum += i * (sizes[i] + 0.0);


                output << currentTime << " " << (currentMaxSize + 0.0) / sum << std::endl;

                ifToGetPoint = delta;
            }

            i = find(uTree, uTree [1] * (rand() + 1.0) / RAND_MAX, maxSize);
            j = find(vTree, vTree [1] * (rand() + 1.0) / RAND_MAX, maxSize);

            if ((i == j) && (sizes[i] * (rand() + 1.0) / RAND_MAX <= 1))
            {
                i = j = INDEX_POISON;
                continue;
            }

        } while ((i == INDEX_POISON) && (j == INDEX_POISON));

        double r = (rand() + 1.0) / RAND_MAX;

        if ( (r > (lambda / (lambda + 1))) || ((i == j) && (i == 1)) )
        {
            if (i + j > currentMaxSize)
                currentMaxSize = i + j;

            if (i + j > maxSize)
            {
                int oldSize = maxSize;
                maxSize *= 2;
                sizes = (int *)realloc(sizes, maxSize * sizeof(int));
                uVec = (double *)realloc(uVec, maxSize * sizeof(double));
                vVec = (double *)realloc(vVec, maxSize * sizeof(double));
                for (int it = 0; it < oldSize; it++)
                {
                    sizes[it + oldSize] = 0;
                    uVec[it + oldSize] = 0;
                    vVec[it + oldSize] = 0;
                }

                segmentTreeDtor(vTree);
                segmentTreeDtor(uTree);
                uTree = segmentTreeCtor(uVec, maxSize);
                vTree = segmentTreeCtor(vVec, maxSize);
                // throw "cry baby";
            }

            sizes[i] -= 1;
            sizes[j] -= 1;
            sizes[i + j] += 1;

            update(uTree, i + j, updateU_I(uVec, i + j, sizes[i + j]), maxSize);
            update(vTree, i + j, updateV_I(vVec, i + j, sizes[i + j]), maxSize);

        }
        #if 1
        else
        {
            // printf("FRAGMENT!!!: i = %d, j = %d\n", i, j);
            // printf("before: sizes(1): %d, sizes(i) = %d, sizes(j) = %d\n", sizes[1], sizes[i], sizes[j]);
            // sleep(1);

            sizes[1] += (i + j - 2);
            N += (i + j - 2);
            if (i != 1)
                sizes[i] = 1;
            if (j != 1)
            sizes[j] = 1;

            // printf("now: sizes(1): %d, sizes(i) = %d, sizes(j) = %d\n", sizes[1], sizes[i], sizes[j]);
            // sleep(1);

            update(uTree, 1, updateU_I(uVec, 1, sizes[1]), maxSize);
            update(vTree, 1, updateV_I(vVec, 1, sizes[1]), maxSize);

            // printf("uTree[1] = %lf, vTree[1] = %lf\n", uTree[1], vTree[1]);
        }
        #endif

        update(uTree, i, updateU_I(uVec, i, sizes[i]), maxSize);
        update(vTree, i, updateV_I(vVec, i, sizes[i]), maxSize);
        update(uTree, j, updateU_I(uVec, j, sizes[j]), maxSize);
        update(vTree, j, updateV_I(vVec, j, sizes[j]), maxSize);

        if (N <= arrSize / 2)
        {
            N *= 2;
            for (int i = 0; i < maxSize; i++)
            {
                sizes[i] *= 2;
                uVec[i] *= 2;
                vVec[i] *= 2;
            }
            volume *= 2;
        }

        i = j = INDEX_POISON;

    }

        std::cout << "kmax = " << currentMaxSize << " currentTime = " << currentTime << " volume = " << volume << std::endl;

    output.close();

}