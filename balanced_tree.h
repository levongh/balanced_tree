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
    using value_type = T;
    using size_type = size_t;

private:
    struct bt_node
    {
        value_type* m_value;
        bt_node* m_left_child;
        bt_node* m_right_child;
        bt_node* m_parent;
        size_type m_height;

        template <typename ... Args>
        explicit bt_node(Args&& ... args)
            : m_value(s_allocator.allocate(1))
            , m_left_child(nullptr)
            , m_right_child(nullptr)
            , m_parent(nullptr)
            , m_height(0)
        {
            s_allocator.construct(m_value, std::forward<Args>(args)...);
        }

        ~bt_node()
        {
            s_allocator.destroy(m_value);
            s_allocator.deallocate(m_value, 1);
        }
    };

private:
    template <typename PointerType, typename ReferenceType, typename DataType>
    class iterator_helper
    {
        friend balanced_tree;
    public:
        typedef std::ptrdiff_t difference_type;
        typedef balanced_tree::value_type value_type;
        typedef PointerType pointer;
        typedef ReferenceType& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

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

    private:
        explicit iterator_helper(DataType data)
            : m_data(data)
        {}

    public:

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
            return *m_data->m_value;
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
            return *m_data->m_value;
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

        iterator_helper operator-- (int) const
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
        friend balanced_tree;
    public:
        typedef std::ptrdiff_t difference_type;
        typedef balanced_tree::value_type value_type;
        typedef PointerType pointer;
        typedef ReferenceType& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

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

    private:
        explicit reverse_iterator_helper(DataType data)
            : m_data(data)
        {}

    public:

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
            return *m_data->m_value;
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
            return *m_data->m_value;
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

        reverse_iterator_helper operator-- (int) const
        {
            reverse_iterator_helper tmp = *this;
            --(*this);
            return tmp;
        }

    private:
        DataType m_data;
    };

public:
    typedef iterator_helper<value_type*, value_type, bt_node*> iterator;
    typedef iterator_helper<value_type const *, const value_type, bt_node const *> const_iterator;

    typedef reverse_iterator_helper<value_type*, value_type, bt_node*> reverse_iterator;
    typedef reverse_iterator_helper<value_type const *, const value_type, bt_node const *> const_reverse_iterator;

    // @{public interfaces
public:
    balanced_tree()
        : m_head(nullptr)
        , m_size(0)
    {
    }

    balanced_tree(std::initializer_list<value_type> il)
        : m_head(nullptr)
        , m_size(0)
    {
        insert(il);
    }

    balanced_tree(const balanced_tree& that)
        : m_head(nullptr)
        , m_size(that.m_size)
    {
        balanced_tree::copy(that.m_head, m_head, nullptr);
    }

    balanced_tree& operator= (const balanced_tree& that)
    {
        if (&that != this) {
            balanced_tree::copy(that.m_head, m_head, nullptr);
            m_size = that.m_size;
        }
        return *this;
    }

    balanced_tree(balanced_tree&& that)
        : m_head(that.m_head)
        , m_size(that.m_size)
    {
        that.m_head = nullptr;
        that.m_size = 0;
    }

    balanced_tree& operator= (balanced_tree&& that)
    {
        if (&that != this) {
            m_head = that.m_head;
            m_size = that.m_size;
            that.m_head = nullptr;
            that.m_size = 0;
        }
        return *this;
    }

    ~balanced_tree()
    {
        clear();
    }

public:
    /*
     * @brief insert
     */
    std::pair<iterator, bool> insert(const value_type& value)
    {
        auto result = balanced_tree::insert(value, this, nullptr, m_head);
        if (result.second) {
            ++m_size;
        }
        return result;
    }

    /*
     * @brief insert
     */
    std::pair<iterator, bool> insert(value_type&& value)
    {
        value_type tmp(value);
        return insert(tmp);
    }

    /*
     * @brief insert
     */
    void insert(std::initializer_list<value_type> il)
    {
        for (const auto& value : il) {
            insert(value);
        }
    }

