#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;

/*
 * Цель для стрельбы.
 *
 * @param distance расстояние до цели (м).
 */
struct Target {
    double distance;
};

/*
 * Результат серии выстрелов.
 *
 * @param hits количество попаданий.
 * @param shotsFired количество произведённых выстрелов.
 * @param timeSpent время, затраченное на стрельбу (с).
 */
struct ShotResult {
    int hits;
    int shotsFired;
    double timeSpent;
};

/*
 * Вычисляет вероятность попадания.
 *
 * @param d расстояние до цели.
 * @param eff прицельная дальность оружия.
 * @param maxMods макс. кол-во модификаций.
 * @param dispersion кучность.
 * @return вероятность попадания.
 */
double hitProbability(double d, double eff, double maxMods, double dispersion) {
    if (d <= eff) return 1.0;
    double i = max(1.0, maxMods * dispersion);
    double p = eff * (i / 23.0) / d;
    if (p > 1.0) p = 1.0;
    if (p < 0.0) p = 0.0;
    return p;
}

/*
 * Абстрактный базовый класс оружия.
 *
 * Содержит общие поля и интерфейс для всех типов оружия.
 */
class Weapon {
protected:
    string displayName;
    int ammo;
    int fullAmmo;
    double caliber;
    int typeShoot;
    double weight;
    double effectiveRange;
    double reloadTime;
    double dispersion;
    int maxMods;
    double fireRate;
    int shotsSinceReload;

public:
    /*
     * Конструктор.
     *
     * @param n название.
     * @param cap ёмкость магазина.
     * @param cal калибр (мм).
     * @param type тип стрельбы.
     * @param w вес (кг).
     * @param eff прицельная дальность (м).
     * @param rT время перезарядки (с).
     * @param disp кучность.
     * @param mods макс. модификаций.
     * @param rof скорострельность (выстр./мин).
     */
    Weapon(string n, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : displayName(n), fullAmmo(cap), caliber(cal), typeShoot(type),
        weight(w), effectiveRange(eff), reloadTime(rT),
        dispersion(disp), maxMods(mods), fireRate(rof),
        shotsSinceReload(0) {
        ammo = fullAmmo;
    }

    virtual ~Weapon() {}

    /*
     * Перезарядка оружия.
     *
     * @param count число патронов.
     * @return время перезарядки (с).
     */
    virtual double reload(int count) = 0;

    /*
     * Выполнить выстрелы.
     *
     * @param mode режим стрельбы.
     * @param t цель.
     * @param shots число выстрелов.
     * @return результат стрельбы.
     */
    virtual ShotResult shoot(int mode, Target t, int shots) = 0;

    /*
     * Получить имя оружия.
     *
     * @return название.
     */
    string name() const { return displayName; }

    /*
     * Получить текущее количество патронов.
     *
     * @return ammo.
     */
    int getAmmo() const { return ammo; }

    /*
     * Ёмкость магазина.
     *
     * @return fullAmmo.
     */
    int cap() const { return fullAmmo; }

    /*
     * Установить количество патронов (с проверкой границ).
     *
     * @param n новое количество.
     */
    void setAmmo(int n) { ammo = max(0, min(n, fullAmmo)); }

    /*
     * Добавить патроны.
     *
     * @param n число патронов для добавления.
     */
    void addAmmo(int n) { setAmmo(getAmmo() + n); }

    /*
     * Использовать один патрон.
     */
    void useAmmo() { if (ammo > 0) { --ammo; ++shotsSinceReload; } }

    /*
     * Тип стрельбы.
     *
     * @return typeShoot.
     */
    int getType() const { return typeShoot; }

    /*
     * Прицельная дальность.
     *
     * @return effectiveRange.
     */
    double getEff() const { return effectiveRange; }

    /*
     * Количество модификаций.
     *
     * @return maxMods.
     */
    int getMods() const { return maxMods; }

    /*
     * Время перезарядки.
     *
     * @return reloadTime.
     */
    double getReloadTime() const { return reloadTime; }

    /*
     * Интервал между выстрелами в секундах.
     *
     * @return интервал (сек).
     */
    double shotIntervalSeconds() const {
        if (fireRate <= 0.0) return 0.2;
        return 60.0 / fireRate;
    }

    /*
     * Сброс счётчика выстрелов после перезарядки.
     */
    void resetShots() { shotsSinceReload = 0; }

    /*
     * Получить число выстрелов после последней перезарядки.
     *
     * @return shotsSinceReload.
     */
    int getShotsSinceReload() const { return shotsSinceReload; }

    /*
     * Печать характеристик оружия.
     */
    void printSpecs() {
        cout << "Оружие: " << displayName << "\n"
            << "Магазин: " << fullAmmo << "\n"
            << "Калибр: " << caliber << " мм\n"
            << "Тип стрельбы: " << typeShoot << "\n"
            << "Вес: " << weight << " кг\n"
            << "Прицельная дальность: " << effectiveRange << " м\n"
            << "Кучность: " << dispersion << "\n"
            << "Макс. модификаций: " << maxMods << "\n"
            << "Скорострельность: " << fireRate << " выстр./мин\n\n";
    }
    friend ostream& operator<<(ostream& stream, const Weapon& weapon);
};
ostream& operator<<(ostream& stream, const Weapon& weapon) {
    stream << "<Weapon> { "
        << "displayName: " << weapon.displayName
        << ", ammo: " << weapon.ammo
        << ", fullAmmo: " << weapon.fullAmmo
        << ", caliber: " << weapon.caliber
        << ", typeShoot: " << weapon.typeShoot
        << ", weight: " << weapon.weight
        << ", effRange: " << weapon.effectiveRange
        << ", reloadTime: " << weapon.reloadTime
        << ", dispersion: " << weapon.dispersion
        << ", maxMods: " << weapon.maxMods
        << ", fireRate: " << weapon.fireRate
        << " }";
    return stream;
}

/*
 * Класс Пистолет.
 *
 */
class Pistol : public Weapon {
public:
    /*
     * Конструктор с параметрами.
     */
    Pistol(string name, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : Weapon(name, cap, cal, type, w, eff, rT, disp, mods, rof) {}

    /*
     * Конструктор по умолчанию (случайные характеристики).
     */
    Pistol()
        : Weapon("Пистолет",
            rand() % 6 + 10, 9.0, 0,
            0.8 + (rand() % 5) * 0.1,
            (double)(rand() % 61 + 40),
            1.0 + (rand() % 11) / 10.0,
            0.5 + (rand() % 11) / 10.0,
            1 + rand() % 3,
            250 + rand() % 151) {}

    /*
     * Перезарядка: заполнить магазин (или до count).
     *
     * @param count число патронов.
     * @return время перезарядки (с).
     */
    double reload(int count) override {
        int add = min(fullAmmo, count);
        setAmmo(add);
        resetShots();
        return reloadTime;
    }

    /*
     * Стрельба для пистолета (одиночные выстрелы).
     *
     * @param ignored режим (не используется).
     * @param t цель.
     * @param shots число выстрелов.
     * @return ShotResult.
     */
    ShotResult shoot(int, Target t, int shots) override {
        ShotResult r{ 0,0,0.0 };
        double interval = shotIntervalSeconds();
        for (int i = 0; i < shots && getAmmo() > 0; ++i) {
            double p = hitProbability(t.distance, getEff(), getMods(), dispersion);
            if (((double)rand() / RAND_MAX) < p) ++r.hits;
            ++r.shotsFired;
            useAmmo();
            r.timeSpent += interval;
        }
        return r;
    }
    friend ostream& operator<<(ostream& stream, const Pistol& pistol);
};

ostream& operator<<(ostream& stream, const Pistol& pistol) {
    stream << "<Pistol> { "
        << "displayName: " << pistol.displayName
        << ", ammo: " << pistol.ammo
        << ", fullAmmo: " << pistol.fullAmmo
        << ", caliber: " << pistol.caliber
        << ", typeShoot: " << pistol.typeShoot
        << ", weight: " << pistol.weight
        << ", effRange: " << pistol.effectiveRange
        << ", reloadTime: " << pistol.reloadTime
        << ", dispersion: " << pistol.dispersion
        << ", maxMods: " << pistol.maxMods
        << ", fireRate: " << pistol.fireRate
        << " }";
    return stream;
}

/*
 * Класс однозарядной винтовки (SingleRifle).
 *
 * Особенности: малая ёмкость, небольшая деградация точности.
 */
class SingleRifle : public Weapon {
public:
    SingleRifle(string name, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : Weapon(name, cap, cal, type, w, eff, rT, disp, mods, rof) {}

    SingleRifle()
        : Weapon("Одноз. винт.",
            1,
            7.62, 0,
            3.5 + (rand() % 20) / 10.0,
            (double)(rand() % 401 + 400),
            2.0 + (rand() % 21) / 10.0,
            0.2 + (rand() % 6) / 20.0,
            rand() % 2,
            25 + rand() % 11) {}

    /*
     * Перезарядка: добавить патроны (ограниченно).
     */
    double reload(int count) override {
        int add = min(fullAmmo - getAmmo(), count);
        addAmmo(add);
        double t = reloadTime * add;
        resetShots();
        return t;
    }

    /*
     * Стрельба: увеличиваем вероятность немного в зависимости от модов;
     * применяем слабую экспоненциальную деградацию точности.
     */
    ShotResult shoot(int, Target t, int shots) override {
        ShotResult r{ 0,0,0.0 };
        double interval = shotIntervalSeconds();
        for (int i = 0; i < shots && getAmmo() > 0; ++i) {
            double p = hitProbability(t.distance, getEff(), getMods(), dispersion);
            p *= (1.0 + 0.06 * getMods());
            double decay = pow(0.998, getShotsSinceReload());
            p *= decay;
            p = min(1.0, max(0.0, p));
            if (((double)rand() / RAND_MAX) < p) ++r.hits;
            ++r.shotsFired;
            useAmmo();
            r.timeSpent += interval;
        }
        return r;
    }
    friend ostream& operator<<(ostream& stream, const SingleRifle& rifle);
};

ostream& operator<<(ostream& stream, const SingleRifle& rifle) {
    stream << "<SingleRifle> { "
        << "displayName: " << rifle.displayName
        << ", ammo: " << rifle.ammo
        << ", fullAmmo: " << rifle.fullAmmo
        << ", caliber: " << rifle.caliber
        << ", typeShoot: " << rifle.typeShoot
        << ", weight: " << rifle.weight
        << ", effRange: " << rifle.effectiveRange
        << ", reloadTime: " << rifle.reloadTime
        << ", dispersion: " << rifle.dispersion
        << ", maxMods: " << rifle.maxMods
        << ", fireRate: " << rifle.fireRate
        << " }";
    return stream;
}

/*
 * Класс помпового дробовика (PumpShotgun).
 *
 * Особенности: высокая кучность значение dispersion, небольшая прибавка от модов,
 * более выраженный спад точности с выстрелами.
 */
class PumpShotgun : public Weapon {
public:
    PumpShotgun(string name, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : Weapon(name, cap, cal, type, w, eff, rT, disp, mods, rof) {}

    PumpShotgun()
        : Weapon("Дробовик",
            rand() % 3 + 5,
            18.5, 0,
            3.0 + (rand() % 31) / 10.0,
            (double)(rand() % 26 + 25),
            0.6 + (rand() % 15) / 10.0,
            3.0 + (rand() % 21) / 10.0,
            rand() % 2,
            50 + rand() % 41) {}

    /*
     * Перезарядка: добавление патронов по одному.
     */
    double reload(int count) override {
        int canAdd = min(fullAmmo - getAmmo(), count);
        addAmmo(canAdd);
        double t = reloadTime * canAdd;
        resetShots();
        return t;
    }

    /*
     * Стрельба дробовика.
     */
    ShotResult shoot(int, Target t, int shots) override {
        ShotResult r{ 0,0,0.0 };
        double interval = shotIntervalSeconds();
        for (int i = 0; i < shots && getAmmo() > 0; ++i) {
            double p = hitProbability(t.distance, getEff(), getMods(), dispersion);
            p *= (1.0 + 0.03 * getMods());
            double decay = pow(0.97, getShotsSinceReload());
            p *= decay;
            p = min(1.0, max(0.0, p));
            if (((double)rand() / RAND_MAX) < p) ++r.hits;
            ++r.shotsFired;
            useAmmo();
            r.timeSpent += interval;
        }
        return r;
    }
    friend ostream& operator<<(ostream& stream, const PumpShotgun& shotgun);
};

ostream& operator<<(ostream& stream, const PumpShotgun& shotgun) {
    stream << "<PumpShotgun> { "
        << "displayName: " << shotgun.displayName
        << ", ammo: " << shotgun.ammo
        << ", fullAmmo: " << shotgun.fullAmmo
        << ", caliber: " << shotgun.caliber
        << ", typeShoot: " << shotgun.typeShoot
        << ", weight: " << shotgun.weight
        << ", effRange: " << shotgun.effectiveRange
        << ", reloadTime: " << shotgun.reloadTime
        << ", dispersion: " << shotgun.dispersion
        << ", maxMods: " << shotgun.maxMods
        << ", fireRate: " << shotgun.fireRate
        << " }";
    return stream;
}

/*
 * Класс автоматической винтовки (AssaultRifle).
 *
 * Особенности: разные режимы стрельбы (mode), влияние модификаций, деградация точности
 * в зависимости от режима.
 */
class AssaultRifle : public Weapon {
public:
    AssaultRifle(string name, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : Weapon(name, cap, cal, type, w, eff, rT, disp, mods, rof) {}

    AssaultRifle()
        : Weapon("Автом. винтовка",
            rand() % 11 + 20,
            5.56, 2,
            3.0 + (rand() % 31) / 10.0,
            (double)(rand() % 301 + 200),
            1.5 + (rand() % 11) / 10.0,
            0.5 + (rand() % 21) / 10.0,
            rand() % 4,
            550 + rand() % 151) {}

    /*
     * Перезарядка: полная (вставляем полный магазин).
     */
    double reload(int count) override {
        setAmmo(cap());
        resetShots();
        return reloadTime;
    }

    /*
     * Стрельба: режимы влияют на скорость деградации точности.
     *
     * @param mode режим (0 = одиночный, 1 = , 2 = режим2 и т.д.).
     */
    ShotResult shoot(int mode, Target t, int shots) override {
        ShotResult r{ 0,0,0.0 };
        double interval = shotIntervalSeconds();
        for (int i = 0; i < shots && getAmmo() > 0; ++i) {
            double p = hitProbability(t.distance, getEff(), getMods(), dispersion);
            p *= (1.0 + 0.05 * getMods());
            if (mode == 1) {
                double decay = pow(0.9975, getShotsSinceReload());
                p *= decay;
            }
            else if (mode == 2) {
                double decay = pow(0.993, getShotsSinceReload());
                p *= decay;
            }
            else {
                double decay = pow(0.995, getShotsSinceReload());
                p *= decay;
            }
            p = min(1.0, max(0.0, p));
            if (((double)rand() / RAND_MAX) < p) ++r.hits;
            ++r.shotsFired;
            useAmmo();
            r.timeSpent += interval;
        }
        return r;
    }
    friend ostream& operator<<(ostream& stream, const AssaultRifle& ar);
};

ostream& operator<<(ostream& stream, const AssaultRifle& ar) {
    stream << "<AssaultRifle> { "
        << "displayName: " << ar.displayName
        << ", ammo: " << ar.ammo
        << ", fullAmmo: " << ar.fullAmmo
        << ", caliber: " << ar.caliber
        << ", typeShoot: " << ar.typeShoot
        << ", weight: " << ar.weight
        << ", effRange: " << ar.effectiveRange
        << ", reloadTime: " << ar.reloadTime
        << ", dispersion: " << ar.dispersion
        << ", maxMods: " << ar.maxMods
        << ", fireRate: " << ar.fireRate
        << " }";
    return stream;
}

/*
 * Класс снайперской винтовки (SniperRifle).
 *
 * Особенности: высокая прицельная дальность, малая деградация.
 */
class SniperRifle : public Weapon {
public:
    SniperRifle(string name, int cap, double cal, int type, double w, double eff,
        double rT, double disp, int mods, double rof)
        : Weapon(name, cap, cal, type, w, eff, rT, disp, mods, rof) {}

