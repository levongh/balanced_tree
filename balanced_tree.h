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
private:
    using value_type = typename T;
    using size_type = size_t;

    // @{public interfaces
public:
    balanced_tree();
    balanced_tree(std::initializer_list<value_type> il);
    balanced_tree(const balanced_tree& that);
    balanced_tree& operator= (const balanced_tree& that);
    balanced_tree(balanced_tree&& that);
    balanced_tree& operator= (balanced_tree&& that);
    ~balanced_tree();

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
    void insert(std::initializer_list<value_type> il);

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

public:
    class iterator
    {
    public:
        typedef difference_type std::ptrdiff_t;
        typedef value_type balanced_tree::value_type;
        typedef pointer value_type*;
        typedef reference value_type&;
        typedef std::bidirectional_iterator_tag;

    public:
        iterator();
        iterator(const iterator& that);
        iterator(iterator&& that);
        iterator& operator= (const iterator& that);
        iterator& operator= (iterator&& that);
        ~iterator();

        reference operator* () const;
        iterator& operator++ ();

        bool operator== (const iterator& that);
        bool operator!= (const iterator& that);

        reference operator-> ();
        iterator operator++ (int) const;

        iterator& operator-- ();
        iterator operator-- (int) const;

    private:
        bt_node* m_data;
    };

    // @}

private:
    struct bt_node
    {
        value_type m_value;
        bt_node* m_left_child;
        bt_node* m_right_child;
        bt_node* m_parent;
        size_type m_height;
    };

    bt_node* m_head;
    size_type m_size;

private:
    static Compare s_less_than;
    static Allocator s_allocator;
};

} // namespace std
