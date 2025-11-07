#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

/*
 * Перечисление типов животных.
 *

 */
enum AnimalType { 
    CAT, 
    DOG,
    WOMBAT, 
    RAT 
};

/*
 * Класс Cat — описывает кота.
 *
 */
class Cat {
    int sleepDuration = 6;
    int foodCycle = 3;
public:
    /*
     * Метод взаимодействия кота.
     */
    void interact() { cout << "Кот взаимодействует\n"; }

    /*
     * Перегрузка оператора вывода для отображения информации о коте.
     *
     * @param stream поток вывода.
     * @param cat объект кота.
     * @return поток с информацией о коте.
     */
    friend ostream& operator<<(ostream& stream, const Cat& cat);
};
ostream& operator<<(ostream& stream, const Cat& cat) {
    stream << "<Cat> {" << "sleepDuration: " << cat.sleepDuration << "; foodCycle: " << cat.foodCycle << "} ";
    return stream;
}

/*
 * Класс Dog — описывает собаку.
 */
class Dog {
    int sleepDuration = 8;
    int foodCycle = 2;
public:
    /*
     * Метод взаимодействия собаки.
     */
    void interact() { cout << "Собака взаимодействует\n"; }

    /*
     * Перегрузка оператора вывода для отображения информации о собаке.
     *
     * @param stream поток вывода.
     * @param dog объект собаки.
     * @return поток с информацией о собаке.
     */
    friend ostream& operator<<(ostream& stream, const Dog& dog);
};
ostream& operator<<(ostream& stream, const Dog& dog) {
    stream << "<Dog> {" << "sleepDuration: " << dog.sleepDuration << "; foodCycle: " << dog.foodCycle << "} ";
    return stream;
}

/*
 * Класс Wombat — описывает вомбата.
 *
 */
class Wombat {
    int sleepDuration = 10;
    int foodCycle = 4;
public:
    /*
     * Метод взаимодействия вомбата.
     */
    void interact() { cout << "Вомбат взаимодействует\n"; }

    /*
     * Перегрузка оператора вывода для отображения информации о вомбате.
     *
     * @param stream поток вывода.
     * @param wombat объект вомбата.
     * @return поток с информацией о вомбате.
     */
    friend ostream& operator<<(ostream& stream, const Wombat& wombat);
};
ostream& operator<<(ostream& stream, const Wombat& wombat) {
    stream << "<Wombat> {" << "sleepDuration: " << wombat.sleepDuration << "; foodCycle: " << wombat.foodCycle << "} ";
    return stream;
}

/*
 * Класс Rat — описывает крысу.
 *
 */
class Rat {
    int sleepDuration = 5;
    int foodCycle = 2;
public:
    /*
     * Метод взаимодействия крысы.
     */
    void interact() { cout << "Крыса взаимодействует\n"; }

    /*
     * Перегрузка оператора вывода для отображения информации о крысе.
     *
     * @param stream поток вывода.
     * @param rat объект крысы.
     * @return поток с информацией о крысе.
     */
    friend ostream& operator<<(ostream& stream, const Rat& rat);
};
ostream& operator<<(ostream& stream, const Rat& rat) {
    stream << "<Rat> {" << "sleepDuration: " << rat.sleepDuration << "; foodCycle: " << rat.foodCycle << "} ";
    return stream;
}

/*
 * Класс Animal — универсальное животное, связывает тип с конкретным объектом.
 *
 */
class Animal {
private:
    void* specific;
    AnimalType type;
    string name;
    int age;
    double satiety;
    double fatigue;
    bool busy;

public:
    /*
     * Конструктор животного.
     *
     * @param t тип животного.
     * @param data указатель на объект животного.
     * @param n имя животного.
     * @param a возраст животного.
     */
    Animal(AnimalType t, void* data, string n, int a)
        : type(t), specific(data), name(n), age(a), satiety(100), fatigue(0), busy(false) {}

    /*
     * Получить имя животного.
     *
     * @return строка с именем.
     */
    string getName() { return name; }

    /*
     * Получить тип животного.
     *
     * @return значение enum AnimalType.
     */
    AnimalType getType() { return type; }

    /*
     * Животное играет с посетителями.
     *
     * @param intensity интенсивность взаимодействия (0–1).
     */
    void playWithVisitors(double intensity) {
        if (busy) return;
        cout << name << " играет с посетителями.\n";
        fatigue += 20 * intensity;
        if (fatigue > 100) fatigue = 100;
        satiety -= 10 * intensity;
        if (satiety < 0) satiety = 0;
    }

    /*
     * Животное ест.
     */
    void eat() {
        if (busy) return;
        cout << name << " кушает.\n";
        satiety = 100;
    }

    /*
     * Животное спит.
     */
    void sleep() {
        if (busy) return;
        cout << name << " спит.\n";
        fatigue = 0;
    }

    /*
     * Проверка необходимости сна.
     *
     * @return true, если усталость > 80.
     */
    bool checkSleep() { return fatigue > 80; }

