#pragma once

namespace test {

const int SIZE = 1000;

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
    if (empty.size() == 0 && empty.empty()) {
        std::cout << "PASS    " << __FUNCTION__ << std::endl;
    } else {
        std::cout << "FAILED  " << __FUNCTION__ << std::endl;
    }
}

void initialize_list_constructor()
{
    std::balanced_tree<int> i_list_tree = {1, 2, 3, 4};
    std::vector<int> i_list_vec = {1, 2, 3, 4};

    if (i_list_tree.size() == i_list_vec.size()) {
        if (std::equal(i_list_tree.begin(), i_list_tree.end(), i_list_vec.begin())) {
            std::cout << "PASS    " << __FUNCTION__ << std::endl;
        } else {
            std::cout << "FAILED  " << __FUNCTION__ << std::endl;
        }
    } else {
            std::cout << "FAILED  " << __FUNCTION__ <<std::endl;
    }
}

void copy_constructor()
{
    std::balanced_tree<int> i_list_tree = {4, 3, 2, 1};
    std::balanced_tree<int> copy_tree(i_list_tree);
    std::vector<int> i_list_vec = {1, 2, 3, 4};

   assert(i_list_tree.size() == copy_tree.size());

   if (std::equal(copy_tree.begin(), copy_tree.end(), i_list_vec.begin())) {
       std::cout << "PASS    " << __FUNCTION__ << std::endl;
   } else {
       std::cout << "FAILED  " << __FUNCTION__ << std::endl;
   }
}

void copy_assignment()
{
    std::balanced_tree<int> initial;
    test::initailize(initial);

    assert(initial.size() == test::SIZE);
    std::balanced_tree<int> copy;

    copy = initial;
    assert(initial.size() == test::SIZE);

    if(std::equal(copy.begin(), copy.end(), initial.begin())) {
       std::cout << "PASS    " << __FUNCTION__ << std::endl;
    } else {
       std::cout << "FAILED " << __FUNCTION__ << std::endl;
    }
}

void move_constructor()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    std::balanced_tree<int> moved(std::move(tree));
    assert(tree.empty());

    if (moved.size() == test::SIZE) {
       std::cout << "PASS    " << __FUNCTION__ << std::endl;
    } else {
       std::cout << "FAILED " << __FUNCTION__ << std::endl;
    }
}

void move_assignement()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    std::balanced_tree<int> moved;
    moved = std::move(tree);
    assert(tree.empty());

    if (moved.size() == test::SIZE) {
       std::cout << "PASS    " << __FUNCTION__ << std::endl;
    } else {
       std::cout << "FAILED " << __FUNCTION__ << std::endl;
    }
}

void clear()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

  /*  tree.clear();
    if (tree.size() == 0 && tree.empty()) {
       std::cout << "PASS    " << __FUNCTION__ << std::endl;
    } else {
       std::cout << "FAILED " << __FUNCTION__ << std::endl;
    }*/
}

void begin_and_rend()
{
    std::balanced_tree<int> tree;
    test::initailize(tree);

    assert(tree.size() == test::SIZE);
    if (tree.rend() == tree.begin()) {
        std::cout << "PASS    " << __FUNCTION__ << std::endl;
    }

}

void testing_with_couts()
{
    std::balanced_tree<int> empty;
    std::balanced_tree<int> filled = {1, 2, 3, 4};

    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    std::cout << "Printing the filled one with size " << filled.size() << "\n";
    for (const auto& val : filled) {
        std::cout << val << std::endl;
    }

    empty.insert(345);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.insert({10, 20, 948, 1234, 4, 10, 23});
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.erase(10);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.erase(21);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    std::balanced_tree<int> filled2(filled);
    std::cout << "Printing the filled2 one with size " << filled2.size() << "\n";
    for (const auto& val : filled2) {
        std::cout << val << std::endl;
    }

    std::balanced_tree<int> empty2(std::move(empty));
    std::cout << "Printing the empty2 one with size " << empty2.size() << "\n";
    for (const auto& val : empty2) {
        std::cout << val << std::endl;
    }
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }
}
