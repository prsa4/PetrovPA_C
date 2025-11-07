#include <iostream>
#include <ctime>
#include <string>
#include <map>
#include <stdio.h>

/*
 * Генерирует случайную строку из строчных букв
 *
 * @param n длина строки.
 * @return возвращает строку из случайных строчных букв.
 */
std::string generateRandomString(int n) {
    if (n < 0) { exit(1); }
    std::string str(n, ' ');
    for (int i = 0;i < n;i++) {
        str[i] = 'a' + (rand() % 26);
    }
    return str;
}

/*
 * Считывает и проверяет входную строку
 *
 * @param stroka входная строка.
 * @return возвращает проверенную строку, содержащую только буквы.
 */
std::string newString(std::string stroka = "") {
    std::cin >> stroka;
    for (char c : stroka) {
        if (!(c >= 'a' && c <= 'z')) {
            exit(1);
        }
    }
    return stroka;
}

/*
 * Создает map частот символов в строке
 *
 * @param str входная строка.
 * @return возвращает map, где ключи — символы, а значения — их частоты.
 */
std::map<char, int> createMap(std::string str) {
    std::map<char, int> map;
    for (char c : str) {
        ++map[c];
    }
    return map;
}

/*
 * Выводит частоту каждого символа
 *
 * @param map словарь с частотами символов.
 */
void countLetters(std::map<char, int> map) {
    for (auto item : map) {
        printf("Символ: %c = %i\n", item.first, item.second);
    }
}

/*
 * Находит максимальную частоту символа
 *
 * @param map словарь с частотами символов.
 * @return возвращает максимальное значение частоты.
 */
int findMostFrequent(std::map<char, int> map) {
    int max = 0;
    for (auto item : map) {
        if (item.second > max) { max = item.second; }
    }
    return max;
}

/*
 * Выводит гистограмму частот символов
 *
 * @param map словарь с частотами символов.
 */
void printHistogram(std::map<char, int> map) {
    for (auto items : map) {
        std::cout << std::string(items.second, '*') << '\n';
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));
    int len;
    std::cout << "Введите размер: ";
    std::cin >> len;
    std::string stroka;
    stroka = generateRandomString(len);

    std::cout << "\nСтрока: " << stroka << std::endl;
    std::map<char, int> mapString = createMap(stroka);
    countLetters(mapString);
    std::cout << "макс:" << findMostFrequent(mapString) << '\n';
    printHistogram(mapString);
    return 1;
}