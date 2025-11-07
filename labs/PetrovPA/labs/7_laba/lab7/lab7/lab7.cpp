#include <iostream>
#include <cstdlib>
using namespace std;

#define SIZE 5

/**
 * Шаблонный класс очереди с динамическим изменением размера.
 *
 * @tparam T тип элементов, хранящихся в очереди.
 */
template <typename T>
class MyQueue {
private:
    T* data;
    int beginIndx, endIndx, capacity;

    /**
     * Увеличивает размер массива в два раза при переполнении.
     *
     * Использует realloc для перераспределения памяти.
     */
    void newSize() {
        capacity *= 2;
        data = (T*)realloc(data, capacity * sizeof(T));
        if (!data) {
            exit(1);
        }
    }

public:
    /**
     * Конструктор по умолчанию.
     *
     * Выделяет память под очередь фиксированного размера SIZE.
     */
    MyQueue() {
        capacity = SIZE;
        data = (T*)malloc(capacity * sizeof(T));
        beginIndx = 0;
        endIndx = 0;
    }

    /**
     * Добавляет элемент в конец очереди.
     *
     * @param x значение, которое нужно вставить.
     */
    void pushBack(T x) {
        if (endIndx >= capacity)
            newSize();
        data[endIndx++] = x;
    }

    /**
     * Проверяет, пуста ли очередь.
     *
     * @return true — если очередь пуста, иначе false.
     */
    bool isEmpty() const { return beginIndx == endIndx; }

    /**
     * Возвращает количество элементов в очереди.
     *
     * @return число элементов в очереди.
     */
    int size() const { return endIndx - beginIndx; }
    

    void deleteFirst() {

        T* newData = (T*)malloc((endIndx - 1) * sizeof(T));
        for (int i = 1; i < endIndx; ++i)
            newData[i - 1] = data[i];

        free(data);
        data = newData;
        endIndx--;
    }

    /**
     * Деструктор.
     *
     * Освобождает динамически выделенную память.
     */
    ~MyQueue() { free(data); }

    /**
     * Вложенный класс итератора для поддержки цикла for-each.
     *
     * Позволяет обходить очередь от первого до последнего элемента.
     */
    class Iterator {
    private:
        T* ptr;
    public:
        /**
         * Конструктор итератора.
         *
         * @param p указатель на начальный элемент.
         */
        Iterator(T* p) : ptr(p) {}

        /**
         * Разыменование итератора.
         *
         * @return ссылка на текущий элемент.
         */
        T& operator*() const { return *ptr; }

        /**
         * Переход к следующему элементу.
         *
         * @return ссылка на текущий итератор.
         */
        Iterator& operator++() { ++ptr; return *this; }

        /**
         * Проверка неравенства двух итераторов.
         *
         * @param other другой итератор для сравнения.
         * @return true, если итераторы указывают на разные элементы.
         */
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }


    };

    /**
     * Возвращает итератор на первый элемент очереди.
     *
     * @return объект итератора, указывающий на начало данных.
     */
    Iterator begin() { return Iterator(data + beginIndx); }

    /**
     * Возвращает итератор на элемент, следующий за последним.
     *
     * @return объект итератора, указывающий на конец данных.
     */
    Iterator end() { return Iterator(data + endIndx); }




    template <typename U>
    friend ostream& operator<<(ostream& stream, const MyQueue<U>& que);

};




template<typename T>
ostream& operator<<(ostream& stream, const MyQueue<T>& que) {
    stream << "<MyQueue> { ";
    stream << "beginIndex: " << que.beginIndx
        << ", endIndex: " << que.endIndx
        << ", capacity: " << que.capacity
        << ", data: [";
    for (int i = que.beginIndx; i < que.endIndx; ++i) {
        stream << que.data[i];
        if (i < que.endIndx - 1)
            stream << ", ";
    }
    stream << "] }";
    return stream;
}


int main() {
    setlocale(LC_ALL, "RUS");

    MyQueue<int> q;
    q.pushBack(10);
    q.pushBack(20);
    q.pushBack(30);
    q.pushBack(40);

    cout << "Элементы очереди: ";
    for (auto el : q) {
        cout << el << " ";
    }
    cout << endl;

    q.deleteFirst();
    cout << "Элементы очереди: ";
    for (auto el : q) {
        cout << el << " ";
    }
    cout << endl;

    cout << q;
}
