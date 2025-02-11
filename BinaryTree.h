#pragma once

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <cstring>
#include <iostream>
#include <locale>
#include <set>
#include <vector>

#include "common.hpp"

using namespace std;


template <typename T>
class BinaryTree {
    struct Node;
    struct Operation {
        virtual void apply(Node *n) = 0;
    };
    class Node {
        T value;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *next = nullptr;
        int height = 1;
        int dis = 0;

        void reCalc() {
            int leftHeight = (left) ? left->height : 0;
            int rightHeight = (right) ? right->height : 0;
            height = std::max(leftHeight, rightHeight) + 1;
            dis = leftHeight - rightHeight;
        }

        explicit Node(T value, Node *left = nullptr, Node *right = nullptr) : value(value), left(left), right(right) {
            reCalc();
        }

        // 1. КЛП
        void NLR(Operation &op) {
            op.apply(this);
            if (left) left->NLR(op);
            if (right) right->NLR(op);
        }
        // 2. КПЛ
        void NRL(Operation &op) {
            op.apply(this);
            if (right) right->NRL(op);
            if (left) left->NRL(op);
        }
        // 3. ЛПК
        void LRN(Operation &op) {
            if (left) left->LRN(op);
            if (right) right->LRN(op);
            op.apply(this);
        }
        // 4. ЛКП
        void LNR(Operation &op) {
            if (left) left->LNR(op);
            op.apply(this);
            if (right) right->LNR(op);
        }
        // 5. ПЛК
        void RLN(Operation &op) {
            if (right) right->RLN(op);
            if (left) left->RLN(op);
            op.apply(this);
        }
        // 6. ПКЛ
        void RNL(Operation &op) {
            if (right) right->RNL(op);
            op.apply(this);
            if (left) left->RNL(op);
        }
        void print(const char *format, std::ostream &os) {
            if (!left && !right) {
                os << value;
                return;
            }
            for (size_t i = 0, len = strlen(format); i < len; i++) {
                switch (format[i]) {
                    case 'N':
                        os << value;
                        break;
                    case 'L':
                        if (left) left->print(format, os);
                        break;
                    case 'R':
                        if (right) right->print(format, os);
                        break;
                    default:
                        os << format[i];
                }
            }
        }

    };

private:
    Node *root = nullptr;  // Корень дерева
    int size = 0;          // Количество узлов в дереве

    void delTree(Node *tree) {
        if (tree == nullptr) return;
        delTree(tree->left);
        delTree(tree->right);
        delete tree;
    }

    void NLR(Node *n, Operation &op) {
        if (n == nullptr) return;
        op.apply(n);
        NLR(n->left, op);
        NLR(n->right, op);
    }
    // Обход в заданном порядке
    void go(Node *n, const char *order, Operation &op) {
        if (n == nullptr) return;
        for (size_t i = 0, len = strlen(order); i < len; i++) {
            switch (order[i]) {
                case 'L':
                    go(n->left, order, op);
                    break;
                case 'R':
                    go(n->right, order, op);
                    break;
                case 'N':
                    op.apply(n);
                    break;
            }
        }
    }

    Node *copy(Node *n) {
        if (n == nullptr) return nullptr;
        return new Node(n->value, copy(n->left), copy(n->right));
    }
    // Вставка

    Node *insertTo(Node *n, const T &v) {
        if (v <= n->value) {
            if (n->left)
                n->left = insertTo(n->left, v);
            else
                n->left = new Node(v);
        } else {
            if (n->right) {
                n->right = insertTo(n->right, v);
            } else
                n->right = new Node(v);
        }
        return balance(n);
    }
    explicit BinaryTree(Node *root) : root(root) {
        this->size = subTreeSize(root);
        check(root);
    }

public:
    BinaryTree() = default;
    BinaryTree(const T *items, const int size) {
        for (int i = 0; i < size; i++) insert(items[i]);
    }
    explicit BinaryTree(const std::set<T> &set) {
        for (T x : set) insert(x);
    }
    ~BinaryTree() {
        delTree(root);
    }
    int getSize() const {
        return size;
    }
    Node *getRoot() {
        return root;
    }

    void insert(const T &value) {
        size++;
        if (root) {
            root = insertTo(root, value);
        } else
            root = new Node(value);
    }
    // Поиск узла по значению
    Node *find(const T &v) const {
        Node *n = root;
        while (n != nullptr) {

            if (v == n->value) return n;
            if (v < n->value)
                n = n->left;
            else
                n = n->right;
        }
        return nullptr;
    }
    // Удаление узла по значению