    SniperRifle()
        : Weapon("Снайп. винтовка",
            rand() % 4 + 3,
            7.62, 0,
            5.0 + (rand() % 31) / 10.0,
            (double)(rand() % 501 + 700),
            2.0 + (rand() % 21) / 10.0,
            0.08 + (rand() % 15) / 200.0,
            1 + rand() % 2,
            30 + rand() % 11) {}

    /*
     * Перезарядка: полная.
     */
    double reload(int count) override {
        setAmmo(cap());
        resetShots();
        return reloadTime;
    }

    /*
     * Стрельба для снайперки.
     */
    ShotResult shoot(int, Target t, int shots) override {
        ShotResult r{ 0,0,0.0 };
        double interval = shotIntervalSeconds();
        for (int i = 0; i < shots && getAmmo() > 0; ++i) {
            double p = hitProbability(t.distance, getEff(), getMods(), dispersion);
            p *= (1.0 + 0.07 * getMods());
            double decay = pow(0.9985, getShotsSinceReload());
            p *= decay;
            p = min(1.0, max(0.0, p));
            if (((double)rand() / RAND_MAX) < p) ++r.hits;
            ++r.shotsFired;
            useAmmo();
            r.timeSpent += interval;
        }
        return r;
    }
    friend ostream& operator<<(ostream& stream, const SniperRifle& sr);
};
ostream& operator<<(ostream& stream, const SniperRifle& sr) {
    stream << "<SniperRifle> { "
        << "displayName: " << sr.displayName
        << ", ammo: " << sr.ammo
        << ", fullAmmo: " << sr.fullAmmo
        << ", caliber: " << sr.caliber
        << ", typeShoot: " << sr.typeShoot
        << ", weight: " << sr.weight
        << ", effRange: " << sr.effectiveRange
        << ", reloadTime: " << sr.reloadTime
        << ", dispersion: " << sr.dispersion
        << ", maxMods: " << sr.maxMods
        << ", fireRate: " << sr.fireRate
        << " }";
    return stream;
}

/*
 * Статистика стрельбы по оружию и дистанции.
 *
 * @param weapon название оружия.
 * @param dist дистанция (м).
 * @param shots кол-во выстрелов.
 * @param hits кол-во попаданий.
 * @param acc точность (0–1).
 * @param rof реальная скорострельность (выстр./мин).
 */
struct Stat {
    string weapon;
    double dist;
    int shots;
    int hits;
    double acc;
    double rof;
};

/*
 * Класс стрельбища (ShootingRange).
 *
 * Симуляция выполняет один прогон (без усреднений).
 */
class ShootingRange {
    vector<Target> targets;
    vector<Weapon*> weapons;
public:
    /*
     * Конструктор.
     */
    ShootingRange(const vector<Target>& t) : targets(t) {}

