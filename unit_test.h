#pragma once

namespace test {

const int SIZE = 1000;

#define TEST(x) \
if ((x)) {\
    std::cout << "PASS    " << __FUNCTION__ << std::endl;\
} else {\
    std::cout << "FAILED  " << __FUNCTION__ << std::endl;\
}

template <typename T>
void initailize(std::balanced_tree<T>& tree)
{
    for (int i = 0; i < SIZE; ++i) {
        tree.insert(i);
    }
}

} //namespace test

void default_constructor()
{
    std::balanced_tree<int> empty;

    TEST(empty.size() == 0 && empty.empty());
}

void initialize_list_constructor()
{
    std::balanced_tree<int> i_list_tree = {1, 2, 3, 4};
    std::vector<int> i_list_vec = {1, 2, 3, 4};

    assert(i_list_tree.size() == i_list_vec.size());
    TEST(std::equal(i_list_tree.begin(), i_list_tree.end(), i_list_vec.begin()));
}

void copy_constructor()
{
    std::balanced_tree<int> i_list_tree = {4, 3, 2, 1};
    std::balanced_tree<int> copy_tree(i_list_tree);
    std::vector<int> i_list_vec = {1, 2, 3, 4};

    assert(i_list_tree.size() == copy_tree.size());
    TEST(std::equal(copy_tree.begin(), copy_tree.end(), i_list_vec.begin()));
}

void copy_assignment()
{
    std::balanced_tree<int> initial;
    test::initailize(initial);

    assert(initial.size() == test::SIZE);
    std::balanced_tree<int> copy;

    copy = initial;
    assert(initial.size() == test::SIZE);

    TEST(std::equal(copy.begin(), copy.end(), initial.begin()));
}

void move_constructor()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    std::balanced_tree<int> moved(std::move(tree));
    assert(tree.empty());

    TEST(moved.size() == test::SIZE);
}

void move_assignement()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    std::balanced_tree<int> moved;
    moved = std::move(tree);
    assert(tree.empty());

    TEST(moved.size() == test::SIZE);
}

void clear()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    tree.clear();
    TEST(tree.size() == 0 && tree.empty());
}

