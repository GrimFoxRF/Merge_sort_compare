//алгоритм сортировки массива в одном потоке и в нескольких потоках
#include <Windows.h>
#include <ctime>
#include "MergeSort.h"
#include "ThreadPool.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	int numCores = coreNumber();//количество ядер CPU
	std::cout << "\nКоличество ядер CPU: " << numCores << std::endl;
	int maxThreads = numCores;//устанавливает максимальное число потоков
	int activeThreads = 0;//счетчик активных потоков

	std::time_t start1 = 0;
	std::time_t	end1 = 0;
	double seconds1 = 0;

	std::vector<int> largeVector1; //создаем первый большой вектор
	std::vector<int> largeVector2; //создаем второй большой вектор

	const long long size = 5'000'000;//размер вектора

	largeVectorFill(largeVector1, size); //заполняем вектор 1 случайными числами от 0 до 100
	largeVectorFill(largeVector2, size); //заполняем вектор 2 случайными числами от 0 до 100

	std::time(&start1);
	mergeSortInOneThread(largeVector1, 0, largeVector1.size() - 1);//сортировка в одном потоке
	std::time(&end1);
	seconds1 = std::difftime(end1, start1);
	std::cout << "\nВремя выполнения сортировки в одном потоке: " << seconds1 << " секунд" << std::endl;

	std::time_t start2 = 0;
	std::time_t end2 = 0;
	double seconds2 = 0;

	ThreadPool pool; // Создаем пул потоков

	std::time(&start2);
	mergeSortInManyThreads(largeVector2, 0, largeVector2.size() - 1, pool); // maxThreads,activeThreads);//многопоточная сортировка
	std::time(&end2);
	
	seconds2 = std::difftime(end2, start2);
	std::cout << "Время выполнения сортировки в " << maxThreads << " потоках: " << seconds2 << " секунд" << std::endl;
	//printArray(largeVector2); //тест для вывода всего массива на экран, не использовать с огромными массивами

	calculateRatio(seconds1, seconds2);//во сколько раз один метод быстрее другого

	return 0;
}