    /*
     * Добавить оружие на стрельбище.
     *
     * @param w указатель на Weapon.
     */
    void addWeapon(Weapon* w) { weapons.push_back(w); }

    /*
     * Симуляция стрельбы (один прогон).
     *
     * @param shotsPerTarget количество выстрелов на каждую цель.
     * @return вектор статистики (по каждому оружию и каждой дистанции).
     */
    vector<Stat> simulate(int shotsPerTarget) {
        vector<Stat> stats;
        for (auto w : weapons) {
            for (auto t : targets) {
                w->reload(w->cap());
                int done = 0;
                int hits = 0;
                double timeSec = 0.0;
                while (done < shotsPerTarget) {
                    if (w->getAmmo() == 0) {
                        timeSec += w->reload(w->cap());
                    }
                    int fire = min(shotsPerTarget - done, w->getAmmo());
                    if (fire <= 0) break;
                    ShotResult r = w->shoot(w->getType(), t, fire);
                    done += r.shotsFired;
                    hits += r.hits;
                    timeSec += r.timeSpent;
                }
                double acc = (static_cast<double>(hits) / static_cast<double>(done));
                double rof = (static_cast<double>(done) / (timeSec / 60.0));
                stats.push_back({ w->name(), t.distance, done, hits, acc, rof });
            }
        }
        return stats;
    }

};
/*
 * Печать характеристик оружия.
 *
 * @param weapons вектор указателей на оружие.
 */
void printWeaponInfo(const vector<Weapon*>& weapons) {
    for (auto w : weapons) {
        w->printSpecs();
    }
}

/*
 * Печать статистики стрельбы в табличном виде.
 *
 * @param stats вектор статистики.
 */
void printStats(const vector<Stat>& stats) {
    cout << setw(17) << "Оружие" << setw(10) << "Дист(m)" << setw(10) << "Выстрелы" << setw(12) << "Попадания" << setw(12) << "Процент(%)"
        << setw(12) << "Скорострельность" << "\n";

    cout << string(71, '=') << "\n";
    cout << fixed << setprecision(2);
    for (auto& s : stats) {
        cout <<left << setw(17) << s.weapon <<right << setw(10) << s.dist << setw(10) << s.shots << setw(12) << s.hits << setw(12) << s.acc * 100.0
            << setw(12) << s.rof << "\n";
    }
}


int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    vector<Target> targets = { {25},{50},{100},{200},{400},{700} };
    ShootingRange range(targets);

    Pistol autoP;
    SingleRifle autoSR;
    PumpShotgun autoPS;
    AssaultRifle autoAR;
    SniperRifle autoSN;


    vector<Weapon*> allWeapons = {
        &autoP, &autoSR, &autoPS, &autoAR, &autoSN, 
    };
    for (auto w : allWeapons) range.addWeapon(w);

    auto stats = range.simulate(100);

    printWeaponInfo(allWeapons);
    printStats(stats);

    for (auto w : allWeapons) {
        cout << w << "\n";
    }

    return 0;
}
