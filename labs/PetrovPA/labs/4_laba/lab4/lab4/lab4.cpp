#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

/**
 * Перечисление типов фигур.
 */
enum figureType {
    HEXAGON,  
    SQUARE,   
    CIRCLE    
};

/**
 * Класс, представляющий шестиугольник.
 */
class Hexagon {
public:
    double x, y;   
    double a;    

    /**
     * Конструктор шестиугольника.
     * @param x Координата X центра.
     * @param y Координата Y центра.
     * @param a Длина стороны.
     */
    Hexagon(double x, double y, double a) : x(x), y(y), a(a) {}

    /**
     * Выводит информацию о шестиугольнике.
     */
    void print() const {
        cout << "Шестиугольник (" << x << "," << y << "), a=" << a << endl;
    }
    string getType() { return "Шестиугольник"; }
    /**
     * Вычисляет площадь шестиугольника.
     * @return Площадь.
     */
    double area() const { return 3 * sqrt(3) / 2 * a * a; }

    /**
     * Возвращает радиус описанной окружности.
     * @return Радиус.
     */
    double radius() const { return a; }
};

/**
 * Класс, представляющий квадрат.
 */
class Square {
public:
    double x, y; 
    double a;    

    /**
     * Конструктор квадрата.
     * @param x Координата X центра.
     * @param y Координата Y центра.
     * @param a Длина стороны.
     */
    Square(double x, double y, double a) : x(x), y(y), a(a) {}

    /**
     * Выводит информацию о квадрате.
     */
    void print() const {
        cout << "Квадрат (" << x << "," << y << "), a=" << a << endl;
    }
    string getType() { return "Квадрат"; }

    /**
     * Вычисляет площадь квадрата.
     * @return Площадь.
     */
    double area() const { return a * a; }

    /**
     * Возвращает радиус описанной окружности.
     * @return Радиус.
     */
    double radius() const { return a * sqrt(2) / 2; }
};

/**
 * Класс, представляющий круг.
 */
class Circle {
public:
    double x, y; 
    double r;   

    /**
     * Конструктор круга.
     * @param x Координата X центра.
     * @param y Координата Y центра.
     * @param r Радиус.
     */
    Circle(double x, double y, double r) : x(x), y(y), r(r) {}

    /**
     * Выводит информацию о круге.
     */
    void print() const {
        cout << "Круг (" << x << "," << y << "), r=" << r << endl;
    }
    string getType() { return "Круг"; }

    /**
     * Вычисляет площадь круга.
     * @return Площадь.
     */
    double area() const { return 3.14159 * r * r; }

    /**
     * Возвращает радиус.
     * @return Радиус.
     */
    double radius() const { return r; }
};

/**
 * Универсальный класс для хранения фигуры любого типа.
 */
class Figure {
private:
    void* ptr;        
    figureType type;  
    double vx, vy;   

public:
    /// Конструкторы для разных фигур
    Figure(Hexagon* h) { ptr = h; type = HEXAGON; setRandomVelocity(); }
    Figure(Square* s) { ptr = s; type = SQUARE;  setRandomVelocity(); }
    Figure(Circle* c) { ptr = c; type = CIRCLE;  setRandomVelocity(); }

    /**
     * Выводит информацию о фигуре.
     */
    void print() const {
        switch (type) {
        case HEXAGON: ((Hexagon*)ptr)->print(); break;
        case SQUARE:  ((Square*)ptr)->print();  break;
        case CIRCLE:  ((Circle*)ptr)->print();  break;
        }
    }

    string getType() const {
        switch (type) {
        case HEXAGON: return ((Hexagon*)ptr)->getType();
        case SQUARE:  return ((Square*)ptr)->getType();
        case CIRCLE:  return ((Circle*)ptr)->getType();
        }
    }

    /**
     * Возвращает площадь фигуры.
     * @return Площадь.
     */
    double getArea() const {
        switch (type) {
        case HEXAGON: return ((Hexagon*)ptr)->area();
        case SQUARE:  return ((Square*)ptr)->area();
        case CIRCLE:  return ((Circle*)ptr)->area();
        }
        return 0;
    }

    /**
     * Возвращает радиус фигуры (радиус описанной окружности).
     * @return Радиус.
     */
    double getRadius() const {
        switch (type) {
        case HEXAGON: return ((Hexagon*)ptr)->radius();
        case SQUARE:  return ((Square*)ptr)->radius();
        case CIRCLE:  return ((Circle*)ptr)->radius();
        }
        return 0;
    }

    /**
     * Получает координату X центра фигуры.
     * @return Координата X.
     */
    double getX() const {
        switch (type) {
        case HEXAGON: return ((Hexagon*)ptr)->x;
        case SQUARE:  return ((Square*)ptr)->x;
        case CIRCLE:  return ((Circle*)ptr)->x;
        }
        return 0;
    }

