#pragma once

#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>

namespace std {

template <typename T,
         typename Compare = std::less<T>,
         typename Allocator = std::allocator<T> >
class balanced_tree
{
public:
    using value_type = typename T;
    using size_type = size_t;

public:
    balanced_tree();
    balanced_tree(std::initializer_list<T> il);
    balanced_tree(const balanced_tree&);
    balanced_tree& operator= (const balanced_tree&);
    balanced_tree(balanced_tree&&);
    balanced_tree& operator= (balanced_tree&&);
    ~balanced_tree();

    // @{public interfaces
public:
    /*
     * @brief insert
     */
    std::pair<iterator, bool> insert(const value_type& value);

    /*
     * @brief insert
     */
    std::pair<iterator, bool> insert(value_type&& value);

    /*
     * @brief insert
     */
    void insert(std::initializer_list<T> il);

public:
    /*
     * @brief removes all data from tree
     */
    void clear();

    /*
     * @brief erase element from tree by position
     */
    iterator erase(iterator position);

    /*
     * @brief erase element from tree by position
     */
    iterator erase(const_iterator position);

    /*
     * @brief erase element from tree by value
     */
    void erase(const value_type& value);

public:
    /*
     * @brief returns true  if tree is empty false another case
     */
    bool empty() const noexcept;

    /*
     * @brief returns the size of tree
     */
    size_type size() const noexcept;

public:
    /*
     * @brief fine elementy by value
     */
    iterator find(const value_type& value) noexcept;

    /*
     * @brief fine elementy by value
     */
    const_iterator find(const value_type& value) const noexcept;

private:
    struct bt_node
    {
        T m_data;
        bt_node* m_left_child;
        bt_node* m_right_child;
        bt_node* m_parent;
        size_type m_height;
    };

    bt_node* m_head;
    size_type m_size;
};

} // namespace std
