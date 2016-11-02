#pragma once

#include <functional>
#include <initializer_list>
#include <memory>

namespace std {

template <typename T,
         typename Compare = std::less<T>,
         typename Allocator = std::allocator<T> >
class balanced_tree
{
public:
    balanced_tree();
    balanced_tree(std::initializer_list<T> il);
    balanced_tree(const balanced_tree&);
    balanced_tree& operator= (const balanced_tree&);
    balanced_tree(balanced_tree&&);
    balanced_tree& operator= (balanced_tree&&);
    ~balanced_tree();

private:
    struct bt_node
    {
        T m_data;
        bt_node* m_left_child;
        bt_node* m_right_child;
        bt_node* m_parent;
    };

    bt_node* m_head;
};

} // namespace std
