#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <set>
using namespace std;

class Column {
public:
    string name;
    string type;
    bool isId;

    /**
     * Конструктор класса Column.
     * @param n имя столбца.
     * @param t тип данных столбца.
     * @param id флаг, указывающий, является ли столбец идентификатором.
     */
    Column(const string& n, const string& t, bool id = false)
        : name(n), type(t), isId(id) {}
};

class Value {
public:
    Column* column;
    string data;

    /**
     * Конструктор класса Value.
     * @param c указатель на столбец, которому принадлежит значение.
     * @param d строковое представление значения.
     */
    Value(Column* c, const string& d) : column(c), data(d) {}
};

class Row {
public:
    int id;
    vector<Value> values;

    /**
     * Конструктор класса Row.
     * @param i идентификатор строки.
     */
    Row(int i) : id(i) {}
};

class Table {
public:
    string name;
    vector<Column*> columns;
    vector<Row*> rows;
    int nextId = 1;
    set<string> types = { "int", "double", "bool", "string" };

    /**
     * Конструктор таблицы.
     * @param n имя таблицы.
     */
    Table(const string& n) {
        name = n;
        columns.push_back(new Column("id", "int", true));
    }

    /**
     * Деструктор. Освобождает память, выделенную под столбцы и строки.
     */
    ~Table() {
        for (auto c : columns) {
            delete c;
        }
        for (auto r : rows) delete r;
    }

    /**
     * Поиск столбца по имени.
     * @param n имя столбца.
     * @return указатель на столбец или nullptr, если не найден.
     */
    Column* findColumn(const string& n) {
        for (auto c : columns)
            if (c->name == n) return c;
        return nullptr;
    }

    /**
     * Определяет тип данных строки.
     * @param value строка со значением.
     * @return строка с типом: int, double, bool или string.
     */
    string getType(const string& value) {
        if (value.empty()) return "string";
        bool hasDot = false;
        bool isNum = true;
        for (char c : value) {
            if (c == '.') {
                if (hasDot) return "string";
                hasDot = true;
            }
            else if (!isdigit(c) && c != '-' && c != '+') {
                isNum = false;
                break;
            }
        }
        if (isNum) return hasDot ? "double" : "int";
        string low = value;
        for (auto& c : low) c = tolower(c);
        if (low == "true" || low == "false" || low == "1" || low == "0") return "bool";
        return "string";
    }

    /**
     * Проверяет, соответствует ли значение типу столбца.
     * @param type ожидаемый тип столбца.
     * @param value проверяемое значение.
     * @return true, если тип корректен; иначе false.
     */
    bool validType(const string& type, const string& value) {
        if (value.empty()) return true;
        string valType = getType(value);
        if (type == valType) return true;
        if (type == "string") return true;
        if (type == "double" && valType == "int") return true;
        return false;
    }

    /**
     * Добавляет новый столбец в таблицу.
     * @param n имя столбца.
     * @param type тип данных столбца.
     * @throws runtime_error если столбец уже существует.
     */
    void addColumn(const string& n, const string& type) {
        if (findColumn(n)) throw runtime_error("Столбец уже существует");
        types.insert(type);
        columns.push_back(new Column(n, type));
        for (auto r : rows)
            r->values.push_back(Value(columns.back(), ""));
        cout << "Добавлен столбец " << n << " (" << type << ")\n";
    }

    /**
     * Удаляет столбец из таблицы и всех строк.
     * @param n имя столбца для удаления.
     * @throws runtime_error если столбец не найден или является id.
     */
    void removeColumn(const string& n) {
        for (size_t i = 0; i < columns.size(); ++i) {
            if (columns[i]->name == n) {
                if (columns[i]->isId) throw runtime_error("Нельзя удалить id");
                delete columns[i];
                columns.erase(columns.begin() + i);
                for (auto r : rows) {
                    if (i - 1 < r->values.size())
                        r->values.erase(r->values.begin() + (i - 1));
                }
                cout << "Удалён столбец " << n << endl;
                return;
            }
        }
        throw runtime_error("Столбец не найден");
    }

