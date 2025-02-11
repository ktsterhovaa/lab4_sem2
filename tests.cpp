

#include <chrono>
#include <complex>
#include <cstdlib>
#include "../BinaryTree.h"
#include "lib/googletest/include/gtest/gtest.h"



using namespace std;




constexpr int sum(int a, int b) {
    return a + b;
}

TEST(Tree, reduce) {
    static_assert(sum(1, 2) == 3, "1 + 2 = 3");
    static_assert(sum(11, 22) == 33, "11 + 22 = 33");
    static_assert(sum(1000, -203) == 797, "1000 - 203 = 797");
}



TEST(BinaryTree, smallTree) {
    BinaryTree<int> bt;
    ASSERT_EQ(0, bt.getSize());
    bt.insert(8);
    ASSERT_EQ(string("8"), bt.toLNR());
    ASSERT_EQ(string("8"), bt.toString("N L R"));
    ASSERT_EQ(8, bt.reduce(sum));
    bt.insert(3);
    ASSERT_EQ(string("3 8"), bt.toLNR());
    ASSERT_EQ(string("3 8"), bt.toString("L N R"));
    ASSERT_EQ(3 + 8, bt.reduce(sum));
    bt.insert(10);
    ASSERT_EQ(2, bt.height());

    // 1. КЛП
    ASSERT_EQ(string("8 3 10"), bt.toNLR());
    ASSERT_EQ(string("8 3 10"), bt.toString("N L R"));
    ASSERT_EQ(string("8-3+10"), bt.toString("N-L+R"));
    ASSERT_EQ(string("(8)[3]{10}"), bt.toString("(N)[L]{R}"));
    // 2. КПЛ
    ASSERT_EQ(string("8 10 3"), bt.toNRL());
    ASSERT_EQ(string("8 10 3"), bt.toString("N R L"));
    // 3. ЛПК
    ASSERT_EQ(string("3 10 8"), bt.toLRN());
    // 4. ЛКП
    ASSERT_EQ(string("3 8 10"), bt.toLNR());
    // 5. ПЛК
    ASSERT_EQ(string("10 3 8"), bt.toRLN());
    // 6. ПКЛ
    ASSERT_EQ(string("10 8 3"), bt.toRNL());
    bt.insert(1);
    ASSERT_EQ(string("1 3 8 10"), bt.toLNR());
    ASSERT_EQ(string("1 3  8 10"), bt.toString("L N R"));

    bt.insert(6);
    ASSERT_EQ(string("1 3 6 8 10"), bt.toLNR());
    bt.insert(4);
    ASSERT_EQ(string("1 3 4 6 8 10"), bt.toLNR());
    bt.insert(7);
    ASSERT_EQ(string("1 3 4 6 7 8 10"), bt.toLNR());
    bt.insert(14);
    ASSERT_EQ(string("1 3 4 6 7 8 10 14"), bt.toLNR());
    bt.insert(13);
    ASSERT_EQ(string("1 3 4 6 7 8 10 13 14"), bt.toLNR());
    ASSERT_EQ(string("6 3 1 4 8 7 13 10 14"), bt.toNLR());

    // Извлечение поддерева (по заданному корню)
    BinaryTree<int> *subTree = bt.subTree(13);
    ASSERT_EQ(string("13 10 14"), subTree->toNLR());
    // Сравнение деревьев
    int data[] = {14};
    BinaryTree<int> bt2(data, 1);
    ASSERT_FALSE(subTree->match(bt2));
    bt2.insert(10);
    bt2.insert(13);
    ASSERT_EQ(string("13 10 14"), bt2.toNLR());
    ASSERT_TRUE(subTree->match(bt2));

    ASSERT_TRUE(bt.subTreeCheck(subTree));
    delete subTree;
}




TEST(BinaryTree, baseOperationsInsertFindRemove) {
    BinaryTree<int> bt;
    ASSERT_EQ(0, bt.getSize());

    vector<int> vec;
    const int SIZE = 1000;
    for (int i = 0; i < SIZE; i++) {

        int value = (i * 17 + 13) % SIZE;
        ASSERT_FALSE(bt.find(value));
        bt.insert(value);
        vec.push_back(value);
        ASSERT_TRUE(bt.find(value));
        ASSERT_EQ(vec.size(), bt.getSize());
        for (int x : vec) {
            ASSERT_TRUE(bt.find(x));
        }

        auto minValue = *min_element(vec.begin(), vec.end());
        auto maxValue = *max_element(vec.begin(), vec.end());
        ASSERT_EQ(minValue, bt.minimum()->value);
        ASSERT_EQ(minValue, bt.getMin(bt.getRoot()));
        ASSERT_EQ(maxValue, bt.maximum()->value);
        ASSERT_EQ(maxValue, bt.getMax(bt.getRoot()));

        while (bt.getSize() > (rand() % 100)) {

            int randomIndex = rand() % bt.getSize();
            int v = vec[randomIndex];
            bt.remove(v);
            vec.erase(vec.begin() + randomIndex);
            ASSERT_EQ(vec.size(), bt.getSize());
            ASSERT_EQ(vec.size(), bt.calcSize());
            for (int x : vec) {
                ASSERT_TRUE(bt.find(x));
            }
        }
    }
}




TEST(BinaryTree, printAsTree) {
    BinaryTree<int> bt{3, 2, 1};
    ASSERT_EQ(2, bt.height(bt.getRoot()));
    ASSERT_EQ(0, bt.disbalanceСheck(bt.getRoot()));
    bt.check(bt.getRoot());
    ASSERT_EQ(2, bt.height(bt.getRoot()));
    ASSERT_EQ(0, bt.disbalanceСheck(bt.getRoot()));

    bt.printAsTree();
    bt.insert(0);
    bt.check();

    ASSERT_EQ(3, bt.height(bt.getRoot()));
    ASSERT_EQ(1, bt.disbalanceСheck(bt.getRoot()));
    bt.insert(4);
    bt.check();

    ASSERT_EQ(3, bt.height(bt.getRoot()));
    ASSERT_EQ(0, bt.disbalanceСheck(bt.getRoot()));
    bt.printAsTree();
}


