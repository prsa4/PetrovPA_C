#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <ctime>
using namespace std;

/**
 * Класс Visitor представляет посетителя учреждения.
 */
class Visitor {
public:
    /** Перечисление типов посетителей */
    enum Type { Child, Adult, OldMan };
private:
    Type type;
    int time;
    bool electronic;
public:
    /**
     * Конструктор класса Visitor.
     * @param t тип посетителя
     * @param st время обслуживания
     * @param e тип очереди (электронная или живая)
     */
    Visitor(Type t, int st, bool e) : type(t), time(st), electronic(e) {}

    /**
     * Возвращает тип посетителя.
     * @return значение перечисления Type
     */
    Type getType() const { return type; }

    /**
     * Возвращает время обслуживания посетителя.
     * @return время обслуживания в минутах
     */
    int getServiceTime() const { return time; }

    /**
     * Проверяет, является ли очередь электронной.
     * @return true, если очередь электронная, иначе false
     */
    bool isElectronic() const { return electronic; }

    /**
     * Преобразует тип посетителя в строку.
     * @param t тип посетителя
     * @return строковое представление типа
     */
    string typeToString(Type t) {
        switch (t) {
        case Child: return "Ребёнок";
        case Adult: return "Взрослый";
        case OldMan: return "Старик";
        default: return " ";
        }
    }

    /**
     * Перегрузка оператора вывода для класса Visitor.
     * @param stream поток вывода
     * @param visitor объект класса Visitor
     * @return ссылка на поток
     */
    friend ostream& operator<<(ostream& stream, const Visitor& visitor);
};

ostream& operator<<(ostream& stream, const Visitor& visitor) {
    stream << "<Visitor> { type: " << visitor.type
        << ", time: " << visitor.time
        << ", electronic: " << visitor.electronic << " } ";
    return stream;
}

/**
 * Класс Window моделирует окно обслуживания посетителей.
 */
class Window {
private:
    bool busy;
    int remaining;
    Visitor::Type type;
public:
    int handled[3] = { 0, 0, 0 };

    /**
     * Конструктор по умолчанию.
     */
    Window() : busy(false), remaining(0) {}

    /**
     * Проверяет, свободно ли окно.
     * @return true, если окно свободно, иначе false
     */
    bool isFree() const { return !busy; }

    /**
     * Назначает нового посетителя окну.
     * @param v объект Visitor для обслуживания
     */
    void assign(const Visitor& v) {
        busy = true;
        remaining = v.getServiceTime();
        type = v.getType();
        handled[v.getType()]++;
    }

    /**
     * Имитация прохода одной минуты обслуживания.
     */
    void tick() {
        if (busy) {
            remaining--;
            if (remaining <= 0)
                busy = false;
        }
    }

    /**
     * Перегрузка оператора вывода для класса Window.
     * @param stream поток вывода
     * @param wind объект Window
     * @return ссылка на поток
     */
    friend ostream& operator<<(ostream& stream, const Window& wind);
};

ostream& operator<<(ostream& stream, const Window& wind) {
    stream << "<Window> { busy: " << wind.busy
        << ", remaining: " << wind.remaining
        << ", type: " << wind.type << " } ";
    return stream;
}

/**
 * Класс Institution моделирует работу учреждения с окнами и очередями.
 */
class Institution {
private:
    queue<Visitor> online;
    queue<Visitor> offline;
    vector<Window> windows;
    int processed[3] = { 0, 0, 0 };
    int total_time[3] = { 0, 0, 0 };
    int online_count[3] = { 0, 0, 0 };
    int offline_count[3] = { 0, 0, 0 };
public:
    /**
     * Конструктор класса Institution.
     * @param windowCount количество окон обслуживания
     */
    Institution(int windowCount = 3) {
        windows.resize(windowCount);
    }

    /**
     * Создаёт случайного посетителя с параметрами.
     * @return объект Visitor
     */
    Visitor createVisitor() {
        int p = rand() % 100;
        Visitor::Type type;
        int service_time;
        bool electronic;
        if (p < 5) type = Visitor::Child;
        else if (p < 45) type = Visitor::Adult;
        else if (p < 75) type = Visitor::OldMan;
        else return Visitor(Visitor::Adult, 0, false);

        if (type == Visitor::Child) {
            service_time = 5 + rand() % 6;
            electronic = (rand() % 100 < 20);
        }
        else if (type == Visitor::Adult) {
            service_time = 2 + rand() % 4;
            electronic = (rand() % 100 < 70);
        }
        else {
            service_time = 7 + rand() % 3;
            electronic = (rand() % 100 < 5);
        }
        return Visitor(type, service_time, electronic);
    }

