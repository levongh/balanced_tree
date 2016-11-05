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
    typedef iterator iterator_helper<value_type*, value_type, bt_node*>;
    typedef const_iterator iterator_helper<value_type const *, const value_type, bt_node const *>;

    typedef reverse_iterator reverse_iterator_helper<value_type*, value_type, bt_node*>;
    typedef const_reverse_iterator reverse_iterator_helper<value_type const *, const value_type, bt_node const *>;

    // @}
private:
    template <typename PointerType, typename ReferenceType, typename DataType>
    class iterator_helper
    {
    public:
        typedef difference_type std::ptrdiff_t;
        typedef value_type balanced_tree::value_type;
        typedef pointer PointerType;
        typedef reference ReferenceType&;
        typedef std::bidirectional_iterator_tag;

    public:
        iterator_helper()
            : m_data(nullptr)
        {}

        iterator_helper(const iterator_helper& that)
            : m_data(that.m_data)
        {}

        iterator_helper(iterator_helper&& that)
            : m_data(that.m_data)
        {
            that.m_data = nullptr;
        }

        template <typename IterT>
        iterator_helper(const IterT& that)
            : m_data(that.m_data)
        {}

        iterator_helper& operator= (const iterator_helper& that)
        {
            if (&that != this) {
                m_data = that.m_data;
            }
            return *this;
        }

        iterator_helper& operator= (iterator_helper&& that)
        {
            if (&that != this) {
                m_data = that.m_data;
                that.m_data = nullptr;
            }
            return *this;
        }

        template <typename IterT>
        iterator_helper& operator= (const IterT& that)
        {
            if (&that != this) {
                m_data = that.m_data;
            }
            return *this;
        }

        ~iterator_helper() = default;

        reference operator* () const
        {
            return *m_data;
        }

        iterator_helper& operator++ ()
        {
            m_data = balanced_tree::successor(m_data);
            return *this;
        }

        bool operator== (const iterator_helper& that)
        {
            return m_data = that.m_data;
        }

        bool operator!= (const iterator_helper& that)
        {
            return m_data != that.m_data;
        }

        reference operator-> ()
        {
            return *m_data;
        }

        const iterator_helper operator++ (int) const
        {
            iterator_helper tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator_helper& operator-- ()
        {
            m_data = balanced_tree::predecessor(m_data);
            return *this;
        }

        iterator_helper operator-- (int) const;
        {
            iterator_helper tmp = *this;
            --(*this);
            return tmp;
        }

    private:
        DataType m_data;
    };

    template <typename PointerType, typename ReferenceType, typename DataType>
    class reverse_iterator_helper
    {
    public:
        typedef difference_type std::ptrdiff_t;
        typedef value_type balanced_tree::value_type;
        typedef pointer PointerType;
        typedef reference ReferenceType&;
        typedef std::bidirectional_iterator_tag;

    public:
        reverse_iterator_helper()
            : m_data(nullptr)
        {}

        reverse_iterator_helper(const reverse_iterator_helper& that)
            : m_data(that.m_data)
        {}

        reverse_iterator_helper(reverse_iterator_helper&& that)
            : m_data(that.m_data)
        {
            that.m_data = nullptr;
        }

        template <typename IterT>
        reverse_iterator_helper(const IterT& that)
            : m_data(that.m_data)
        {}

        reverse_iterator_helper& operator= (const reverse_iterator_helper& that)
        {
            if (&that != this) {
                m_data = that.m_data;
            }
            return *this;
        }

        reverse_iterator_helper& operator= (reverse_iterator_helper&& that)
        {
            if (&that != this) {
                m_data = that.m_data;
                that.m_data = nullptr;
            }
            return *this;
        }

        template <typename IterT>
        reverse_iterator_helper& operator= (const IterT& that)
        {
            if (&that != this) {
                m_data = that.m_data;
            }
            return *this;
        }

        ~reverse_iterator_helper() = default;

        reference operator* () const
        {
            return *m_data;
        }

        reverse_iterator_helper& operator++ ()
        {
            m_data = balanced_tree::predecessor(m_data);
            return *this;
        }

        bool operator== (const reverse_iterator_helper& that)
        {
            return m_data = that.m_data;
        }

        bool operator!= (const reverse_iterator_helper& that)
        {
            return m_data != that.m_data;
        }

        reference operator-> ()
        {
            return *m_data;
        }

        const reverse_iterator_helper operator++ (int) const
        {
            reverse_iterator_helper tmp = *this;
            ++(*this);
            return tmp;
        }

        reverse_iterator_helper& operator-- ()
        {
            m_data = balanced_tree::successor(m_data);
            return *this;
        }

        reverse_iterator_helper operator-- (int) const;
        {
            reverse_iterator_helper tmp = *this;
            --(*this);
            return tmp;
        }

    private:
        DataType m_data;
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

} // namespace std