    Node *remove(Node *r, const T &v) {

        if (r == nullptr) return nullptr;

        if (v < r->value) {
            r->left = remove(r->left, v);
            return balance(r);
        }
        if (v > r->value) {
            r->right = remove(r->right, v);
            return balance(r);
        }

        assert(v == r->value);
        if (r->left && r->right) {
            r->value = minimum(r->right)->value;
            r->right = remove(r->right, r->value);
        } else {
            Node *toDelete = r;
            if (r->left) {
                r = r->left;
            } else if (r->right) {
                r = r->right;
            } else
                r = nullptr;
            size--;
            delete toDelete;
        }
        return balance(r);
    }
    // Удаление узла по значению
    void remove(const T &v) {
        root = remove(root, v);
    }

    int height() const {
        return root ? root->height : 0;
    }

    int height(Node *n) {
        if (!n) return 0;

        return max(height(n->left), height(n->right)) + 1;
    }
    Node *getRight() {
        return root->right;
    }
    Node *getLeft() {
        return root->left;
    }
    Node get(){
        return root->value;
    }

    Node *minimum(Node *n) {
        if (n == nullptr) throw range_error("Empty tree");
        while (n->left) n = n->left;
        return n;
    }
    Node *minimum() {
        return minimum(root);
    }
    Node *maximum(Node *n) {
        if (n == nullptr) throw range_error("Empty tree");
        while (n->right) n = n->right;
        return n;
    }
    Node *maximum() {
        return maximum(root);
    }
    T getMin(Node *n) {
        return minimum(n)->value;
    }
    T getMax(Node *n) {
        return maximum(n)->value;
    }
    // Поддерево
    BinaryTree<T> *subTree(const T &v) {
        return new BinaryTree<T>(copy(find(v)));
    }

    int subTreeSize(const Node *n) {
        if (n)
            return subTreeSize(n->left) + 1 + subTreeSize(n->right);
        else
            return 0;
    }

    int calcSize() {
        return subTreeSize(root);
    }
    // Сравнение деревьев
    bool matchTree(const Node *a, const Node *b) {
        if (a == nullptr && b == nullptr) return true;
        if (a == nullptr || b == nullptr) return false;
        if (a->value != b->value) return false;
        return matchTree(a->left, b->left) && matchTree(a->right, b->right);
    }
    bool match(BinaryTree<T> &tree) {
        return matchTree(root, tree.root);
    }
    bool subTreeCheck(BinaryTree<T> *subTree) {
        if (subTree == nullptr || subTree->root == nullptr) return false;
        Node *n = find(subTree->root->value);
        if (!n) return false;
        return matchTree(subTree->root, n);
    }
    struct Print : public Operation {
        stringstream ss;
        void apply(Node *n) override {
            ss << n->value << " ";
        }
        string result() {
            return trim_copy(ss.str());
        }
    };

    // 1. КЛП
    string toNLR() {
        Print op;
        if (root) root->NLR(op);
        return op.result();
    }
    // 2. КПЛ
    string toNRL() {
        Print op;
        if (root) root->NRL(op);
        return op.result();
    }
    // 3. ЛПК
    string toLRN() {
        Print op;
        if (root) root->LRN(op);
        return op.result();
    }
    // 4. ЛКП
    string toLNR() {
        Print op;
        if (root) root->LNR(op);
        return op.result();
    }
    // 5. ПЛК
    string toRLN() {
        Print op;
        if (root) root->RLN(op);
        return op.result();
    }
    // 6. ПКЛ
    string toRNL() {
        Print op;
        if (root) root->RNL(op);
        return op.result();
    }
    // Вывод строку в соответствии с форматом
    string toString(const char *format) {
        stringstream ss;
        if (root) root->print(format, ss);
        return trim_copy(ss.str());
    }
    // map, reduce, where

    BinaryTree<T> map(T f(T)) {
        BinaryTree<T> res;
        for (T x : *this) {
            res.insert(f(x));
        }
        return res;
    }

    BinaryTree<T> where(bool h(T)) {
        BinaryTree<T> res;
        for (T x : *this) {
            if (h(x)) res.insert(x);
        }
        return res;
    }

    T reduce(T f(T, T)) {
        return reduce(root, f);
    }
    T reduce(Node *n, T f(T, T)) {
        if (n->left) {
            T value = f(n->value, reduce(n->left, f));
            if (n->right) {
                return f(value, reduce(n->right, f));
            }
            return value;
        }
        if (n->right) {
            return f(n->value, reduce(n->right, f));
        }
        return n->value;
    }

