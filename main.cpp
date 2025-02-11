#include <fcntl.h>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include "binarytree.h"
#include "common.hpp"
#include "menu.h"


using namespace std;


template <typename T>
T mapFunction(T x) {
    return x * x;
}

std::wstring mapFunction(std::wstring x) {
    return x + L"!";
}


bool whereFunction(int x) {
    bool result = (x % 2) == 0;
    std::wcout << L"  where: число " << x << L" чётное: " << result << std::endl;
    return result;
}

bool whereFunction(double x) {
    bool result = abs(x) < 5;
    std::wcout << L"  where: число |" << x << L"| < 5: " << result << std::endl;
    return result;
}

bool whereFunction(std::complex<double> x) {
    bool result = abs(x) < 5;
    std::wcout << L"  where: число |" << x << L"| < 5: " << result << std::endl;
    return result;
}

bool whereFunction(const std::wstring x) {
    bool result = x.size() < 4;
    std::wcout << L"  where: длина строки " << x << L" < 4: " << result << std::endl;
    return result;
}


template <typename T>
T reduceFunction(T a, T b) {
    T result = a + b;
    std::wcout << L"  reduce: " << a << L" + " << b << " = " << result << std::endl;
    return result;
}



#define PRINT(x) wprintf(L"%s = %d\n", #x, x);

template <typename T>
void treeBaseOperations(BinaryTree<T> &tree) {
    while (true) {
        tree.printAsTree();
        wcout << L"Операции с деревом:" << endl;
        wcout << L"1. Вставка" << endl;
        wcout << L"2. Удаление" << endl;
        wcout << L"3. Поиск" << endl;
        wcout << L"0. Выход" << endl;
        int choice;
        wcin >> choice;
        T element;
        switch (choice) {
            case 1:
                wcout << L"Вставка: введите элемент для вставки: ";
                wcin >> element;
                tree.insert(element);
                break;
            case 2:
                wcout << L"Удаление: введите элемент для удаления: ";
                wcin >> element;
                tree.remove(element);
                break;
            case 3:
                wcout << L"Поиск: введите элемент для поиска: ";
                wcin >> element;
                if (tree.find(element)) {
                    wcout << L"Элемент найден" << endl;
                } else {
                    wcout << L"Элемент не найден" << endl;
                }
                break;
            case 0:
                return;
            default:
                wcout << L"Нет такого пункта меню" << endl;
        }
    }
}

template <typename T>
void applyMapWhereReduce(BinaryTree<T> &tree) {

    if (tree.getRoot() == nullptr) {
        wcout << L"Пустое дерево. Введите значения." << endl;
        wcout << L"Введите количество элементов: ";
        int N;
        wcin >> N;
        for (int i = 1; i <= N; i++) {
            T element;
            wcout << L"Введите элемент номер " << i << ": ";
            wcin >> element;
            tree.insert(element);
        }
        tree.printAsTree();
    }

    wprintf(L"Применяем операцию map\n");
    BinaryTree<T> mapRes = tree.map(mapFunction);
    mapRes.printAsTree();

    wprintf(L"Применяем операцию where\n");
    BinaryTree<T> whereRes = tree.where(whereFunction);
    whereRes.printAsTree();

    wprintf(L"Применяем операцию reduce\n");
    T reduceRes = tree.reduce(reduceFunction);
    std::wcout << L"Результат reduce: " << reduceRes << std::endl << std::endl;

    wcout << endl;
}

template <typename T>
BinaryTree<T> inputTree() {
    BinaryTree<T> tree;
    wcout << L"Введите количество элементов: ";
    int N;
    wcin >> N;
    for (int i = 1; i <= N; i++) {
        T element;
        wcout << L"Введите элемент номер " << i << ": ";
        wcin >> element;
        tree.insert(element);
    }
    tree.printAsTree();
    return tree;
}

template <typename T>
void threadedByFixedTraversal(BinaryTree<T> &tree) {

    if (tree.getRoot() == nullptr) {
        wcout << L"Пустое дерево. Введите значения." << endl;
        wcout << L"Введите количество элементов: ";
        int N;
        wcin >> N;
        for (int i = 1; i <= N; i++) {
            T element;
            wcout << L"Введите элемент номер " << i << ": ";
            wcin >> element;
            tree.insert(element);
        }
        tree.printAsTree();
    }


    tree.thread();

    wprintf(L"Выводим прошивку - узлы в порядке прошивки\n");
    for (auto cur = tree.first; cur != nullptr; cur = cur->next) {
        wcout << cur->value << endl;
    }
    wcout << endl;
}