    /**
     * Получает координату Y центра фигуры.
     * @return Координата Y.
     */
    double getY() const {
        switch (type) {
        case HEXAGON: return ((Hexagon*)ptr)->y;
        case SQUARE:  return ((Square*)ptr)->y;
        case CIRCLE:  return ((Circle*)ptr)->y;
        }
        return 0;
    }

    /**
     * Устанавливает новые координаты центра фигуры.
     * @param x Новая координата X.
     * @param y Новая координата Y.
     */
    void setPos(double x, double y) {
        switch (type) {
        case HEXAGON: ((Hexagon*)ptr)->x = x; ((Hexagon*)ptr)->y = y; break;
        case SQUARE:  ((Square*)ptr)->x = x; ((Square*)ptr)->y = y; break;
        case CIRCLE:  ((Circle*)ptr)->x = x; ((Circle*)ptr)->y = y; break;
        }
    }

    /**
     * Перемещает фигуру в соответствии с её скоростью.
     */
    void move() {
        double nx = getX() + vx;
        double ny = getY() + vy;
        setPos(nx, ny);
    }

    /**
    * Разворот по оси X
    */
    void turnX() { vx = -vx; }
    /**
    * Разворот по оси Y
    */
    void turnY() { vy = -vy; }

    /**
    * Разворот по обеим осям
    */

    void turn() { vx = -vx; vy = -vy; }

    /**
     * Задает фигуре случайную скорость.
     */
    void setRandomVelocity() {
        vx = (rand() % 3 - 1) * 0.5;
        vy = (rand() % 3 - 1) * 0.5;
        if (vx == 0 && vy == 0) vx = 0.5;
    }
};

/**
 * Класс сцены, содержащей несколько фигур.
 */
class Scene {
private:
    vector<Figure> figures;
    double width, height;  

public:
    /**
     * Конструктор сцены.
     * @param w Ширина.
     * @param h Высота.
     */
    Scene(double w, double h) : width(w), height(h) {}

    /**
     * Добавляет фигуру на сцену.
     * @param f Фигура.
     */
    void add(Figure f) { figures.push_back(f); }

    /**
     * Выводит информацию обо всех фигурах.
     */
    void print() {
        for (auto& f : figures) f.print();
    }

    /**
     * Вычисляет суммарную площадь всех фигур на сцене.
     * @return Суммарная площадь.
     */
    double totalArea() {
        double sum = 0;
        for (auto& f : figures) sum += f.getArea();
        return sum;
    }

    /**
     * Запускает симуляцию движения фигур.
     *
     * @param seconds Время симуляции в секундах.
     * @param dt Шаг симуляции.
     */
    void simulate(double seconds, double dt) {
        int steps = seconds / dt;
        for (int t = 0; t < steps; t++) {
            
            for (auto& f : figures) {
                f.move();
                if (f.getX() < 0 || f.getX() > width) {
                    f.turnX();
                    printf("Столкновение произошло между %s и стенкой\n", f.getType().c_str());

                }
                if (f.getY() < 0 || f.getY() > height) { 
                    f.turnY();
                    printf("Столкновение произошло между %s и стенкой\n", f.getType().c_str());
                }
            }
           
            for (size_t i = 0; i < figures.size(); i++) {
                for (size_t j = i + 1; j < figures.size(); j++) {
                    double dx = figures[i].getX() - figures[j].getX();
                    double dy = figures[i].getY() - figures[j].getY();
                    double dist = sqrt(dx * dx + dy * dy);
                    if (dist < figures[i].getRadius() + figures[j].getRadius()) {
                        printf("Столкновение произошло между %s и %s\n", figures[i].getType().c_str(), figures[j].getType().c_str());
                        figures[i].turn();
                        figures[j].turn();
                    }
                }
            }
        }
    }
};

/**
 * Создает случайную фигуру в пределах заданной сцены.
 *
 * @param W Ширина сцены.
 * @param H Высота сцены.
 * @return Случайная фигура.
 */
Figure randomFigure(double W, double H) {
    int t = rand() % 3;
    if (t == 0) return Figure(new Circle(rand() % int(W), rand() % int(H), 2 + rand() % 5));
    if (t == 1) return Figure(new Square(rand() % int(W), rand() % int(H), 3 + rand() % 5));
    return Figure(new Hexagon(rand() % int(W), rand() % int(H), 3 + rand() % 5));
}


int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(0));
    int n;
    cout << "Количество фигур: ";
    cin >> n;

    Scene sc(100, 100);

    for (int i = 0; i < n; i++) {
        sc.add(randomFigure(100, 100));
    }

    cout << "Начальная сцена:\n";
    sc.print();
    cout << "Суммарная площадь: " << sc.totalArea() << endl;

    sc.simulate(600, 0.01);

    cout << "\nПосле симуляции:\n";
    sc.print();

    return 0;
}
