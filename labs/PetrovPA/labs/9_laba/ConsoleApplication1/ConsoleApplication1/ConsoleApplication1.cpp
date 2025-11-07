#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

/**
 * Шаблонный класс SortVector — наследуется от std::vector<T>.
 * Реализует сортировку, вставку с сохранением порядка,
 * подсчёт кратных элементов и генерацию перестановок.
 */
template <typename T>
class SortVector : public std::vector<T> {
public:

    /**
     * Конструктор по умолчанию.
     * Создаёт пустой вектор.
     */
    SortVector() : std::vector<T>() {}

    /**
     * Конструктор с инициализатором.
     * @param l список значений для инициализации.
     * После создания элементы автоматически сортируются.
     */
    SortVector(initializer_list<T> l)
        : std::vector<T>(l)
    {
        std::sort(this->begin(), this->end());
    }
    
    /**
     * Подсчитывает количество элементов, кратных числу n.
     * @param n делитель для проверки кратности.
     * @return количество элементов, делящихся на n без остатка.
     */
    int checkIntegerDivision(int n) {
        auto lambda = [this, n]() {
            int k = 0;
            for (T i : *this) {
                if (i % n == 0) ++k;
            }
            return k;
            };
        return lambda();
    }

    /**
     * Вставляет элемент x в вектор с сохранением сортировки.
     * @param x вставляемое значение.
     */
    void insert(T x) {
        SortVector<T> newVec;
        bool inserted = false;

        for (size_t i = 0; i < this->size(); ++i) {
            if (!inserted && x <= (*this)[i]) {
                newVec.push_back(x);
                inserted = true;
            }
            newVec.push_back((*this)[i]);
        }

        if (!inserted)
            newVec.push_back(x);

        *this = newVec;
    }

    /**
     * Выводит все перестановки элементов вектора.
     * Использует рекурсивную функцию generatePermutations().
     */
    void printPermutations() {
        SortVector<T> temp = *this;
        generatePermutations(temp, 0);
    }

    /**
     * Рекурсивно генерирует все перестановки элементов.
     * @param arr вектор, для которого создаются перестановки.
     * @param index текущая позиция, начиная с которой переставляются элементы.
     */



    void generatePermutations(SortVector<T>& arr, int index) {
        if (index == arr.size() - 1) {
            for (T val : arr) cout << val << " ";
            cout << endl;
            return;
        }

        for (int i = index; i < arr.size(); ++i) {
            std::swap(arr[index], arr[i]);
            generatePermutations(arr, index + 1);
            std::swap(arr[index], arr[i]);
        }
    }

    /**
     * Оператор вывода для класса SortVector.
     * @param os поток вывода.
     * @param vec объект SortVector для печати.
     * @return ссылка на поток вывода.
     */
    template<typename U>
    friend ostream& operator<<(ostream& os, const SortVector<U>& vec);
};

/**
 * Реализация оператора вывода << для SortVector.
 */
template <typename T>
ostream& operator<<(ostream& os, const SortVector<T>& vec) {
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
    return os;
}

/**
 * Заполняет вектор случайными числами.
 * @param vec исходный вектор.
 * @return новый вектор, заполненный случайными значениями.
 */
template<typename T>
SortVector<T> fillVector(SortVector<T> vec) {
    int k;
    cout << "Колво элементов: ";
    cin >> k;
    for (int i = 0; i < k; ++i) {
        vec.push_back(rand() % 1000);
    }
    return vec;
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    SortVector<int> vec;
    SortVector<int> vec1 = { 123, 543, 2, 63427, 132, 523467 };

    cout << endl << "vec1: " << vec1 << endl;

    vec = fillVector(vec);
    cout << vec << endl;

    int y;
    cin >> y;
    cout << vec.checkIntegerDivision(y) << endl;

    cout << vec << endl;

    cin >> y;
    vec.insert(y);
    cout << vec << "\n";

    vec.printPermutations();
}

