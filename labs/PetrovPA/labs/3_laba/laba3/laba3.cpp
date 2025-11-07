#include <iostream>
#include <vector>
#include <map>
#include <fstream> 
using namespace std;

struct Date {
	int day;
	int month;
	int year;
};

struct MediaFile {
	int id;
	string filename;
	double mb;
	string type;
	Date creation_date;
};

/**
 * Выводит на экран список медиафайлов.
 *
 * @param arr Вектор медиафайлов.
 */
void printArr(vector<MediaFile> arr) {
	for (auto media : arr) {
		printf("\n\nId: %d \nИмя: %s \nПамять: %f \nТип: %s \nДата: %d.%d.%d ",
			media.id, media.filename.c_str(), media.mb,
			media.type.c_str(), media.creation_date.day, media.creation_date.month, media.creation_date.year);
	}
}
/**
 * Генерирует случайную строку из строчных английских букв.
 *
 * @param n Длина строки.
 * @return Случайная строка длиной n.
 */
std::string generateRandomString(int n) {
	if (n < 0) { exit(1); }
	std::string str(n, ' ');
	for (int i = 0; i < n; i++) {
		str[i] = 'a' + (rand() % 26);
	}
	return str;
}

/**
 * Генерирует случайное целое число.
 *
 * @return Случайное целое число.
 */
int generateRandomInt() {
	return rand();
}

/**
 * Генерирует случайное вещественное число.
 *
 * @return Случайное число с плавающей точкой.
 */
double generateRandomDouble() {
	return rand() / 1.0;
}

/**
 * Случайным образом выбирает тип медиафайла из набора.
 *
 * @return Случайный тип медиафайла.
 */
string randomGenerateType() {
	string type[4] = { "Audio", "Video", "Image", "Document" };
	return type[rand() % 4];
}

/**
 * Генерирует случайную дату.
 *
 * @return Структура Date со случайными значениями.
 */
Date randomGenerateDate() {
	Date date;
	date.year = rand();
	date.month = rand() % 12 + 1;
	map<int, int> days = {
		{1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30},
		{7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
	};
	if (date.month == 2 && date.year % 4 == 0) {
		date.day = 29;
	}
	else {
		date.day = rand() % days[date.month] + 1;
	}
	return date;
}

/**
 * Генерирует вектор случайных медиафайлов.
 *
 * @param n Количество генерируемых медиафайлов.
 * @return Вектор структур MediaFile со случайными данными.
 */
vector<MediaFile> randomGenerateMediaFile(int n) {
	vector<MediaFile> vec;
	MediaFile med;
	for (int i = 0; i < n; ++i) {
		med.id = generateRandomInt();
		med.filename = generateRandomString(6);
		med.mb = generateRandomDouble();
		med.type = randomGenerateType();
		med.creation_date = randomGenerateDate();
		vec.push_back(med);
	}
	return vec;
}

/**
 * Фильтрует медиафайлы по типу и минимальному размеру.
 *
 * @param vec Вектор медиафайлов.
 * @param type Тип для фильтрации.
 * @param mb Минимальный размер.
 * @return Вектор медиафайлов, соответствующих условиям.
 */
vector<MediaFile> filter(vector<MediaFile> vec, string type, int mb) {
	vector<MediaFile> newVec;
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].type == type && mb < vec[i].mb) {
			newVec.push_back(vec[i]);
		}
	}
	return newVec;
}

/**
 * Выполняет поиск медиафайлов по имени.
 *
 * @param vec Вектор медиафайлов.
 * @param name Подстрока для поиска в имени файла.
 */
void searchName(vector<MediaFile> vec, string name) {
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].filename.find(name) != string::npos) {
			printf("Id: %d | name: %s", vec[i].id, vec[i].filename.c_str());
		}
	}
}

/**
 * Выводит количество медиафайлов по каждому типу.
 *
 * @param distr Словарь, где ключ — тип, значение — количество.
 */
void printCountType(map<string, int> distr) {
	for (auto m : distr) {
		printf("%s : %d\n", m.first.c_str(), m.second);
	}
}

/**
 * Подсчитывает количество медиафайлов каждого типа и выводит результат.
 *
 * @param vec Вектор медиафайлов.
 */
void distribution(vector<MediaFile> vec) {
	map<string, int> distr;
	for (auto media : vec) {
		++distr[media.type];
	}
	printCountType(distr);
}

/**
 * Сравнивает два медиафайла по дате.
 * Если даты совпадают — сравнение по имени.
 *
 * @param a Первый медиафайл.
 * @param b Второй медиафайл.
 * @return true, если a больше b, иначе false.
 */
