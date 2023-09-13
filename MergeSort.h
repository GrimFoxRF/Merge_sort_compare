#pragma once
#include <iostream>
#include <vector>
#include "ThreadPool.h"

void largeVectorFill(std::vector<int>& largeVector, long long size);//заполнение вектора элементами

void merge(std::vector<int>& array, int left, int middle, int right);//слияние подмассивов в один
void mergeSortInOneThread(std::vector<int>& array, int left, int right);//сортировка в одном потоке
void mergeSortInManyThreads(std::vector<int>& array, int left, int right, ThreadPool& pool); //int& maxThreads,int& activeThreads);//многопоточная сортировка

int coreNumber();//получение числа ядер системы
void calculateRatio(double t1, double t2);//разница в скоросте между разными методами
void printArray(const std::vector<int>& array);//вывод содержимого массива в консоль (лучше не использовать с огромными массивами)