    Node *first;
    // Прошивка
    struct Thread : public Operation {
        Node *first = nullptr;
        Node *last = nullptr;
        void apply(Node *n) override {
            if (first == nullptr) first = n;
            if (last != nullptr) last->next = n;
            n->next = nullptr;

            last = n;
        };
    };

    // Прошивка дерева в порядке Корень Левое Правое
    Node *thread() {
        first = nullptr;
        Thread td;
        NLR(root, td);
        first = td.first;
        return first;
    }

    // Прошивка дерева в заданном порядке
    Node *thread(const char *order) {
        assert(strlen(order) == 3);
        first = nullptr;
        Thread td;
        go(root, order, td);
        first = td.first;
        return first;
    }
    // Проверка разницы высот для данной вершины
    int disbalanceСheck(Node *t) {
        if (t == nullptr) return 0;
        return height(t->left) - height(t->right);
    }
    // Проверка правильности сбалансированного дерева
    void checkBeforeBalance(Node *n) {
        assert(n->height == height(n));
        assert(n->dis == disbalanceСheck(n));
        assert(n->dis <= 1);
        assert(n->dis >= -1);
        if (n->left) {
            assert(n->left->value <= n->value);
            check(n->left);
        }
        if (n->right) {
            assert(n->value < n->right->value);
            check(n->right);
        }
    }
    void check() {
        check(root);
    }
    void check(Node *n) {
        checkBeforeBalance(n);
        assert(n->dis >= -1);
        assert(n->dis <= 1);
    }
    Node *balance(Node *a) {
        if (a == nullptr) return nullptr;
        a->reCalc();

        if (a->dis <= -2) {
            if (a->right->dis <= 0)
                a = leftRotation(a);
            else
                a = bigLeftRotation(a);
        } else if (a->dis >= 2) {
            if (a->left->dis >= 0)
                a = rightRotation(a);
            else
                a = bigRightRotation(a);
        }

        return a;
    }

    // Малое левое вращение
    Node *leftRotation(Node *a) {

        Node *b = a->right;
        a->right = b->left;
        a->reCalc();
        b->left = a;
        b->reCalc();
        return b;
    }
    // Малое правое вращение
    Node *rightRotation(Node *a) {

        assert(a->left != nullptr);
        Node *b = a->left;
        a->left = b->right;
        a->reCalc();
        b->right = a;
        b->reCalc();
        return b;
    }
    // Большое левое вращение
    Node *bigLeftRotation(Node *a) {
        Node *b = a->right;
        a->right = rightRotation(b);
        return leftRotation(a);
    }
    // Большое правое вращение
    Node *bigRightRotation(Node *a) {
        Node *newRoot = a->left;
        a->left = leftRotation(newRoot);
        return rightRotation(a);
    }
    string print(Node *n) {
        if (n) {
            return to_string(n->value) + " " + to_string(n->height) + "/" + to_string(n->dis);
        } else {
            return "";
        }
    }
    void printTree(BinaryTree<T> tree)
    {
        int space = 0;
        if (tree->getRoot == nullptr)
        {
            return;
        }
        space += tree->height;
        printTree(tree->getRight(), space, tree->height);
        std::cout << std::endl;
        for (int i = tree->height; i < space; ++i)
        {
            std::cout << " ";
        }
        std::cout <<tree->get() << "\n";

        printTree(tree->getLeft(), space, tree->height);
    }








    // Одна клеточка для представления узла при рисовании
    struct CellDisplay {
        bool present;   // Существует ли узел?
        string valStr;  // Значение в виде строки
        explicit CellDisplay(Node *n) : present(n) {
            valStr = (n) ? to_string(n->value) : "";

        };
    };

    // Таблица с представлением ячеек
    vector<vector<CellDisplay>> getDisplayRows() const {

        vector<Node *> traversal_stack;
        if (!root) return vector<vector<CellDisplay>>();
        vector<vector<Node *>> nodes;  // Узлы дерева по уровням
        const int height = root->height;
        nodes.resize(height);
        Node *p = root;
        int depth = 0;
        while (true) {

            if (depth == height - 1) {
                nodes[depth].push_back(p);
                if (depth == 0) break;
                --depth;
                continue;
            }

            if (traversal_stack.size() == depth) {
                nodes[depth].push_back(p);
                traversal_stack.push_back(p);
                if (p) p = p->left;
                ++depth;
                continue;
            }

            if (nodes[depth + 1].size() % 2) {
                p = traversal_stack.back();
                if (p) p = p->right;
                ++depth;
                continue;
            }

            if (depth == 0) break;
            traversal_stack.pop_back();
            p = traversal_stack.back();
            --depth;
        }
        // Преобразуем Node* в CellDisplay
        vector<vector<CellDisplay>> display;
        for (const auto &row : nodes) {
            display.emplace_back();  // Создаём новую строчку
            for (Node *n : row) display.back().push_back(CellDisplay(n));
        }
        return display;
    }