template <typename T>
void threadedByAnyTraversal(BinaryTree<T> &tree) {

    if (tree.getRoot() == nullptr) {
        wcout << L"Пустое дерево. Введите значения." << endl;
        wcout << L"Введите количество элементов: ";
        int N;
        wcin >> N;
        for (int i = 1; i <= N; i++) {
            T element;
            wcout << L"Введите элемент номер " << i << ": ";
            wcin >> element;
            tree.insert(element);
        }
        tree.printAsTree();
    }

    wcout << L"Введите порядок обхода N-корень L-левое поддерево R-правое, например LNR: ";
    wstring order;
    wcin >> order;
    wprintf(L"Прошиваеем дерево в заданном порядке обхода\n");
    string s(order.begin(), order.end());
    tree.thread(s.c_str());

    wprintf(L"Выводим прошивку - узлы в порядке прошивки\n");
    for (auto cur = tree.first; cur != nullptr; cur = cur->next) {
        wcout << cur->value << endl;
    }
    wcout << endl;
}

wstring toWS(string s) {
    return wstring(s.begin(), s.end());
}
string toS(wstring s) {
    return string(s.begin(), s.end());
}

template <typename T>
void toStringInFixedTraversal(BinaryTree<T> &tree) {
    wcout << L"Дерево:" << endl;
    tree.printAsTree();
    wcout << L"1. КЛП = Корень Левый Правый: " << toWS(tree.toNLR()) << endl;
    wcout << L"2. КПЛ = Корень Правый Левый: " << toWS(tree.toNRL()) << endl;
    wcout << L"3. ЛПК = Левый Правый Корень: " << toWS(tree.toLRN()) << endl;
    wcout << L"4. ЛКП = Левый Корень Правый: " << toWS(tree.toLNR()) << endl;
    wcout << L"5. ПЛК = Правый Левый Корень: " << toWS(tree.toRLN()) << endl;
    wcout << L"6. ПКЛ = Правый Корень Левый: " << toWS(tree.toRNL()) << endl;
}

template <typename T>
void toStringInAnyTraversal(BinaryTree<T> &tree) {
    tree.printAsTree();

    wcout << L"Введите порядок обхода N-корень L-левое поддерево R-правое, например LNR: ";
    wstring order;
    wcin >> order;
    wcout << L"Вывод в заданном порядке" << endl;
    wcout << toWS(tree.toString(toS(order).c_str())) << endl;
    wcout << endl;
}

template <typename T>
void treeExtractSubtree(BinaryTree<T> &tree) {
    tree.printAsTree();
    if (tree.getRoot() == nullptr) {
        wcout << L"Пустое дерево. Введите значения." << endl;
        wcout << L"Введите количество элементов: ";
        int N;
        wcin >> N;
        for (int i = 1; i <= N; i++) {
            T element;
            wcout << L"Введите элемент номер " << i << ": ";
            wcin >> element;
            tree.insert(element);
        }
        tree.printAsTree();
    }
    T key;
    wcout << L"Значение в корне поддерева: ";
    wcin >> key;
    BinaryTree<T> *res = tree.subTree(key);
    res->printAsTree();
    delete res;
    wcout << endl;
}

template <typename T>
void treeFindSubtree(BinaryTree<T> &tree) {
    tree.printAsTree();
    BinaryTree<T> subTreeToFind = inputTree<T>();
    if (tree.subTreeCheck(&subTreeToFind)) {
        wcout << L"Поддерево найдено" << endl;
    } else {
        wcout << L"Поддерево не найдено" << endl;
    };
}




template <typename T>
void treeMenu() {
    BinaryTree<T> bt;
    MenuX<BinaryTree<T>> m{L"Возможные операции с деревом",
                           { {L"Базовые операции: вставка, поиск, удаление + Балансировка", treeBaseOperations<T>},
                             {L"Применение функции map, where, reduce - ручной ввод данных", applyMapWhereReduce<T>},
                             {L"Прошивка по фиксированному обходу", threadedByFixedTraversal<T>},
                             {L"Прошивка по обходу, задаваемому параметром метода", threadedByAnyTraversal<T>},
                             {L"Сохранение в строку по фиксированному обходу", toStringInFixedTraversal<T>},
                             {
                                     L"Сохранение в строку по обходу, задаваемому строкой форматирования",
                                     toStringInAnyTraversal<T>
                             },
                             {L"Извлечение поддерева (по заданному корню)", treeExtractSubtree<T>},
                             {L"Поиск на вхождение поддерева", treeFindSubtree<T>}
                           },
                           bt};
}



void binaryTreeMenu() {
    Menu m{
            L"Выберите тип элементов ",
            {{L"Целые числа (int)", treeMenu<int>}, {L"Вещественные числа (double)", treeMenu<double>}}

    };
}

int main() {
#if WIN32 || WIN64
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
#endif

    binaryTreeMenu();

}