public:
    /*
     * @brief removes all data from tree
     */
    void clear()
    {
        balanced_tree::destroy(m_head);
        m_size = 0;
        m_head = nullptr;
    }

    /*
     * @brief erase element from tree by position
     */
    iterator erase(const iterator position)
    {
        iterator new_position(position);
        ++new_position;
        balanced_tree::destroy_one(position.m_data);
        --m_size;
        return new_position;
    }

    /*
     * @brief erase element from tree by position
     */
    reverse_iterator erase(const reverse_iterator position)
    {
        reverse_iterator new_position(position);
        ++new_position;
        balanced_tree::destroy_one(position.m_data);
        --m_size;
        return new_position;
    }

    /*
     * @brief erase element from tree by value
     */
    size_type erase(const value_type& value)
    {
        auto iter = find(value);
        if (iter != end()) {
            erase(iter);
            return 1;
        }
        return 0;
    }

public:
    /*
     * @brief returns true  if tree is empty false another case
     */
    bool empty() const noexcept
    {
        return m_head == nullptr;
    }

    /*
     * @brief returns the size of tree
     */
    size_type size() const noexcept
    {
        return m_size;
    }

public:
    /*
     * @brief find elementy by value
     */
    iterator find(const value_type& value) noexcept
    {
        return iterator{balanced_tree::find(m_head, value)};
    }

    /*
     * @brief find elementy by value
     */
    const_iterator find(const value_type& value) const noexcept
    {
        return const_iterator{balanced_tree::find(m_head, value)};
    }

public:
    /*
     * @brief get a begin iterator on container
     */
    iterator begin()
    {
        return iterator{balanced_tree::min(m_head)};
    }

    const_iterator begin() const noexcept
    {
        return const_iterator{balanced_tree::min(m_head)};
    }

    /*
     * @brief get a end iterator on container
     */
    iterator end()
    {
        return iterator();
    }

    const_iterator end() const noexcept
    {
        return const_iterator();
    }

public:
    /*
     * @brief get a const begin iterator on container
     */
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /*
     * @brief get a const end iterator on container
     */
    const_iterator cend() const noexcept
    {
        return end();
    }

public:
    /*
     * @brief get a reverse begin iterator on container
     */
    reverse_iterator rbegin()
    {
        return reverse_iterator(balanced_tree::max(m_head));
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(balanced_tree::max(m_head));
    }

    /*
     * @brief get a end iterator on container
     */
    reverse_iterator rend()
    {
        return reverse_iterator();
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator();
    }
    // @}

private:
    static bt_node* predecessor(const bt_node* node);
    static bt_node* successor(const bt_node* node);
    static bt_node* max(bt_node* node);
    static bt_node* min(bt_node* node);
    static void destroy(bt_node* node);
    static void destroy_one(bt_node* node);
    static const bt_node* find(const bt_node* node, const value_type& value);
    static bt_node* find(bt_node* node, const value_type& value);
    static void left_rotate(balanced_tree* tree, bt_node* x);
    static void right_rotate(balanced_tree* tree, bt_node* y);
    static std::pair<iterator, bool> insert(const value_type& value, balanced_tree* tree, bt_node* parent, bt_node* node);
    static int height(const bt_node* node);
    static int direction(const bt_node* node);
    static void refresh_heights(bt_node* node);
    static void swap(bt_node* src, bt_node* dest);
    static void copy(const bt_node* src, bt_node*& dest, bt_node* parent);

    bt_node* m_head;
    size_type m_size;

private:
    static Compare s_less_than;
    static Allocator s_allocator;
};

template <typename T, typename Compare, typename Allocator>
Compare balanced_tree<T, Compare, Allocator>::s_less_than;

template <typename T, typename Compare, typename Allocator>
Allocator balanced_tree<T, Compare, Allocator>::s_allocator;

template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node* balanced_tree<T, Compare, Allocator>::predecessor(const bt_node* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (node->m_left_child != nullptr) {
        return balanced_tree::max(node->m_left_child);
    }
    auto parent = node->m_parent;
    auto current = node;
    while (parent != nullptr && current == parent->m_left_child) {
        current = parent;
        parent = parent->m_parent;
    }
    return parent;
}


