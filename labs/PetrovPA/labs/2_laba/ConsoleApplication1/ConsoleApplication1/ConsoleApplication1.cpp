#include <iostream>
#include <ctime>
using namespace std;

/*
 * Сортировка массива (разбиение)
 *
 * @param arr указатель на массив.
 * @param min минимальный индекс.
 * @param max максимальный индекс.
 * @return возвращает индекс разделения для рекурсии.
 */
int swapArr(double* arr, int min, int max) {
    double pivot = arr[(min + max) / 2];
    int i = min, j = max;

    while (i <= j) {
        while (arr[i] < pivot) ++i;
        while (arr[j] > pivot) --j;
        if (i <= j) {
            swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    return i;
}

/*
 * Быстрая сортировка массива
 *
 * @param arr указатель на массив.
 * @param min минимальный индекс.
 * @param max максимальный индекс.
 */
void sortirovka(double* arr, int min, int max) {
    if (min >= max) return;
    int mid = swapArr(arr, min, max);
    sortirovka(arr, min, mid - 1);
    sortirovka(arr, mid, max);
}

/*
 * Заполнение массива случайными числами
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 */
void fillArray(double* arr, int size) {
    for (int i = 0; i < size; ++i) arr[i] = 20 + (20000 - 20) * rand() / double(RAND_MAX);
}

/*
 * Вывод массива на экран
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 */
void printArray(double* arr, int size) {
    for (int i = 0; i < size; ++i) printf("%d: %f\n", i + 1, arr[i]);
}

/*
 * Поиск максимального значения в массиве
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 * @return максимальное значение.
 */
double findMax(double* arr, int size) {
    double max = arr[0];
    for (int i = 1; i < size; ++i) if (max < arr[i]) max = arr[i];
    return max;
}

/*
 * Поиск минимального значения в массиве
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 * @return минимальное значение.
 */
double findMin(double* arr, int size) {
    double min = arr[0];
    for (int i = 1; i < size; ++i) if (min > arr[i]) min = arr[i];
    return min;
}

/*
 * Вычисление среднего арифметического
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 * @return среднее значение.
 */
double avg(double* arr, int size) {
    double sum = 0;
    for (int i = 0; i < size; ++i) sum += arr[i];
    return sum / size;
}

/*
 * Вычисление медианы
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 * @return медианное значение.
 */
double median(double* arr, int size) {
    return (size % 2 != 0) ? arr[(size - 1) / 2] : (arr[size / 2 - 1] + arr[size / 2]) / 2.0;
}

/*
 * Создание нового массива, умноженного на индекс
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 * @return указатель на новый массив.
 */
double* newArray(double* arr, int size) {
    double* arr2 = new double[size];
    for (int i = 0; i < size; ++i) arr2[i] = arr[i] * i;
    return arr2;
}

/*
 * Изменение массива через ссылку на массив размера 5
 *
 * @param arr ссылка на массив из 5 элементов.
 */
void arrayReference(double(&arr)[5]) {
    for (int i = 0; i < 5; ++i) arr[i] += 100;
}

/*
 * Изменение массива через указатель
 *
 * @param arr указатель на массив.
 * @param size размер массива.
 */
void arrayPointer(double* arr, int size) {
    for (int i = 0; i < size; ++i) arr[i] += 100;
}

/*
 * Вывод максимума и минимума
 */
void action1(double* arr, int n) {
    cout << "Max: " << findMax(arr, n) << " | Min: " << findMin(arr, n) << endl;
}

/*
 * Среднее арифметическое и медиана
 */
void action2(double* arr, int n) {
    cout << "Среднее арифметическое: " << avg(arr, n) << endl;
    cout << "Медиана: " << median(arr, n) << endl;
}

/*
 * Создание нового массива и вывод
 */
void action3(double* arr, int n) {
    double* arr2 = newArray(arr, n);
    cout << "Новый массив:\n";
    printArray(arr2, n);
    delete[] arr2;
}

/*
 * Передачи массива по указателю и по ссылке
 */
void action4() {
    cout << "\nПередача массива по указателю:\n";
    double* arrPtr = new double[5] {1, 2, 3, 4, 5};
    arrayPointer(arrPtr, 5);
    printArray(arrPtr, 5);
    delete[] arrPtr;

    cout << "\nПередача массива по ссылке:\n";
    double arrRef[5]{ 1,2,3,4,5 };
    arrayReference(arrRef);
    printArray(arrRef, 5);
}

/*
 * Меню выбора действия
 */
void menu() {
    cout << "\n0. Выход\n";
    cout << "1. Найти самую низкую и самую высокую частоту.\n";
    cout << "2. Вычислить среднее арифметическое и медиану.\n";
    cout << "3. Создать новый массив, содержащий частоты, умноженные на коэффициент.\n";
    cout << "4. Сравнить передачу массива в функцию по ссылке и по указателю.\n";
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "RUS");

    int n;
    cout << "Значение N: ";
    cin >> n;

    double* arr = new double[n];
    fillArray(arr, n);
    sortirovka(arr, 0, n - 1);

    int choice = 0;
    while (true) {
        system("cls");
        cout << "Исходный массив:\n";
        printArray(arr, n);
        menu();
        cin >> choice;

        switch (choice) {
        case 0: exit(0);
        case 1: action1(arr, n); break;
        case 2: action2(arr, n); break;
        case 3: action3(arr, n); break;
        case 4: action4(); break;
        default: break;
        }
        cin.ignore();
        cin.get();
    }

    delete[] arr;
}
