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
     * @brief find elementy by value
     */
    iterator find(const value_type& value) noexcept;

    /*
     * @brief find elementy by value
     */
    const_iterator find(const value_type& value) const noexcept;

public:
    /*
     * @brief get a begin iterator on container
     */
    iterator begin();

    /*
     * @brief get a end iterator on container
     */
    iterator end();

public:
    /*
     * @brief get a const begin iterator on container
     */
    const_iterator cbegin() const;

    /*
     * @brief get a const end iterator on container
     */
    const_iterator cend() const;

public:
    /*
     * @brief get a reverse begin iterator on container
     */
    reverse_iterator rbegin();

    /*
     * @brief get a end iterator on container
     */
    reverse_iterator rend();

public:
    /*
     * @brief get a const begin iterator on container
     */
    const_reverse_iterator crbegin() const;

    /*
     * @brief get a const end iterator on container
     */
    const_const_iterator crend() const;


public:
    typedef iterator iterator_helper<value_type*, value_type, false>;
    typedef const_iterator iterator_helper<value_type const *, const value_type, false>;
    typedef reverse_iterator iterator_helper<value_type*, value_type, true>;
    typedef const_reverse_iterator iterator_helper<value_type const *, const value_type, true>;

    // @}
private:
    template <typename PointerType, typename ReferenceType, bool is_reverse>
    class iterator_helper
    {
    public:
        typedef difference_type std::ptrdiff_t;
        typedef value_type balanced_tree::value_type;
        typedef pointer PointerType;
        typedef reference ReferenceType&;
        typedef std::bidirectional_iterator_tag;

    public:
        iterator_helper();
        iterator_helper(const iterator_helper& that);
        iterator_helper(iterator_helper&& that);
        iterator_helper& operator= (const iterator_helper& that);
        iterator_helper& operator= (iterator_helper&& that);
        ~iterator_helper();

        reference operator* () const;
        iterator_helper& operator++ ();

        bool operator== (const iterator_helper& that);
        bool operator!= (const iterator_helper& that);

        reference operator-> ();
        iterator_helper operator++ (int) const;

        iterator_helper& operator-- ();
        iterator_helper operator-- (int) const;

    private:
        bt_node* m_data;
    };

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

namespace {

template <typename NodeT, bool reverse>
struct IncrementHelper;

template <typename NodeT>
struct IncrementHelper<NodeT, true>
{
    static NodeT* advance(const NodeT* node,
            NodeT* (*incrementer)(const NodeT*),
            NodeT* (*decrementer)(const NodeT*))
    {
        return decrementer(node);
    }
};

template <typename NodeT>
struct IncrementHelper<NodeT, false>
{
    static NodeT* advance(const NodeT* node,
            NodeT* (*incrementer)(const NodeT*),
            NodeT* (*decrementer)(const NodeT*))
    {
        return incrementer(node);
    }
};

} // unnamed namespace

} // namespace std