    /**
     * Добавляет новую строку с данными.
     * @param line строка со значениями, разделёнными ';'.
     * @throws runtime_error при ошибке типа данных или избыточных значениях.
     */
    void addRow(const string& line) {
        vector<string> parts;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ';')) parts.push_back(item);
        if (parts.size() > columns.size() - 1) throw runtime_error("Слишком много значений");

        Row* r = new Row(nextId++);
        for (int i = 1; i < columns.size(); ++i) {
            string val = (i - 1 < parts.size()) ? parts[i - 1] : "";
            string type = columns[i]->type;
            if (!types.count(type)) throw runtime_error("Неизвестный тип данных: " + type);
            if (!validType(type, val)) throw runtime_error("Неверный тип для " + columns[i]->name);
            r->values.push_back(Value(columns[i], val));
        }
        rows.push_back(r);
        cout << "Добавлена строка.\n";
    }

    /**
     * Удаляет строку по идентификатору.
     * @param id идентификатор строки для удаления.
     * @throws runtime_error если строка не найдена.
     */
    void deleteRow(int id) {
        for (size_t i = 0; i < rows.size(); ++i)
            if (rows[i]->id == id) {
                delete rows[i];
                rows.erase(rows.begin() + i);
                cout << "Удалена строка id=" << id << endl;
                return;
            }
        throw runtime_error("Строка не найдена");
    }

    /**
     * Выводит структуру таблицы и её содержимое.
     */
    void show() {
        cout << "\nТаблица: " << name << "\n";
        cout << "Количество столбцов: " << columns.size() << "\n";
        cout << "Количество строк: " << rows.size() << "\n\n";

        cout << "Структура таблицы:\n";
        for (auto c : columns)
            cout << " - " << c->name << " (" << c->type << ")\n";

        cout << "\nДанные:\n";
        for (auto c : columns) cout << c->name << "\t";
        cout << "\n----------------------------------------\n";

        for (auto r : rows) {
            cout << r->id << "\t";
            for (auto& v : r->values)
                cout << v.data << "\t";
            cout << "\n";
        }
    }

    /**
     * Поиск строк, содержащих указанное значение.
     * @param name значение для поиска.
     */
    void search(const string& name) {
        for (auto r : rows) {
            for (size_t i = 0; i < r->values.size(); ++i)
                if (r->values[i].data == name)
                    cout << "Строка id=" << r->id << " Столбец: " << r->values[i].column->name << "\n";
        }
    }

    /**
     * Вывод значений определённого столбца.
     * @param name имя столбца.
     * @throws runtime_error если столбец не найден.
     */
    void searchColumn(const string& name) {
        for (size_t i = 0; i < columns.size(); ++i)
            if (columns[i]->name == name) {
                cout << "Столбец " << name << ": ";
                for (auto r : rows)
                    if (i - 1 < r->values.size())
                        cout << r->values[i - 1].data << " ";
                cout << "\n";
                return;
            }
        throw runtime_error("Столбец не найден");
    }
};

class Catalog {
public:
    map<string, Table*> tables;

    /**
     * Деструктор. Удаляет все таблицы при завершении программы.
     */
    ~Catalog() { for (auto& p : tables) delete p.second; }

    /**
     * Создаёт новую таблицу.
     * @param n имя таблицы.
     * @throws runtime_error если таблица уже существует.
     */
    void createTable(const string& n) {
        if (tables.count(n)) throw runtime_error("Таблица уже существует");
        tables[n] = new Table(n);
        cout << "Создана таблица " << n << endl;
    }

    /**
     * Удаляет таблицу.
     * @param n имя таблицы.
     * @throws runtime_error если таблица не найдена.
     */
    void deleteTable(const string& n) {
        if (!tables.count(n)) throw runtime_error("Таблица не найдена");
        delete tables[n];
        tables.erase(n);
        cout << "Удалена таблица " << n << endl;
    }

    /**
     * Возвращает указатель на таблицу по имени.
     * @param n имя таблицы.
     * @return указатель на таблицу.
     * @throws runtime_error если таблица не найдена.
     */
    Table* get(const string& n) {
        if (!tables.count(n)) throw runtime_error("Таблица не найдена");
        return tables[n];
    }

    /**
     * Выводит список всех таблиц в каталоге.
     */
    void listTables() {
        cout << "Список таблиц:\n";
        if (tables.empty()) cout << "(пусто)\n";
        for (auto& p : tables) cout << " - " << p.first << endl;
    }
};

/**
 * Вывод справки по доступным командам.
 */
void help() {
    cout << "\nКоманды:\n"
        << "1 Создать таблицу <имя>\n"
        << "2 Удалить таблицу <имя>\n"
        << "3 Добавить столбец <таблица> <имя> <тип>\n"
        << "4 Удалить столбец <таблица> <имя>\n"
        << "5 Добавить строку <таблица> <значения через ;>\n"
        << "6 Удалить строку <таблица> <id>\n"
        << "7 Показать таблицу <таблица>\n"
        << "8 Список таблиц\n"
        << "9 Поиск значения <таблица> <значение>\n"
        << "10 Вывод столбца <таблица> <имя>\n"
        << "11 Помощь\n"
        << "0 Выход\n";
}

/**
 * Обрабатывает команды пользователя.
 */
void run() {
    Catalog cat;
    int cmd;
    help();
    while (true) {
        try {
            cout << "\n>> ";
            if (!(cin >> cmd)) break;
            if (cmd == 1) { string n; cin >> n; cat.createTable(n); }
            else if (cmd == 2) { string n; cin >> n; cat.deleteTable(n); }
            else if (cmd == 3) { string t, n, type; cin >> t >> n >> type; cat.get(t)->addColumn(n, type); }
            else if (cmd == 4) { string t, n; cin >> t >> n; cat.get(t)->removeColumn(n); }
            else if (cmd == 5) { string t; cin >> t; string rest; getline(cin, rest); if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1); cat.get(t)->addRow(rest); }
            else if (cmd == 6) { string t; int id; cin >> t >> id; cat.get(t)->deleteRow(id); }
            else if (cmd == 7) { string t; cin >> t; cat.get(t)->show(); }
            else if (cmd == 8) { cat.listTables(); }
            else if (cmd == 9) { string t, name; cin >> t >> name; cat.get(t)->search(name); }
            else if (cmd == 10) { string t, name; cin >> t >> name; cat.get(t)->searchColumn(name); }
            else if (cmd == 11) { help(); }
            else if (cmd == 0) break;
            else cout << "Неизвестная команда\n";
        }
        catch (exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    }
    cout << "Выход.\n";
}


int main() {
    setlocale(LC_ALL, "RUS");
    run();
    return 0;
}
