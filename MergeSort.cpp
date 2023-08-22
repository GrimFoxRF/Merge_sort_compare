#include "MergeSort.h"
#include <cstdlib>
#include <future>
#include <mutex>

std::mutex activeThreadsMutex;//мьютекс для переменной активных потоков
//заполнение вектора элементами
void largeVectorFill(std::vector<int>& largeVector, long long size)
{
	largeVector.reserve(size);//резервируем память под вектор

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	std::cout << "\nЗаполняем массив случайными числами" << std::endl;

	for (int i = 0; i < size; i++) 
	{
		largeVector.push_back(std::rand()%101);
	}

	std::cout << "Массив размером [" << size << "] заполнен случайными числами" << std::endl;
}
//слияние подмассивов в один
void merge(std::vector<int>& array, int left, int middle, int right)
{
    int sizeLeft = middle - left + 1;
    int sizeRight = right - middle;

    std::vector<int> leftArray(sizeLeft);
    std::vector<int> rightArray(sizeRight);

    for (int i = 0; i < sizeLeft; ++i)
        leftArray[i] = array[left + i];
    for (int j = 0; j < sizeRight; ++j)
        rightArray[j] = array[middle + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < sizeLeft && j < sizeRight) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            ++i;
        }
        else {
            array[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < sizeLeft) {
        array[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < sizeRight) {
        array[k] = rightArray[j];
        ++j;
        ++k;
    }
}
//сортировка в одном потоке
void mergeSortInOneThread(std::vector<int>& array, int left, int right)
{
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSortInOneThread(array, left, middle);
        mergeSortInOneThread(array, middle + 1, right);

        merge(array, left, middle, right);
    }
}
//многопоточная сортировка
void mergeSortInManyThreads(std::vector<int>& array, int left, int right, int& maxThreads, int& activeThreads)
{
    if (left < right) {
        int middle = left + (right - left) / 2;

        std::unique_lock<std::mutex> mutexLock(activeThreadsMutex);
        if (activeThreads < maxThreads) {
            ++activeThreads;
            mutexLock.unlock();

            std::future<void> leftFuture = std::async(std::launch::async, [&] {
                mergeSortInManyThreads(array, left, middle, maxThreads, activeThreads);
                });
            std::future<void> rightFuture = std::async(std::launch::async, [&] {
                mergeSortInManyThreads(array, middle + 1, right, maxThreads, activeThreads);
                });

            leftFuture.get();
            rightFuture.get();

            mutexLock.lock();
            --activeThreads;
        }
        else {
            mutexLock.unlock();

            mergeSortInManyThreads(array, left, middle, maxThreads, activeThreads);
            mergeSortInManyThreads(array, middle + 1, right, maxThreads, activeThreads);
        }

        merge(array, left, middle, right);
    }
}
//получение числа ядер системы
int coreNumber()
{
    std::cout << "\nВычисляем количество ядер CPU..." << std::endl;
    int numCores = std::thread::hardware_concurrency();
    if (numCores > 0) {
        
        return numCores;
    }
    else {
        std::cout << "\nОШИБКА: вычисления количества ядер CPU\n" << std::endl;
        return 0;
    }
}
//разница в скоросте между разными методами
void calculateRatio(double t1, double t2)
{
    if (t1 < t2)
    {
        std::cout << "\nОднопоточная сортировка в " << t2 / t1 << " раз быстрее многопоточной" << std::endl;
    }
    else 
    {
        std::cout << "\nМногопоточная сортировка в " << t1 / t2 << " раз быстрее однопоточной" << std::endl;
    }
}
//вывод содержимого массива в консоль (лучше не использовать с огромными массивами)
void printArray(const std::vector<int>& array)
{
    for (const int& num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