    /*
     * Проверка необходимости еды.
     *
     * @return true, если сытость < 20.
     */
    bool checkEat() { return satiety < 20; }

    /*
     * Установить занятость животного.
     *
     * @param b true — занято, false — свободно.
     */
    void setBusy(bool b) { busy = b; }

    /*
     * Проверка занятости животного.
     *
     * @return true, если занято.
     */
    bool isBusy() { return busy; }

    /*
     * Взаимодействие двух животных одного типа.
     *
     * @param other другое животное.
     */
    void interaction(Animal& other) {
        if (isBusy() || other.isBusy()) return;
        setBusy(true);
        other.setBusy(true);
        cout << name << " взаимодействует с " << other.name << ": ";
        switch (type) {
        case CAT: ((Cat*)specific)->interact(); break;
        case DOG: ((Dog*)specific)->interact(); break;
        case WOMBAT: ((Wombat*)specific)->interact(); break;
        case RAT: ((Rat*)specific)->interact(); break;
        }
    }

    /*
     * Перегрузка оператора вывода для отображения информации о животном.
     *
     * @param stream поток вывода.
     * @param animal объект животного.
     * @return поток с информацией о животном.
     */
    friend ostream& operator<<(ostream& stream, const Animal& animal);
};
ostream& operator<<(ostream& stream, const Animal& animal) {
    stream << "<Animal> {" << "specific: " << animal.specific << "; type: " << animal.type
        << "; name: " << animal.name << "; age: " << animal.age
        << "; satiety: " << animal.satiety << "; fatigue: " << animal.fatigue
        << "; busy: " << animal.busy;
    return stream;
}

/*
 * Класс Zoo — зоопарк, содержащий массив животных.
 *
 */
class Zoo {
private:
    vector<Animal> animals;

public:
    /*
     * Добавить животное в зоопарк.
     *
     * @param a объект Animal.
     */
    void addAnimal(const Animal& a) {
        animals.push_back(a);
    }

    /*
     * Проверить состояние животных: кормление и сон.
     */
    void checkAnimals() {
        for (auto& a : animals) {
            if (a.isBusy()) continue;
            if (a.checkSleep()) {
                a.sleep();
            }
            else if (a.checkEat()) {
                a.eat();
            }
        }
    }

    /*
     * Смоделировать работу зоопарка в течение месяца.
     *
     * @param days количество дней симуляции.
     */
    void simulateMonth(int days) {
        srand(time(NULL));
        for (int d = 1; d <= days; d++) {
            cout << "\n=== День " << d << " ===\n";
            for (int hour = 1; hour <= 16; ++hour) {
                double intensity = (rand() % 101) / 100.0;
                cout << "\nЧас " << hour << " (интенсивность=" << intensity << ")\n";

                for (auto& a : animals) a.setBusy(false);

                for (int i = 0; i < animals.size(); i++) {
                    for (int j = i + 1; j < animals.size(); j++) {
                        if (animals[i].getType() == animals[j].getType()) {
                            if ((rand() % 100) < 20) {
                                animals[i].interaction(animals[j]);
                            }
                        }
                    }
                }

                for (auto& a : animals) {
                    if ((rand() % 100) < 50) {
                        a.playWithVisitors(intensity);
                    }
                }

                checkAnimals();
            }
        }
    }

    /*
     * Перегрузка оператора вывода для отображения информации о зоопарке.
     *
     * @param stream поток вывода.
     * @param zoo объект зоопарка.
     * @return поток с информацией о зоопарке.
     */
    friend ostream& operator<<(ostream& stream, const Zoo& zoo);
};
ostream& operator<<(ostream& stream, const Zoo& zoo) {
    stream << "<Zoo> { animals: {";
    for (int i = 0; i < zoo.animals.size(); ++i) {
        stream << zoo.animals[i] << ";";
    }
    return stream << "}";
}

/*
 * Главная функция программы.
 *
 * Создаёт несколько животных, добавляет их в зоопарк и запускает симуляцию.
 *
 * @return код завершения программы.
 */
int main() {
    setlocale(LC_ALL, "RUS");
    Cat* c1 = new Cat();
    Dog* d1 = new Dog();
    Wombat* w1 = new Wombat();
    Rat* r1 = new Rat();
    Rat* r2 = new Rat();

    Animal a1(CAT, c1, "Барсик1", 3);
    Animal a2(DOG, d1, "Шарик1", 5);
    Animal a3(WOMBAT, w1, "Вомбат1", 2);
    Animal a4(RAT, r1, "Крыса1", 1);
    Animal a5(RAT, r2, "Крыса2", 3);

    Zoo zoo;
    zoo.addAnimal(a1);
    zoo.addAnimal(a2);
    zoo.addAnimal(a3);
    zoo.addAnimal(a4);
    zoo.addAnimal(a5);

    zoo.simulateMonth(30);

    cout << endl << zoo << endl;

    delete c1;
    delete d1;
    delete w1;
    delete r1;

    return 0;
}
