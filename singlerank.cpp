#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>

#include "segment_tree.hpp"


constexpr int arrSize = 1024 * 1024;

inline double kernel (int i, int j)
{
    return i * j;
}

inline double updateU_I (double* uVec, int index, int numOfClasters)
{
    return uVec[index] = numOfClasters * index;
}

long* initSizes()
{
    long* sizes = (long*)calloc(arrSize + 1, sizeof(long));
    sizes[1] = arrSize;

    return sizes;
}

double* inituVec()
{
    double* uVec = (double*)calloc(arrSize + 1, sizeof(double));
    uVec[1] = arrSize;

    return uVec;
}

int main()

{
    long* sizes = initSizes();
    double* uVec = inituVec();

    long i, j;
    i = j = -1;
    long currentMaxSize = 1;
    double maxTime = 2 * log(2);
    double doubleTime = 0.0;
    double currentTime = 0.0;
    // double kernelMax = kernel(currentMaxSize, currentMaxSize);
    long volume = arrSize;
    long N = arrSize;
    long maxSize = N;

    srand(time(NULL));

    double* tree = segmentTreeCtor(uVec, arrSize);

    std::ofstream output;
    output.open("first_task.txt");
    if (!output)
        throw 1;

    double delta = maxTime / 30;
    double ifToGetPoint = delta;

    auto start = std::chrono::high_resolution_clock::now();
    while (currentTime < maxTime)
    {
        // printf("time = %lf\n", currentTime);
        currentTime += 2.0 * volume / (tree[1] * tree[1]);
        ifToGetPoint -= 2.0 * volume / (tree[1] * tree[1]);

        if (ifToGetPoint < 0)
        {
            double sum = 0.0;
            for (int i = 1; i < N; i++)
                sum += i * sizes[i];

            output << (currentMaxSize / sum) << " " << currentTime << std::endl;

            ifToGetPoint = delta;
        }

        do
        {
            i = find(tree, tree [1] * (rand() + 1.0) / RAND_MAX, maxSize); // bad!!
            j = find(tree, tree [1] * (rand() + 1.0) / RAND_MAX, maxSize); // bad!!

            // printf("finding i = %d, j = %d\n", i, j);

            if ((i == j) && ((sizes[i] * (rand() + 1.0) / RAND_MAX) <= 1))
            {
                // printf("continuing\n");
                i = j = -1;
                continue;
            }
        } while((i == -1) && (j == -1));

        if (i + j > currentMaxSize)
            currentMaxSize = i + j;

        if (i + j > maxSize)
        {
            int oldSize = maxSize;
            maxSize *= 2;
            sizes = (long *)realloc(sizes, maxSize * sizeof(long));
            uVec = (double *)realloc(uVec, maxSize * sizeof(double));
            for (int it = 0; it < oldSize; it++)
            {
                sizes[it + oldSize] = 0;
                uVec[it + oldSize] = 0;
            }

            segmentTreeDtor(tree);
            tree = segmentTreeCtor(uVec, maxSize);
            // throw "cry baby";
        }

        sizes[i] -= 1;
        sizes[j] -= 1;
        sizes[i + j] += 1;
        N--;

        updateU_I(uVec, i, sizes[i]);

        update(tree, i, (sizes[i]) * i, maxSize);
        update(tree, j, (sizes[j]) * j, maxSize);
        update(tree, i + j, (sizes[i + j]) * (i + j), maxSize);

        if (N <= arrSize / 2)
        {
            doubleTime = currentTime;
            N *= 2;
            for (int i = 0; i < maxSize; i++)
            {
                sizes[i] *= 2;
                uVec[i] *= 2;
            }
            volume *= 2;
        }

        i = j = -1;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "kmax = " << currentMaxSize << " currentTime = " << currentTime << " volume = " << volume
              << " worked for " << duration.count()  << " double time = " << doubleTime << " Moment_0: " << N << std::endl;

    free(uVec);
    free(sizes);
    output.close();
}