template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node* balanced_tree<T, Compare, Allocator>::successor(const bt_node* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (node->m_right_child != nullptr) {
        return balanced_tree::min(node->m_right_child);
    }
    auto parent = node->m_parent;
    auto current = node;
    while (parent != nullptr && current == parent->m_right_child) {
        current = parent;
        parent = parent->m_parent;
    }
    return parent;
}

template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node* balanced_tree<T, Compare, Allocator>::max(bt_node* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    auto tmp = node;
    while (tmp->m_right_child != nullptr) {
        tmp = tmp->m_right_child;
    }
    return tmp;
}

template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node* balanced_tree<T, Compare, Allocator>::min(bt_node* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    auto tmp = node;
    while (tmp->m_left_child != nullptr) {
        tmp = tmp->m_left_child;
    }
    return tmp;
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::destroy(bt_node* node)
{
    if (node == nullptr) {
        return;
    }
    destroy(node->m_left_child);
    node->m_left_child = nullptr;
    destroy(node->m_right_child);
    node->m_right_child = nullptr;
    delete node;
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::destroy_one(bt_node* node)
{
    const auto dir = balanced_tree::direction(node);
    if (dir == 0) {
        if (node->m_parent->m_left_child == node) {
            node->m_parent->m_left_child = nullptr;
        } else {
            node->m_parent->m_right_child = nullptr;
        }
        delete node;
    } else {
        decltype(node) leaf_node = nullptr;
        decltype(node) sub_tree_root = nullptr;
        if (dir > 0) {
            leaf_node = balanced_tree::min(node->m_right_child);
            sub_tree_root = node->m_right_child;
        } else {
            leaf_node = balanced_tree::max(node->m_left_child);
            sub_tree_root = node->m_left_child;
        }
        balanced_tree::swap(node, leaf_node);
        destroy_one(leaf_node);
        balanced_tree::refresh_heights(sub_tree_root);
    }
}

template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node const* balanced_tree<T, Compare, Allocator>::find(const bt_node* node, const value_type& value)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (!s_less_than(*node->m_value, value) &&
            !s_less_than(value, *node->m_value)) {
        return node;
    } else if (s_less_than(*node->m_value, value)) {
        return find(node->m_right_child, value);
    } else {
        return find(node->m_left_child, value);
    }
    return nullptr;
}

template <typename T, typename Compare, typename Allocator>
typename balanced_tree<T, Compare, Allocator>::bt_node* balanced_tree<T, Compare, Allocator>::find(bt_node* node, const value_type& value)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (!s_less_than(*node->m_value, value) &&
            !s_less_than(value, *node->m_value)) {
        return node;
    } else if (s_less_than(*node->m_value, value)) {
        return find(node->m_right_child, value);
    } else {
        return find(node->m_left_child, value);
    }
    return nullptr;
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::left_rotate(balanced_tree* tree, bt_node* x)
{
    auto y = x->m_right_child;
    if (y == nullptr) {
        return;
    }
    x->m_right_child = y->m_left_child;
    if (y->m_left_child != nullptr) {
        y->m_left_child->m_parent = x;
    }
    y->m_parent = x->m_parent;
    if (x->m_parent == nullptr) {
        tree->m_head = y;
    } else {
        if (x->m_parent->m_left_child == x) {
            x->m_parent->m_left_child = y;
        } else {
            x->m_parent->m_right_child = y;
        }
    }
    y->m_left_child = x;
    x->m_parent = y;
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::right_rotate(balanced_tree* tree, bt_node* y)
{
    auto x = y->m_left_child;
    if (x == nullptr) {
        return;
    }
    y->m_left_child = x->m_right_child;
    if (x->m_right_child != nullptr) {
        x->m_right_child->m_parent = y;
    }
    x->m_parent = y->m_parent;
    if (y->m_parent == nullptr) {
        tree->m_head = x;
    } else {
        if (y->m_parent->m_left_child == y) {
            y->m_parent->m_left_child = x;
        } else {
            y->m_parent->m_right_child = x;
        }
    }
    x->m_right_child = y;
    y->m_parent = x;
}

template <typename T, typename Compare, typename Allocator>
std::pair<typename balanced_tree<T, Compare, Allocator>::iterator, bool> balanced_tree<T, Compare, Allocator>::insert(const value_type& value, balanced_tree* tree, bt_node* parent, bt_node* node)
{
    std::pair<balanced_tree::iterator, bool> result;
    if (node == nullptr) {
        auto new_node = new bt_node(value);
        if (parent != nullptr) {
            new_node->m_parent = parent;
            if (s_less_than(value, *parent->m_value)) {
                parent->m_left_child = new_node;
            } else {
                parent->m_right_child = new_node;
            }
        } else {
            tree->m_head = new_node;
        }
        return std::make_pair(iterator{new_node}, true);
    }

    if (!s_less_than(value, *node->m_value) &&
        !s_less_than(*node->m_value, value)) {
        return std::make_pair(iterator{node}, false);
    } else if (s_less_than(value, *node->m_value)) {
        result = insert(value, tree, node, node->m_left_child);
    } else {
        result = insert(value, tree, node, node->m_right_child);
    }

    if (!result.second) {
        return result;
    }

    auto node_parent = node->m_parent;
    if (node_parent != nullptr) {
        auto dir = balanced_tree::direction(node_parent);
        if (dir > 0 && node_parent->m_right_child == node) {
            if (balanced_tree::direction(node) < 0) {
                balanced_tree::right_rotate(tree, node);
                node = node_parent->m_right_child;
            }
            balanced_tree::left_rotate(tree, node_parent);
        }
        if (dir < 0 && node_parent->m_left_child == node) {
            if (balanced_tree::direction(node) > 0) {
                balanced_tree::left_rotate(tree, node);
                node = node_parent->m_left_child;
            }
            balanced_tree::right_rotate(tree, node_parent);
        }
    }

#define max(a, b) (((a) > (b)) ? (a) : (b))
    node->m_height = 1 + max(balanced_tree::height(node->m_left_child),
                             balanced_tree::height(node->m_right_child));
    if (node_parent != nullptr) {
        node_parent->m_height = 1 + max(balanced_tree::height(node_parent->m_left_child),
                                        balanced_tree::height(node_parent->m_right_child));
    }
#undef max

    return result;
}

template <typename T, typename Compare, typename Allocator>
int balanced_tree<T, Compare, Allocator>::height(const bt_node* node)
{
    if (node == nullptr) {
        return -1;
    } else {
        return node->m_height;
    }
}

template <typename T, typename Compare, typename Allocator>
int balanced_tree<T, Compare, Allocator>::direction(const bt_node* node)
{
    if (node == nullptr) {
        return 0;
    } else {
        return balanced_tree::height(node->m_right_child)
             - balanced_tree::height(node->m_left_child);
    }
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::refresh_heights(bt_node* node)
{
    if (node == nullptr) {
        return;
    }
    refresh_heights(node->m_left_child);
    refresh_heights(node->m_right_child);
#define max(a, b) (((a) > (b)) ? (a) : (b))
    node->m_height = 1 + max(balanced_tree::height(node->m_left_child),
                             balanced_tree::height(node->m_right_child));
#undef max
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::swap(bt_node* src, bt_node* dest)
{
    const auto temp = *src->m_value;
    *src->m_value = *dest->m_value;
    *dest->m_value = temp;
}

template <typename T, typename Compare, typename Allocator>
void balanced_tree<T, Compare, Allocator>::copy(const bt_node* src, bt_node*& dest, bt_node* parent)
{
    if (src == nullptr) {
        return;
    }
    dest = new bt_node(*src->m_value);
    dest->m_parent = parent;
    copy(src->m_left_child, dest->m_left_child, dest);
    copy(src->m_right_child, dest->m_right_child, dest);
}

} // namespace std