    /**
     * Инициализация учреждения начальными посетителями.
     * @param count количество начальных посетителей
     */
    void init(int count = 10) {
        for (int i = 0; i < count; ++i) {
            Visitor v = createVisitor();
            if (v.getServiceTime() == 0) continue;
            if (v.isElectronic()) {
                online.push(v);
                online_count[v.getType()]++;
            }
            else {
                offline.push(v);
                offline_count[v.getType()]++;
            }
        }
    }

    /**
     * Имитация прибытия новых посетителей.
     * @param minute текущая минута
     * @param lunch признак обеденного перерыва
     */
    void arrivals(int minute, bool lunch) {
        if (lunch) return;
        if (minute % 5 == 0) {
            int count = rand() % 4;
            for (int i = 0; i < count; ++i) {
                Visitor v = createVisitor();
                if (v.getServiceTime() == 0) continue;
                if (v.isElectronic()) {
                    online.push(v);
                    online_count[v.getType()]++;
                }
                else {
                    offline.push(v);
                    offline_count[v.getType()]++;
                }
                logVisitors(v);
            }
        }
    }

    /**
     * Вывод информации о новом посетителе.
     * @param v объект Visitor
     */
    void logVisitors(Visitor v) {
        string electronic = v.isElectronic() ? "Электронная" : "Живая";
        cout << endl << "Посетитель: " << v.typeToString(v.getType())
            << endl << "Очередь: " << electronic << endl;
    }

    /**
     * Назначает посетителей на свободные окна.
     * @param lunch если true — обед, назначение не производится
     */
    void assignVisitors(bool lunch) {
        if (!lunch) return;
        for (auto& w : windows) {
            if (!w.isFree()) continue;
            if (!online.empty()) {
                Visitor v = online.front(); online.pop();
                total_time[v.getType()] += v.getServiceTime();
                processed[v.getType()]++;
                w.assign(v);
            }
            else if (!offline.empty()) {
                Visitor v = offline.front(); offline.pop();
                total_time[v.getType()] += v.getServiceTime();
                processed[v.getType()]++;
                w.assign(v);
            }
        }
    }

    /**
     * Запускает симуляцию работы учреждения.
     * @param total_minutes общее время работы (по умолчанию 540 минут)
     */
    void simulate(int minutes = 540) {
        init();
        for (int m = 0; m < minutes; ++m) {
            int hour = 9 + m / 60;
            bool lunch = (hour == 13);
            for (auto& w : windows) w.tick();
            arrivals(m, lunch);
            assignVisitors(!lunch);
        }
    }

    /**
     * Вывод итоговой статистики по работе учреждения.
     */
    void log() {
        cout << "Осталось в электронной очереди: " << online.size() << "\n";
        cout << "Осталось в живой очереди: " << offline.size() << "\n";
        cout << "Ребёнок: " << processed[Visitor::Child] << "\n";
        cout << "Взрослый: " << processed[Visitor::Adult] << "\n";
        cout << "Старик: " << processed[Visitor::OldMan] << "\n";
        cout << "Время обслуживания (минут):\n";
        cout << "Ребёнок: " << total_time[Visitor::Child] << "\n";
        cout << "Взрослый: " << total_time[Visitor::Adult] << "\n";
        cout << "Старик: " << total_time[Visitor::OldMan] << "\n";
    }

    /**
     * Перегрузка оператора вывода для класса Institution.
     * @param stream поток вывода
     * @param instit объект Institution
     * @return ссылка на поток
     */
    friend ostream& operator<<(ostream& stream, const Institution& instit);
};

ostream& operator<<(ostream& stream, const Institution& inst) {
    stream << "<Institution> { ";

    stream << "windows { ";
    for (size_t i = 0; i < inst.windows.size(); ++i) {
        stream << inst.windows[i] << " ";
    }
    stream << "}, ";

    stream << "online_count { "
        << "Ребёнок: " << inst.online_count[Visitor::Child] << ", "
        << "Взрослый: " << inst.online_count[Visitor::Adult] << ", "
        << "Старик: " << inst.online_count[Visitor::OldMan]
        << " }, ";

    stream << "offline_count { "
        << "Ребёнок: " << inst.offline_count[Visitor::Child] << ", "
        << "Взрослый: " << inst.offline_count[Visitor::Adult] << ", "
        << "Старик: " << inst.offline_count[Visitor::OldMan]
        << " }, ";

    stream << "processed { "
        << "Ребёнок: " << inst.processed[Visitor::Child] << ", "
        << "Взрослый: " << inst.processed[Visitor::Adult] << ", "
        << "Старик: " << inst.processed[Visitor::OldMan]
        << " }, ";

    stream << "total_time { "
        << "Ребёнок: " << inst.total_time[Visitor::Child] << ", "
        << "Взрослый: " << inst.total_time[Visitor::Adult] << ", "
        << "Старик: " << inst.total_time[Visitor::OldMan]
        << " } ";

    stream << "}";
    return stream;
}


int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));
    Institution inst;
    inst.simulate();
    inst.log();
    cout << inst;
    return 0;
}