bool great(const MediaFile& a, const MediaFile& b) {
	if (a.creation_date.year != b.creation_date.year) return a.creation_date.year > b.creation_date.year;
	if (a.creation_date.month != b.creation_date.month) return a.creation_date.month > b.creation_date.month;
	if (a.creation_date.day != b.creation_date.day) return a.creation_date.day > b.creation_date.day;
	return a.filename < b.filename;
}

/**
 * Сортировка вектора медиафайлов.
 *
 * @param vec Вектор медиафайлов.
 * @param left Левая граница.
 * @param right Правая граница.
 */
void sortMedia(vector<MediaFile>& vec, int left, int right) {
	int i = left, j = right;
	MediaFile mid = vec[(left + right) / 2];

	while (i <= j) {
		while (great(vec[i], mid)) i++;
		while (great(mid, vec[j])) j--;

		if (i <= j) {
			MediaFile tmp = vec[i];
			vec[i] = vec[j];
			vec[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j) sortMedia(vec, left, j);
	if (i < right) sortMedia(vec, i, right);
}

/**
 * Сортирует медиафайлы с использованием быстрой сортировки.
 *
 * @param vec Вектор медиафайлов.
 * @return Отсортированный вектор медиафайлов.
 */
vector<MediaFile> sortirovkaMedia(vector<MediaFile> vec) {
	if (!vec.empty()) {
		sortMedia(vec, 0, vec.size() - 1);
	}
	return vec;
}

/**
 * Сохраняет медиафайлы в файл "media.txt".
 *
 * @param vec Вектор медиафайлов.
 */
void saveMedia(vector<MediaFile> vec) {
	ofstream out("media.txt");
	for (int i = 0; i < vec.size(); i++) {
		out << vec[i].id << " "
			<< vec[i].filename << " "
			<< vec[i].mb << " "
			<< vec[i].type << " "
			<< vec[i].creation_date.day << " "
			<< vec[i].creation_date.month << " "
			<< vec[i].creation_date.year << "\n";
	}
	out.close();
}

/**
 * Загружает медиафайлы из файла "media.txt".
 *
 * @return Вектор загруженных медиафайлов.
 */
vector<MediaFile> loadMedia() {
	ifstream in("media.txt");
	vector<MediaFile> vec;
	MediaFile media;
	while (in >> media.id >> media.filename >> media.mb >> media.type
		>> media.creation_date.day >> media.creation_date.month >> media.creation_date.year) {
		vec.push_back(media);
	}

	in.close();
	cout << "\nЗагруженный файл:\n ";
	printArr(vec);
	return vec;
}



/**Меню выбора действий
*
*
*
*/
int menu() {
	cout << "\n1. Сгенерировать массив медиафайлов\n";
	cout << "2. Показать медиафайлы\n";
	cout << "3. Отсортировать медиафайлы\n";
	cout << "4. Поиск по имени\n";
	cout << "5. Фильтрация по типу и размеру\n";
	cout << "6. Показать распределение по типам\n";
	cout << "7. Сохранить в файл\n";
	cout << "8. Загрузить из файла\n";
	cout << "0. Выход\n";
	int choice;
	cin >> choice;
	return choice;
}

int main() {
	setlocale(LC_ALL, "RUS");
	srand(time(NULL));

	vector<MediaFile> arr, newArr, fileArr;      
	int choice;
	while (true) {
		choice = menu();
		if (choice == 0) {
			break;
		}

		switch (choice) {
		case 1: {
			int n;
			cout << "Число N: ";
			cin >> n;
			arr = randomGenerateMediaFile(n);
			break;
		}
		case 2:
			if (arr.empty()) cout << "Нет данных.\n";
			else printArr(arr);
			break;
		case 3:
			if (!arr.empty()) {
				arr = sortirovkaMedia(arr);
			}
			else cout << "Нет данных.\n";
			break;
		case 4: {
			if (arr.empty()) { cout << "Нет данных.\n"; break; }
			string name;
			cout << "Имя: ";
			cin >> name;
			searchName(arr, name);
			break;
		}
		case 5: {
			if (arr.empty()) { cout << "Нет данных.\n"; break; }
			string type;
			double mb;
			cout << "Введите тип: ";
			cin >> type;
			cout << "Размер: ";
			cin >> mb;
			newArr = filter(arr, type, mb);
			printArr(newArr);
			break;
		}
		case 6:
			if (arr.empty()) cout << "Нет данных.\n";
			else distribution(arr);
			break;
		case 7:
			if (!arr.empty()) {
				saveMedia(arr);
			}
			else cout << "Нет данных.\n";
			break;
		case 8:
			fileArr = loadMedia();
			printArr(fileArr);
			break;
		default:
			cout << "Неверный выбор.\n";
			break;
		}
	}
	return 0;
}