    vector<string> row_formatter(const vector<vector<CellDisplay>> &rows) const {
        using s_t = string::size_type;

        s_t cell_width = 0;
        for (const auto &row : rows)
            for (const auto &cd : row)
                if (cd.present && cd.valStr.length() > cell_width) {
                    cell_width = cd.valStr.length();
                };

        if (cell_width % 2 == 0) ++cell_width;

        vector<string> formatted_rows;

        s_t row_count = rows.size();

        s_t row_elem_count = 1 << (row_count - 1);

        s_t left_pad = 0;

        for (s_t r = 0; r < row_count; ++r) {
            const auto &cd_row = rows[row_count - r - 1];

            s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;

            string row;

            for (s_t c = 0; c < row_elem_count; ++c) {

                row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
                if (cd_row[c].present) {

                    const string &valstr = cd_row[c].valStr;

                    s_t long_padding = cell_width - valstr.length();
                    s_t short_padding = long_padding / 2;
                    long_padding -= short_padding;
                    row += string(c % 2 ? short_padding : long_padding, ' ');
                    row += valstr;
                    row += string(c % 2 ? long_padding : short_padding, ' ');
                } else {

                    row += string(cell_width, ' ');
                }
            }

            formatted_rows.push_back(row);

            if (row_elem_count == 1) break;

            s_t left_space = space + 1;
            s_t right_space = space - 1;
            for (s_t sr = 0; sr < space; ++sr) {
                string row;
                for (s_t c = 0; c < row_elem_count; ++c) {
                    if (c % 2 == 0) {
                        row += string(c ? left_space * 2 + 1 : left_space, ' ');
                        row += cd_row[c].present ? '/' : ' ';
                        row += string(right_space + 1, ' ');
                    } else {
                        row += string(right_space, ' ');
                        row += cd_row[c].present ? '\\' : ' ';
                    }
                }
                formatted_rows.push_back(row);
                ++left_space;
                --right_space;
            }
            left_pad += space + 1;
            row_elem_count /= 2;
        }

        std::reverse(formatted_rows.begin(), formatted_rows.end());
        return formatted_rows;
    }

    static void trim_rows_left(vector<string> &rows) {
        if (rows.empty()) return;
        auto min_space = rows.front().length();
        for (const auto &row : rows) {
            auto i = row.find_first_not_of(' ');
            if (i == string::npos) i = row.length();
            if (i == 0) return;
            if (i < min_space) min_space = i;
        }
        for (auto &row : rows) {
            row.erase(0, min_space);
        }
    }

    void printAsTree() const {
        wcout << endl;
        if (root == nullptr) { 
            wcout << " <empty tree>" << endl;
            return;
        }
        // Получаем таблицу из ячеек
        const auto rows = getDisplayRows();
        // Генерируем массив отформатированных строк
        auto formatted = row_formatter(rows);
        // Обрезаем лишние пробелы слева
        trim_rows_left(formatted);
        // Выводим на консоль как комментарии
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        // std::string narrow = converter.to_bytes(wide_utf16_source_string);
        for (const auto &row : formatted) {
            std::wstring wide = converter.from_bytes(row);
            wcout << L" " << wide << endl;
        }
    }
    // Итератор для BinaryTree
    struct Iterator {
        using pointer = T *;
        using reference = T &;

        // Создаём обход и запоминаем индекс
        explicit Iterator(Node *node, int index) : index(index), root(node) {
            NLR(node);
        }
        reference operator*() const {
            return path[index]->value;
        }
        pointer operator->() {
            return path[index]->value;
        }
        Iterator &operator++() {
            index++;
            if (index > path.size()) index = path.size();
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.index == b.index && a.root == b.root;
        };
        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.index != b.index || a.root != b.root;
        };

    private:
        // Node Left Right - Корень Левое Правое
        void NLR(Node *node) {
            if (node == nullptr) return;
            path.push_back(node);
            if (node->left != nullptr) NLR(node->left);
            if (node->right != nullptr) NLR(node->right);
        }
        std::vector<Node *> path;
        int index;
        Node *root;
    };
    Iterator begin() const {
        return Iterator(root, 0);
    }
    Iterator end() const {
        return Iterator(root, getSize());
    }
};
