// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_CONTAINER_TYPES_HPP
#define JSONCONS_JSON_CONTAINER_TYPES_HPP

#include <string>
#include <vector>
#include <deque>
#include <exception>
#include <cstring>
#include <algorithm> // std::sort, std::stable_sort, std::lower_bound, std::unique
#include <utility>
#include <initializer_list>
#include <iterator> // std::iterator_traits
#include <memory> // std::allocator
#include <utility> // std::move
#include <type_traits> // std::enable_if
#include <jsoncons/json_exception.hpp>
#include <jsoncons/allocator_holder.hpp>

namespace jsoncons {

    template <class ValueType, class DifferenceType, class Pointer, class Reference>
    struct sequence_iterator_types 
    {
        using value_type      = ValueType;
        using difference_type = DifferenceType;
        using pointer         = Pointer;
        using reference       = Reference;
    };

    // iterator for mutable sequence
    template <class ContainerTypes>
    class sequence_iterator 
    { 
        template <class Cont>
        friend class const_sequence_iterator;

        using Ptr = typename ContainerTypes::pointer;
        Ptr ptr_; 
    public:
        using iterator_category = std::random_access_iterator_tag;

        using value_type = typename ContainerTypes::value_type;
        using difference_type = typename ContainerTypes::difference_type;
        using pointer = typename ContainerTypes::pointer;
        using reference = typename ContainerTypes::reference;
    public:
        sequence_iterator() : ptr_() 
        { 
        }

        sequence_iterator(Ptr ptr) : ptr_(ptr) 
        {
        }

        sequence_iterator(const sequence_iterator&) = default;
        sequence_iterator(sequence_iterator&&) = default;
        sequence_iterator& operator=(const sequence_iterator&) = default;
        sequence_iterator& operator=(sequence_iterator&&) = default;

        reference operator*() const 
        {
            return *ptr_;
        }

        pointer operator->() const 
        {
            return ptr_;
        }

        sequence_iterator& operator++() 
        {
            ++ptr_;
            return *this;
        }

        sequence_iterator operator++(int) 
        {
            sequence_iterator temp = *this;
            ++*this;
            return temp;
        }

        sequence_iterator& operator--() 
        {
            --ptr_;
            return *this;
        }

        sequence_iterator operator--(int) 
        {
            sequence_iterator temp = *this;
            --*this;
            return temp;
        }

        sequence_iterator& operator+=(const difference_type offset) 
        {
            ptr_ += offset;
            return *this;
        }

        sequence_iterator operator+(const difference_type offset) const 
        {
            sequence_iterator temp = *this;
            return temp += offset;
        }

        sequence_iterator& operator-=(const difference_type offset) 
        {
            return *this += -offset;
        }

        sequence_iterator operator-(const difference_type offset) const 
        {
            sequence_iterator temp = *this;
            return temp -= offset;
        }

        difference_type operator-(const sequence_iterator& rhs) const 
        {
            return ptr_ - rhs.ptr_;
        }

        reference operator[](const difference_type offset) const 
        {
            return *(*this + offset);
        }

        bool operator==(const sequence_iterator& rhs) const 
        {
            return ptr_ == rhs.ptr_;
        }

        bool operator!=(const sequence_iterator& rhs) const 
        {
            return !(*this == rhs);
        }

        bool operator<(const sequence_iterator& rhs) const 
        {
            return ptr_ < rhs.ptr_;
        }

        bool operator>(const sequence_iterator& rhs) const 
        {
            return rhs < *this;
        }

        bool operator<=(const sequence_iterator& rhs) const 
        {
            return !(rhs < *this);
        }

        bool operator>=(const sequence_iterator& rhs) const 
        {
            return !(*this < rhs);
        }

        inline 
        friend sequence_iterator<ContainerTypes> operator+(
            difference_type offset, sequence_iterator<ContainerTypes> next) 
        {
            return next += offset;
        }
    };

    // iterator for immutable sequence
    template <class ContainerTypes>
    class const_sequence_iterator 
    { 
        using Ptr = typename ContainerTypes::pointer;
        Ptr ptr_; 
    public:
        using iterator_category = std::random_access_iterator_tag;

        using value_type = typename ContainerTypes::value_type;
        using difference_type = typename ContainerTypes::difference_type;
        using pointer = typename ContainerTypes::pointer;
        using reference = typename ContainerTypes::reference;
    private:
        using iterator_types = sequence_iterator_types<value_type, 
                                                       difference_type,
                                                       typename std::conditional<std::is_const<typename std::remove_pointer<pointer>::type>::value,typename std::add_pointer<value_type>::type,pointer>::type, 
                                                       typename std::conditional<std::is_const<typename std::remove_pointer<pointer>::type>::value,value_type&,reference>::type>; 

    public:
        const_sequence_iterator() : ptr_() 
        { 
        }

        const_sequence_iterator(Ptr ptr) : ptr_(ptr) 
        {
        }

        const_sequence_iterator(const const_sequence_iterator&) = default;
        const_sequence_iterator(const_sequence_iterator&&) = default;
        const_sequence_iterator& operator=(const const_sequence_iterator&) = default;
        const_sequence_iterator& operator=(const_sequence_iterator&&) = default;

        const_sequence_iterator(const sequence_iterator<iterator_types>& iter)
            : ptr_(iter.ptr_)
        {
        }

        reference operator*() const 
        {
            return *ptr_;
        }

        pointer operator->() const 
        {
            return ptr_;
        }

        const_sequence_iterator& operator++() 
        {
            ++ptr_;
            return *this;
        }

        const_sequence_iterator operator++(int) 
        {
            const_sequence_iterator temp = *this;
            ++*this;
            return temp;
        }

        const_sequence_iterator& operator--() 
        {
            --ptr_;
            return *this;
        }

        const_sequence_iterator operator--(int) 
        {
            const_sequence_iterator temp = *this;
            --*this;
            return temp;
        }

        const_sequence_iterator& operator+=(const difference_type offset) 
        {
            ptr_ += offset;
            return *this;
        }

        const_sequence_iterator operator+(const difference_type offset) const 
        {
            const_sequence_iterator temp = *this;
            return temp += offset;
        }

        const_sequence_iterator& operator-=(const difference_type offset) 
        {
            return *this += -offset;
        }

        const_sequence_iterator operator-(const difference_type offset) const 
        {
            const_sequence_iterator temp = *this;
            return temp -= offset;
        }

        difference_type operator-(const const_sequence_iterator& rhs) const 
        {
            return ptr_ - rhs.ptr_;
        }

        reference operator[](const difference_type offset) const 
        {
            return *(*this + offset);
        }

        bool operator==(const const_sequence_iterator& rhs) const 
        {
            return ptr_ == rhs.ptr_;
        }

        bool operator!=(const const_sequence_iterator& rhs) const 
        {
            return !(*this == rhs);
        }

        bool operator<(const const_sequence_iterator& rhs) const 
        {
            return ptr_ < rhs.ptr_;
        }

        bool operator>(const const_sequence_iterator& rhs) const 
        {
            return rhs < *this;
        }

        bool operator<=(const const_sequence_iterator& rhs) const 
        {
            return !(rhs < *this);
        }

        bool operator>=(const const_sequence_iterator& rhs) const 
        {
            return !(*this < rhs);
        }

        inline 
        friend const_sequence_iterator<ContainerTypes> operator+(
            difference_type offset, const_sequence_iterator<ContainerTypes> next) 
        {
            return next += offset;
        }
    };

    // key_value

    template <class KeyT, class ValueT>
    class key_value
    {
    public:
        typedef KeyT key_type;
        typedef ValueT value_type;
        typedef typename ValueT::allocator_type allocator_type;
        typedef typename value_type::string_view_type string_view_type;
    private:
        key_type key_;
        value_type value_;
    public:

        key_value()
        {
        }

        key_value(const key_type& name, const value_type& val)
            : key_(name), value_(val)
        {
        }

        key_value(const string_view_type& name)
            : key_(name)
        {
        }

        template <class T>
        key_value(key_type&& name, T&& val)
            : key_(std::forward<key_type>(name)), 
              value_(std::forward<T>(val))
        {
        }

        template <typename... Args>
        key_value(key_type&& name,  Args&& ... args)
            : key_(std::forward<key_type>(name)), value_(std::forward<Args>(args)...)
        {
        }

        key_value(const key_value& member)
            : key_(member.key_), value_(member.value_)
        {
        }

        key_value(key_value&& member) noexcept
            : key_(std::move(member.key_)), value_(std::move(member.value_))
        {
        }

        const key_type& key() const
        {
            return key_;
        }

        value_type& value()
        {
            return value_;
        }

        const value_type& value() const
        {
            return value_;
        }

        template <class T>
        void value(T&& value)
        {
            value_ = std::forward<T>(value);
        }

        void swap(key_value& member) noexcept
        {
            key_.swap(member.key_);
            value_.swap(member.value_);
        }

        key_value& operator=(const key_value& member)
        {
            if (this != & member)
            {
                key_ = member.key_;
                value_ = member.value_;
            }
            return *this;
        }

        key_value& operator=(key_value&& member) noexcept
        {
            if (this != &member)
            {
                key_.swap(member.key_);
                value_.swap(member.value_);
            }
            return *this;
        }

        void shrink_to_fit() 
        {
            key_.shrink_to_fit();
            value_.shrink_to_fit();
        }
    #if !defined(JSONCONS_NO_DEPRECATED)
        JSONCONS_DEPRECATED_MSG("Instead, use key()")
        const key_type& name() const
        {
            return key_;
        }
    #endif

        friend bool operator==(const key_value& lhs, const key_value& rhs)
        {
            return lhs.key_ == rhs.key_ && lhs.value_ == rhs.value_;
        }

        friend bool operator!=(const key_value& lhs, const key_value& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const key_value& lhs, const key_value& rhs)
        {
            if (lhs.key_ < rhs.key_)
            {
                return true;
            }
            if (lhs.key_ == rhs.key_ && lhs.value_ < rhs.value_)
            {
                return true;
            }
            return false;
        }

        friend bool operator<=(const key_value& lhs, const key_value& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const key_value& lhs, const key_value& rhs) 
        {
            return !(lhs <= rhs);
        }

        friend bool operator>=(const key_value& lhs, const key_value& rhs)
        {
            return !(lhs < rhs);
        }
    };

    template <class KeyT, class ValueT>
    struct get_key_value
    {
        typedef key_value<KeyT,ValueT> key_value_type;

        template <class T1,class T2>
        key_value_type operator()(const std::pair<T1,T2>& p)
        {
            return key_value_type(p.first,p.second);
        }
        template <class T1,class T2>
        key_value_type operator()(std::pair<T1,T2>&& p)
        {
            return key_value_type(std::forward<T1>(p.first),std::forward<T2>(p.second));
        }
        template <class T1,class T2>
        const key_value_type& operator()(const key_value<T1,T2>& p)
        {
            return p;
        }
        template <class T1,class T2>
        key_value_type operator()(key_value<T1,T2>&& p)
        {
            return std::move(p);
        }
    };

    // json_array

    template <class Json>
    class json_array : public allocator_holder<typename Json::allocator_type>
    {
    public:
        typedef typename Json::allocator_type allocator_type;
        typedef Json value_type;
    private:
        typedef typename Json::implementation_policy implementation_policy;
        typedef typename std::allocator_traits<allocator_type>:: template rebind_alloc<value_type> value_allocator_type;
        using container_type = typename implementation_policy::template sequence_container_type<value_type, value_allocator_type>;
    public:
        using difference_type = typename container_type::difference_type;
        using pointer = typename container_type::pointer;
        using const_pointer = typename container_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
    private:
        using iterator_types = sequence_iterator_types<value_type, 
                                                       difference_type,
                                                       pointer, 
                                                       reference>; 

        using const_iterator_types = sequence_iterator_types<value_type, 
                                                             difference_type,
                                                             const_pointer, 
                                                             const_reference>; 

        container_type elements_;
    public:
        typedef sequence_iterator<iterator_types> iterator;
        typedef const_sequence_iterator<const_iterator_types> const_iterator;

        using allocator_holder<allocator_type>::get_allocator;

        json_array()
        {
        }

        explicit json_array(const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              elements_(value_allocator_type(alloc))
        {
        }

        explicit json_array(std::size_t n, 
                            const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              elements_(n,Json(),value_allocator_type(alloc))
        {
        }

        explicit json_array(std::size_t n, 
                            const Json& value, 
                            const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              elements_(n,value,value_allocator_type(alloc))
        {
        }

        template <class InputIterator>
        json_array(InputIterator begin, InputIterator end, const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              elements_(begin,end,value_allocator_type(alloc))
        {
        }
        json_array(const json_array& val)
            : allocator_holder<allocator_type>(val.get_allocator()),
              elements_(val.elements_)
        {
        }
        json_array(const json_array& val, const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              elements_(val.elements_,value_allocator_type(alloc))
        {
        }

        json_array(json_array&& val) noexcept
            : allocator_holder<allocator_type>(val.get_allocator()), 
              elements_(std::move(val.elements_))
        {
        }
        json_array(json_array&& val, const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              elements_(std::move(val.elements_),value_allocator_type(alloc))
        {
        }

        json_array(const std::initializer_list<Json>& init, 
                   const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              elements_(init,value_allocator_type(alloc))
        {
        }
        ~json_array()
        {
        }

        void swap(json_array<Json>& val) noexcept
        {
            elements_.swap(val.elements_);
        }

        std::size_t size() const {return elements_.size();}

        std::size_t capacity() const {return elements_.capacity();}

        void clear() {elements_.clear();}

        void shrink_to_fit() 
        {
            for (std::size_t i = 0; i < elements_.size(); ++i)
            {
                elements_[i].shrink_to_fit();
            }
            elements_.shrink_to_fit();
        }

        void reserve(std::size_t n) {elements_.reserve(n);}

        void resize(std::size_t n) {elements_.resize(n);}

        void resize(std::size_t n, const Json& val) {elements_.resize(n,val);}

    #if !defined(JSONCONS_NO_DEPRECATED)
        JSONCONS_DEPRECATED_MSG("Instead, use erase(const_iterator, const_iterator)")
        void remove_range(std::size_t from_index, std::size_t to_index) 
        {
            JSONCONS_ASSERT(from_index <= to_index);
            JSONCONS_ASSERT(to_index <= elements_.size());
            elements_.erase(elements_.cbegin()+from_index,elements_.cbegin()+to_index);
        }
    #endif
        void erase(const_iterator pos) 
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = elements_.begin() + (pos2 - elements_.begin());
            elements_.erase(it);
    #else
            elements_.erase(pos2);
    #endif
        }

        void erase(const_iterator first, const_iterator last) 
        {
            auto first2 = to_container_iterator(first)             ;
            auto last2 = to_container_iterator(last)             ;
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it1 = elements_.begin() + (first2 - elements_.begin());
            auto it2 = elements_.begin() + (last2 - elements_.begin());
            elements_.erase(it1,it2);
    #else
            elements_.erase(first2,last2);
    #endif
        }

        Json& operator[](std::size_t i) {return elements_[i];}

        const Json& operator[](std::size_t i) const {return elements_[i];}

        // push_back

        template <class T, class A=allocator_type>
        typename std::enable_if<is_stateless<A>::value,void>::type 
        push_back(T&& value)
        {
            elements_.emplace_back(std::forward<T>(value));
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<!is_stateless<A>::value,void>::type 
        push_back(T&& value)
        {
            elements_.emplace_back(std::forward<T>(value),get_allocator());
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<is_stateless<A>::value,iterator>::type 
        insert(const_iterator pos, T&& value)
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = elements_.begin() + (pos2 - elements_.begin());
            return from_container_iterator(elements_.emplace(it, std::forward<T>(value)));
    #else
            return from_container_iterator(elements_.emplace(pos2, std::forward<T>(value)));
    #endif
        }
        template <class T, class A=allocator_type>
        typename std::enable_if<!is_stateless<A>::value,iterator>::type 
        insert(const_iterator pos, T&& value)
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = elements_.begin() + (pos2 - elements_.begin());
            return from_container_iterator(elements_.emplace(it, std::forward<T>(value), get_allocator()));
    #else
            return from_container_iterator(elements_.emplace(pos2, std::forward<T>(value), get_allocator()));
    #endif
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = elements_.begin() + (pos2 - elements_.begin());
            elements_.insert(it, first, last);
            return from_container_iterator(first == last ? it : it + 1);
    #else
            return from_container_iterator(elements_.insert(pos2, first, last));
    #endif
        }

        template <class A=allocator_type, class... Args>
        typename std::enable_if<is_stateless<A>::value,iterator>::type 
        emplace(const_iterator pos, Args&&... args)
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = elements_.begin() + (pos2 - elements_.begin());
            return from_container_iterator(elements_.emplace(it, std::forward<Args>(args)...));
    #else
            return from_container_iterator(elements_.emplace(pos2, std::forward<Args>(args)...));
    #endif
        }

        template <class... Args>
        Json& emplace_back(Args&&... args)
        {
            elements_.emplace_back(std::forward<Args>(args)...);
            return elements_.back();
        }

        iterator begin() {return iterator(elements_.data());}

        iterator end() {return iterator(elements_.data() + elements_.size());}

        const_iterator begin() const {return const_iterator(elements_.data());}

        const_iterator end() const {return const_iterator(elements_.data()+elements_.size());}

        bool operator==(const json_array<Json>& rhs) const
        {
            return elements_ == rhs.elements_;
        }

        bool operator<(const json_array<Json>& rhs) const
        {
            return elements_ < rhs.elements_;
        }
    private:

        json_array& operator=(const json_array<Json>&) = delete;

        typename container_type::iterator to_container_iterator(iterator it)
        {
            return elements_.begin() + (it - begin());
        }

        typename container_type::const_iterator to_container_iterator(const_iterator it) const
        {
            return elements_.begin() + (it - begin());
        }

        iterator from_container_iterator(typename container_type::iterator it)
        {
            return iterator(elements_.data() + (it - elements_.begin()));
        }

        const_iterator from_container_iterator(typename container_type::const_iterator it) const
        {
            return const_iterator(elements_.data() + (it - elements_.begin()));
        }
    };

    struct sorted_unique_range_tag
    {
        explicit sorted_unique_range_tag() = default; 
    };

    // json_object

    template <class KeyT,class Json,class Enable = void>
    class json_object
    {
    };

    // Sort keys
    template <class KeyT,class Json>
    class json_object<KeyT,Json,typename std::enable_if<!Json::implementation_policy::preserve_order>::type> : 
        public allocator_holder<typename Json::allocator_type>
    {
    public:
        typedef typename Json::allocator_type allocator_type;
        typedef KeyT key_type;
        typedef key_value<KeyT,Json> value_type;
    private:
        typedef typename Json::implementation_policy implementation_policy;
        typedef typename std::allocator_traits<allocator_type>:: template rebind_alloc<value_type> key_value_allocator_type;
        using container_type = typename implementation_policy::template sequence_container_type<value_type,key_value_allocator_type>;
    public:
        typedef typename Json::char_type char_type;
        typedef typename Json::string_view_type string_view_type;
        using difference_type = typename container_type::difference_type;
        using pointer = typename container_type::pointer;
        using const_pointer = typename container_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
    private:
        using iterator_types = sequence_iterator_types<value_type, 
                                                       difference_type,
                                                       pointer, 
                                                       reference>; 

        using const_iterator_types = sequence_iterator_types<value_type, 
                                                             difference_type,
                                                             const_pointer, 
                                                             const_reference>; 

        container_type members_;
    public:
        typedef sequence_iterator<iterator_types> iterator;
        typedef const_sequence_iterator<const_iterator_types> const_iterator;

        using allocator_holder<allocator_type>::get_allocator;

        json_object()
        {
        }

        explicit json_object(const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc))
        {
        }

        json_object(const json_object& val)
            : allocator_holder<allocator_type>(val.get_allocator()),
              members_(val.members_)
        {
        }

        json_object(json_object&& val)
            : allocator_holder<allocator_type>(val.get_allocator()), 
              members_(std::move(val.members_))
        {
        }

        json_object(const json_object& val, const allocator_type& alloc) 
            : allocator_holder<allocator_type>(alloc), 
              members_(val.members_,key_value_allocator_type(alloc))
        {
        }

        json_object(json_object&& val,const allocator_type& alloc) 
            : allocator_holder<allocator_type>(alloc), members_(std::move(val.members_),key_value_allocator_type(alloc))
        {
        }

        template<class InputIt>
        json_object(InputIt first, InputIt last)
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(get_key_value<KeyT,Json>()(*s));
            }
            std::stable_sort(members_.begin(),members_.end(),
                             [](const value_type& a, const value_type& b) -> bool {return a.key().compare(b.key()) < 0;});
            auto it = std::unique(members_.begin(), members_.end(),
                                  [](const value_type& a, const value_type& b) -> bool { return !(a.key().compare(b.key()));});
            members_.erase(it, members_.end());
        }

        template<class InputIt>
        json_object(InputIt first, InputIt last, 
                    const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc))
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(get_key_value<KeyT,Json>()(*s));
            }
            std::stable_sort(members_.begin(),members_.end(),
                             [](const value_type& a, const value_type& b) -> bool {return a.key().compare(b.key()) < 0;});
            auto it = std::unique(members_.begin(), members_.end(),
                                  [](const value_type& a, const value_type& b) -> bool { return !(a.key().compare(b.key()));});
            members_.erase(it, members_.end());
        }

        json_object(const std::initializer_list<std::pair<std::basic_string<char_type>,Json>>& init, 
                    const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc))
        {
            members_.reserve(init.size());
            for (auto& item : init)
            {
                insert_or_assign(item.first, item.second);
            }
        }

        void swap(json_object& val) noexcept
        {
            members_.swap(val.members_);
        }

        iterator begin()
        {
            return iterator(members_.data());
        }

        iterator end()
        {
            return iterator(members_.data() + members_.size());
        }

        const_iterator begin() const
        {
            return const_iterator(members_.data());
        }

        const_iterator end() const
        {
            return const_iterator(members_.data() + members_.size());
        }

        std::size_t size() const {return members_.size();}

        std::size_t capacity() const {return members_.capacity();}

        void clear() {members_.clear();}

        void shrink_to_fit() 
        {
            for (std::size_t i = 0; i < members_.size(); ++i)
            {
                members_[i].shrink_to_fit();
            }
            members_.shrink_to_fit();
        }

        void reserve(std::size_t n) {members_.reserve(n);}

        Json& at(std::size_t i) 
        {
            if (i >= members_.size())
            {
                JSONCONS_THROW(json_runtime_error<std::out_of_range>("Invalid array subscript"));
            }
            return members_[i].value();
        }

        const Json& at(std::size_t i) const 
        {
            if (i >= members_.size())
            {
                JSONCONS_THROW(json_runtime_error<std::out_of_range>("Invalid array subscript"));
            }
            return members_[i].value();
        }

        iterator find(const string_view_type& name) noexcept
        {
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            auto result = (it != members_.end() && it->key() == name) ? it : members_.end();
            return from_container_iterator(result);
        }

        const_iterator find(const string_view_type& name) const noexcept
        {
            auto it = std::lower_bound(members_.begin(),members_.end(), 
                                       name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});
            auto result = (it != members_.end() && it->key() == name) ? it : members_.end();
            return from_container_iterator(result);
        }

        void erase(const_iterator pos) 
        {
            auto pos2 = to_container_iterator(pos);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it = members_.begin() + (pos2 - members_.begin());
            members_.erase(it);
    #else
            members_.erase(pos2);
    #endif
        }

        void erase(const_iterator first, const_iterator last) 
        {
            auto first2 = to_container_iterator(first)             ;
            auto last2 = to_container_iterator(last)             ;
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
            auto it1 = members_.begin() + (first2 - members_.begin());
            auto it2 = members_.begin() + (last2 - members_.begin());
            members_.erase(it1,it2);
    #else
            members_.erase(first2,last2);
    #endif
        }

        void erase(const string_view_type& name) 
        {
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            if (it != members_.end() && it->key() == name)
            {
                members_.erase(it);
            }
        }

        template<class InputIt, class Convert>
        void insert(InputIt first, InputIt last, Convert convert)
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(members_.size() + count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(convert(*s));
            }
            std::stable_sort(members_.begin(),members_.end(),
                             [](const value_type& a, const value_type& b) -> bool {return a.key().compare(b.key()) < 0;});
            auto it = std::unique(members_.begin(), members_.end(),
                                  [](const value_type& a, const value_type& b) -> bool { return !(a.key().compare(b.key()));});
            members_.erase(it, members_.end());
        }

        template<class InputIt, class Convert>
        void insert(sorted_unique_range_tag, InputIt first, InputIt last, Convert convert)
        {
            if (first != last)
            {
                std::size_t count = std::distance(first,last);
                members_.reserve(members_.size() + count);

                auto it = to_container_iterator(find(convert(*first).key()));
                if (it != members_.end())
                {
                    for (auto s = first; s != last; ++s)
                    {
                        it = members_.emplace(it, convert(*s));
                    }
                }
                else
                {
                    for (auto s = first; s != last; ++s)
                    {
                        members_.emplace_back(convert(*s));
                    }
                }
            }
        }

        // insert_or_assign

        template <class T, class A=allocator_type>
        typename std::enable_if<is_stateless<A>::value,std::pair<iterator,bool>>::type
        insert_or_assign(const string_view_type& name, T&& value)
        {
            bool inserted;
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end()), 
                                            std::forward<T>(value));
                inserted = true;
                it = members_.begin() + members_.size() - 1;
            }
            else if (it->key() == name)
            {
                it->value(Json(std::forward<T>(value)));
                inserted = false; // assigned
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end()),
                                            std::forward<T>(value));
                inserted = true;
            }
            return std::make_pair(from_container_iterator(it),inserted);
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<!is_stateless<A>::value,std::pair<iterator,bool>>::type
        insert_or_assign(const string_view_type& name, T&& value)
        {
            bool inserted;
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end(), get_allocator()), 
                                            std::forward<T>(value),get_allocator());
                inserted = true;
                it = members_.begin() + members_.size() - 1;
            }
            else if (it->key() == name)
            {
                it->value(Json(std::forward<T>(value), get_allocator()));
                inserted = false; // assigned
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end(), get_allocator()),
                                            std::forward<T>(value),get_allocator());
                inserted = true;
            }
            return std::make_pair(from_container_iterator(it),inserted);
        }

        // try_emplace

        template <class A=allocator_type, class... Args>
        typename std::enable_if<is_stateless<A>::value,std::pair<iterator,bool>>::type
        try_emplace(const string_view_type& name, Args&&... args)
        {
            bool inserted;
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end()), 
                                            std::forward<Args>(args)...);
                it = members_.begin() + members_.size() - 1;
                inserted = true;
            }
            else if (it->key() == name)
            {
                inserted = false;
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end()),
                                            std::forward<Args>(args)...);
                inserted = true;
            }
            return std::make_pair(from_container_iterator(it),inserted);
        }

        template <class A=allocator_type, class... Args>
        typename std::enable_if<!is_stateless<A>::value,std::pair<iterator,bool>>::type
        try_emplace(const string_view_type& name, Args&&... args)
        {
            bool inserted;
            auto it = std::lower_bound(members_.begin(),members_.end(), name, 
                                       [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end(), get_allocator()), 
                                            std::forward<Args>(args)...);
                it = members_.begin() + members_.size() - 1;
                inserted = true;
            }
            else if (it->key() == name)
            {
                inserted = false;
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end(), get_allocator()),
                                            std::forward<Args>(args)...);
                inserted = true;
            }
            return std::make_pair(from_container_iterator(it),inserted);
        }

        template <class A=allocator_type, class ... Args>
        iterator try_emplace(iterator hint, const string_view_type& name, Args&&... args)
        {
            return from_container_iterator(try_emplace(to_container_iterator(hint),name,std::forward<Args>(args)...));
        }

        // insert_or_assign

        template <class T, class A=allocator_type>
        iterator insert_or_assign(iterator hint, const string_view_type& name, T&& value)
        {
            return from_container_iterator(insert_or_assign(to_container_iterator(hint), name, std::forward<T>(value)));
        }

        // merge

        void merge(const json_object& source)
        {
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                try_emplace(it->key(),it->value());
            }
        }

        void merge(json_object&& source)
        {
            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                auto pos = std::lower_bound(members_.begin(),members_.end(), it->key(), 
                                            [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});   
                if (pos == members_.end() )
                {
                    members_.emplace_back(*it);
                }
                else if (it->key() != pos->key())
                {
                    members_.emplace(pos,*it);
                }
            }
        }

        void merge(iterator hint, const json_object& source)
        {
            auto hint2 = to_container_iterator(hint);
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                hint2 = try_emplace(hint2, it->key(),it->value());
            }
        }

        void merge(iterator hint, json_object&& source)
        {
            auto hint2 = to_container_iterator(hint);

            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                iterator pos;
                if (hint2 != members_.end() && hint2->key() <= it->key())
                {
                    pos = std::lower_bound(hint2,members_.end(), it->key(), 
                                          [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
                }
                else
                {
                    pos = std::lower_bound(members_.begin(),members_.end(), it->key(), 
                                          [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
                }
                if (pos == members_.end() )
                {
                    members_.emplace_back(*it);
                    hint2 = members_.begin() + (members_.size() - 1);
                }
                else if (it->key() != pos->key())
                {
                    hint2 = members_.emplace(pos,*it);
                }
            }
        }

        // merge_or_update

        void merge_or_update(const json_object& source)
        {
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                insert_or_assign(it->key(),it->value());
            }
        }

        void merge_or_update(json_object&& source)
        {
            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                auto pos = std::lower_bound(members_.begin(),members_.end(), it->key(), 
                                            [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});   
                if (pos == members_.end() )
                {
                    members_.emplace_back(*it);
                }
                else 
                {
                    pos->value(it->value());
                }
            }
        }

        void merge_or_update(iterator hint, const json_object& source)
        {
            auto hint2 = to_container_iterator(hint);
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                hint2 = insert_or_assign(hint2, it->key(),it->value());
            }
        }

        void merge_or_update(iterator hint, json_object&& source)
        {
            auto hint2 = to_container_iterator(hint);
            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                iterator pos;
                if (hint2 != members_.end() && hint2->key() <= it->key())
                {
                    pos = std::lower_bound(hint2,members_.end(), it->key(), 
                                          [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
                }
                else
                {
                    pos = std::lower_bound(members_.begin(),members_.end(), it->key(), 
                                          [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
                }
                if (pos == members_.end() )
                {
                    members_.emplace_back(*it);
                    hint2 = members_.begin() + (members_.size() - 1);
                }
                else 
                {
                    pos->value(it->value());
                    hint2 = pos;
                }
            }
        }

        bool operator==(const json_object& rhs) const
        {
            return members_ == rhs.members_;
        }

        bool operator<(const json_object& rhs) const
        {
            return members_ < rhs.members_;
        }
    private:
        json_object& operator=(const json_object&) = delete;

        typename container_type::iterator to_container_iterator(iterator it)
        {
            return members_.begin() + (it - begin());
        }

        typename container_type::const_iterator to_container_iterator(const_iterator it) const
        {
            return members_.begin() + (it - begin());
        }

        iterator from_container_iterator(typename container_type::iterator it)
        {
            return iterator(members_.data() + (it - members_.begin()));
        }

        const_iterator from_container_iterator(typename container_type::const_iterator it) const
        {
            return const_iterator(members_.data() + (it - members_.begin()));
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<is_stateless<A>::value,typename container_type::iterator>::type 
        insert_or_assign(typename container_type::iterator hint, const string_view_type& name, T&& value)
        {
            typename container_type::iterator it;
            if (hint != members_.end() && hint->key() <= name)
            {
                it = std::lower_bound(hint,members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }
            else
            {
                it = std::lower_bound(members_.begin(),members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }

            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end()), 
                                            std::forward<T>(value));
                it = members_.begin() + (members_.size() - 1);
            }
            else if (it->key() == name)
            {
                it->value(Json(std::forward<T>(value)));
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end()),
                                            std::forward<T>(value));
            }
            return it;
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<!is_stateless<A>::value,typename container_type::iterator>::type 
        insert_or_assign(typename container_type::iterator hint, const string_view_type& name, T&& value)
        {
            typename container_type::iterator it;
            if (hint != members_.end() && hint->key() <= name)
            {
                it = std::lower_bound(hint,members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }
            else
            {
                it = std::lower_bound(members_.begin(),members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }

            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end(), get_allocator()), 
                                            std::forward<T>(value),get_allocator());
                it = members_.begin() + (members_.size() - 1);
            }
            else if (it->key() == name)
            {
                it->value(Json(std::forward<T>(value),get_allocator()));
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end(), get_allocator()),
                                            std::forward<T>(value),get_allocator());
            }
            return it;
        }

        template <class A=allocator_type, class ... Args>
        typename std::enable_if<is_stateless<A>::value,typename container_type::iterator>::type 
        try_emplace(typename container_type::iterator hint, const string_view_type& name, Args&&... args)
        {
            typename container_type::iterator it = hint;

            if (hint != members_.end() && hint->key() <= name)
            {
                it = std::lower_bound(hint,members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }
            else
            {
                it = std::lower_bound(members_.begin(),members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }

            if (it == members_.end()) 
            {
                members_.emplace_back(key_type(name.begin(),name.end()), 
                                            std::forward<Args>(args)...);
                it = members_.begin() + (members_.size() - 1);
            }
            else if (it->key() == name)
            {
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end()),
                                            std::forward<Args>(args)...);
            }

            return it;
        }

        template <class A=allocator_type, class ... Args>
        typename std::enable_if<!is_stateless<A>::value,typename container_type::iterator>::type 
        try_emplace(typename container_type::iterator hint, const string_view_type& name, Args&&... args)
        {
            typename container_type::iterator it = hint;
            if (hint != members_.end() && hint->key() <= name)
            {
                it = std::lower_bound(hint,members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }
            else
            {
                it = std::lower_bound(members_.begin(),members_.end(), name, 
                                      [](const value_type& a, const string_view_type& k) -> bool {return string_view_type(a.key()).compare(k) < 0;});        
            }

            if (it == members_.end())
            {
                members_.emplace_back(key_type(name.begin(),name.end(), get_allocator()), 
                                            std::forward<Args>(args)...);
                it = members_.begin() + (members_.size() - 1);
            }
            else if (it->key() == name)
            {
            }
            else
            {
                it = members_.emplace(it,
                                            key_type(name.begin(),name.end(), get_allocator()),
                                            std::forward<Args>(args)...);
            }
            return it;
        }
    };

    // Preserve order
    template <class KeyT,class Json>
    class json_object<KeyT,Json,typename std::enable_if<Json::implementation_policy::preserve_order>::type> :
        public allocator_holder<typename Json::allocator_type>
    {
    public:
        typedef typename Json::allocator_type allocator_type;
        typedef KeyT key_type;
        typedef key_value<KeyT,Json> value_type;
    private:
        typedef typename Json::implementation_policy implementation_policy;
        typedef typename std::allocator_traits<allocator_type>:: template rebind_alloc<value_type> key_value_allocator_type;
        using container_type = typename implementation_policy::template sequence_container_type<value_type,key_value_allocator_type>;
        typedef typename std::allocator_traits<allocator_type>:: template rebind_alloc<std::size_t> index_allocator_type;
        using index_container_type = typename implementation_policy::template sequence_container_type<std::size_t,index_allocator_type>;
    public:
        typedef typename Json::char_type char_type;
        typedef typename Json::string_view_type string_view_type;
        using difference_type = typename container_type::difference_type;
        using pointer = typename container_type::pointer;
        using const_pointer = typename container_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
    private:
        using iterator_types = sequence_iterator_types<value_type, 
                                                       difference_type,
                                                       pointer, 
                                                       reference>; 

        using const_iterator_types = sequence_iterator_types<value_type, 
                                                             difference_type,
                                                             const_pointer, 
                                                             const_reference>; 
    private:
        container_type members_;
        index_container_type index_;
    public:
        typedef sequence_iterator<iterator_types> iterator;
        typedef const_sequence_iterator<const_iterator_types> const_iterator;

        using allocator_holder<allocator_type>::get_allocator;

        json_object()
        {
        }
        json_object(const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc)), 
              index_(index_allocator_type(alloc))
        {
        }

        json_object(const json_object& val)
            : allocator_holder<allocator_type>(val.get_allocator()), 
              members_(val.members_),
              index_(val.index_)
        {
        }

        json_object(json_object&& val)
            : allocator_holder<allocator_type>(val.get_allocator()), 
              members_(std::move(val.members_)),
              index_(std::move(val.index_))
        {
        }

        json_object(const json_object& val, const allocator_type& alloc) 
            : allocator_holder<allocator_type>(alloc), 
              members_(val.members_,key_value_allocator_type(alloc)),
              index_(val.index_,index_allocator_type(alloc))
        {
        }

        json_object(json_object&& val,const allocator_type& alloc) 
            : allocator_holder<allocator_type>(alloc), 
              members_(std::move(val.members_),key_value_allocator_type(alloc)),
              index_(std::move(val.index_),index_allocator_type(alloc))
        {
        }

        template<class InputIt>
        json_object(InputIt first, InputIt last)
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(get_key_value<KeyT,Json>()(*s));
            }

            build_index();
            auto last_unique = std::unique(index_.begin(), index_.end(),
                [&](std::size_t a, std::size_t b) { return !(members_.at(a).key().compare(members_.at(b).key())); });

            if (last_unique != index_.end())
            {
                index_.erase(last_unique, index_.end());
                std::sort(index_.begin(), index_.end());

                auto result = index_.rbegin();
                if (*result != members_.size())
                {
                    members_.erase(members_.begin() + (*result + 1), members_.end());
                }
                for (auto it = index_.rbegin() + 1; it != index_.rend(); ++it, ++result)
                {
                    if (*result - *it > 1)
                    {
                        members_.erase(members_.begin() + (*it + 1), members_.begin() + *result);
                    }
                }
            }
            build_index();
        }

        template<class InputIt>
        json_object(InputIt first, InputIt last, 
                    const allocator_type& alloc)
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc)), 
              index_(index_allocator_type(alloc))
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(get_key_value<KeyT,Json>()(*s));
            }

            build_index();
            auto last_unique = std::unique(index_.begin(), index_.end(),
                [&](std::size_t a, std::size_t b) { return !(members_.at(a).key().compare(members_.at(b).key())); });

            if (last_unique != index_.end())
            {
                index_.erase(last_unique, index_.end());
                std::sort(index_.begin(), index_.end());

                auto result = index_.rbegin();
                if (*result != members_.size())
                {
                    members_.erase(members_.begin() + (*result + 1), members_.end());
                }
                for (auto it = index_.rbegin() + 1; it != index_.rend(); ++it, ++result)
                {
                    if (*result - *it > 1)
                    {
                        members_.erase(members_.begin() + (*it + 1), members_.begin() + *result);
                    }
                }
            }
            build_index();
        }

        json_object(std::initializer_list<std::pair<std::basic_string<char_type>,Json>> init, 
                    const allocator_type& alloc = allocator_type())
            : allocator_holder<allocator_type>(alloc), 
              members_(key_value_allocator_type(alloc)), 
              index_(index_allocator_type(alloc))
        {
            members_.reserve(init.size());
            for (auto& item : init)
            {
                insert_or_assign(item.first, item.second);
            }
        }

        void swap(json_object& val) noexcept
        {
            members_.swap(val.members_);
        }

        iterator begin()
        {
            return iterator(members_.data());
        }

        iterator end()
        {
            return iterator(members_.data() + members_.size());
        }

        const_iterator begin() const
        {
            return const_iterator(members_.data());
        }

        const_iterator end() const
        {
            return const_iterator(members_.data() + members_.size());
        }

        std::size_t size() const {return members_.size();}

        std::size_t capacity() const {return members_.capacity();}

        void clear() 
        {
            members_.clear();
            index_.clear();
        }

        void shrink_to_fit() 
        {
            for (std::size_t i = 0; i < members_.size(); ++i)
            {
                members_[i].shrink_to_fit();
            }
            members_.shrink_to_fit();
            index_.shrink_to_fit();
        }

        void reserve(std::size_t n) {members_.reserve(n);}

        Json& at(std::size_t i) 
        {
            if (i >= members_.size())
            {
                JSONCONS_THROW(json_runtime_error<std::out_of_range>("Invalid array subscript"));
            }
            return members_[i].value();
        }

        const Json& at(std::size_t i) const 
        {
            if (i >= members_.size())
            {
                JSONCONS_THROW(json_runtime_error<std::out_of_range>("Invalid array subscript"));
            }
            return members_[i].value();
        }

        iterator find(const string_view_type& name) noexcept
        {
            auto it = std::lower_bound(index_.begin(),index_.end(), name, 
                                        [&](std::size_t i, const string_view_type& k) -> bool {return string_view_type(members_.at(i).key()).compare(k) < 0;});        
            if (it != index_.end() && members_.at(*it).key() == name)
            {
                return from_container_iterator(members_.begin() + *it);
            }
            else
            {
                return from_container_iterator(members_.end());
            }
        }

        const_iterator find(const string_view_type& name) const noexcept
        {
            auto it = std::lower_bound(index_.begin(),index_.end(), name, 
                                        [&](std::size_t i, const string_view_type& k) -> bool {return string_view_type(members_.at(i).key()).compare(k) < 0;});        
            if (it != index_.end() && members_.at(*it).key() == name)
            {
                return from_container_iterator(members_.begin() + *it);
            }
            else
            {
                return end();
            }
        }

        void erase(const_iterator first, const_iterator last) 
        {
            auto first2 = to_container_iterator(first)             ;
            auto last2 = to_container_iterator(last)             ;

            std::size_t pos1 = first2 == members_.end() ? members_.size() : first2 - members_.begin();
            std::size_t pos2 = last2 == members_.end() ? members_.size() : last2 - members_.begin();

            if (pos1 < members_.size() && pos2 <= members_.size())
            {
                erase_index_entries(pos1,pos2);

    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
                auto it1 = members_.begin() + (first2 - members_.begin());
                auto it2 = members_.begin() + (last2 - members_.begin());
                members_.erase(it1,it2);
    #else
                members_.erase(first2,last2);
    #endif
                //build_index();
            }
        }

        void erase(const string_view_type& name) 
        {
            auto pos = to_container_iterator(find(name));
            if (pos != members_.end())
            {
                std::size_t pos1 = pos - members_.begin();
                std::size_t pos2 = pos1 + 1;

                erase_index_entries(pos1, pos2);
    #if defined(JSONCONS_NO_ERASE_TAKING_CONST_ITERATOR)
                auto it = members_.begin() + (pos - members_.begin());
                members_.erase(it);
    #else
                members_.erase(pos);
    #endif
            }
        }

        template<class InputIt, class Convert>
        void insert(InputIt first, InputIt last, Convert convert)
        {
            std::size_t count = std::distance(first,last);
            members_.reserve(members_.size() + count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(convert(*s));
            }

            build_index();
            auto last_unique = std::unique(index_.begin(), index_.end(),
                [&](std::size_t a, std::size_t b) { return !(members_.at(a).key().compare(members_.at(b).key())); });

            if (last_unique != index_.end())
            {
                index_.erase(last_unique, index_.end());
                std::sort(index_.begin(), index_.end());

                auto result = index_.rbegin();
                if (*result != members_.size())
                {
                    members_.erase(members_.begin() + (*result + 1), members_.end());
                }
                for (auto it = index_.rbegin() + 1; it != index_.rend(); ++it, ++result)
                {
                    if (*result - *it > 1)
                    {
                        members_.erase(members_.begin() + (*it + 1), members_.begin() + *result);
                    }
                }
            }
            build_index();
        }

        template<class InputIt, class Convert>
        void insert(sorted_unique_range_tag, InputIt first, InputIt last, Convert convert)
        {
            std::size_t count = std::distance(first,last);

            members_.reserve(members_.size() + count);
            for (auto s = first; s != last; ++s)
            {
                members_.emplace_back(convert(*s));
            }

            build_index();
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<is_stateless<A>::value,std::pair<iterator,bool>>::type
        insert_or_assign(const string_view_type& name, T&& value)
        {
            auto result = insert_index_entry(name,members_.size());
            if (result.second)
            {
                members_.emplace_back(key_type(name.begin(), name.end()), std::forward<T>(value));
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),true);
            }
            else
            {
                auto it = members_.begin() + result.first;
                it->value(Json(std::forward<T>(value)));
                return std::make_pair(from_container_iterator(it),false);
            }
        }

        template <class T, class A=allocator_type>
        typename std::enable_if<!is_stateless<A>::value,std::pair<iterator,bool>>::type
        insert_or_assign(const string_view_type& name, T&& value)
        {
            auto result = insert_index_entry(name,members_.size());
            if (result.second)
            {
                members_.emplace_back(key_type(name.begin(),name.end(),get_allocator()), 
                                      std::forward<T>(value),get_allocator());
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),true);
            }
            else
            {
                auto it = members_.begin() + result.first;
                it->value(Json(std::forward<T>(value),get_allocator()));
                return std::make_pair(from_container_iterator(it),false);
            }
        }

        template <class T>
        iterator insert_or_assign(iterator hint, const string_view_type& key, T&& value)
        {
            return from_container_iterator(insert_or_assign(to_container_iterator(hint), key, std::forward<T>(value)));
        }

        // merge

        void merge(const json_object& source)
        {
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                try_emplace(it->key(),it->value());
            }
        }

        void merge(json_object&& source)
        {
            auto it = std::make_move_iterator(source.members_.begin());
            auto end = std::make_move_iterator(source.members_.end());
            for (; it != end; ++it)
            {
                auto pos = to_container_iterator(find(it->key()));
                if (pos == members_.end())
                {
                    try_emplace(it->key(),std::move(it->value()));
                }
            }
        }

        void merge(iterator hint, const json_object& source)
        {
            auto hint2 = to_container_iterator(hint);
            std::size_t pos = hint2 - members_.begin();
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                hint2 = try_emplace(hint2, it->key(),it->value());
                std::size_t newpos = hint2 - members_.begin();
                if (newpos == pos)
                {
                    ++hint2;
                    pos = hint2 - members_.begin();
                }
                else
                {
                    hint2 = members_.begin() + pos;
                }
            }
        }

        void merge(iterator hint, json_object&& source)
        {
            auto hint2 = to_container_iterator(hint);
            std::size_t pos = hint2 - members_.begin();

            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                hint2 = try_emplace(hint2, it->key(), std::move(it->value()));
                std::size_t newpos = hint2 - members_.begin();
                if (newpos == pos)
                {
                    ++hint2;
                    pos = hint2 - members_.begin();
                }
                else
                {
                    hint2 = members_.begin() + pos;
                }
            }
        }

        // merge_or_update

        void merge_or_update(const json_object& source)
        {
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                insert_or_assign(it->key(),it->value());
            }
        }

        void merge_or_update(json_object&& source)
        {
            auto it = std::make_move_iterator(source.members_.begin());
            auto end = std::make_move_iterator(source.members_.end());
            for (; it != end; ++it)
            {
                typename container_type::iterator pos = to_container_iterator(find(it->key()));
                if (pos == members_.end())
                {
                    insert_or_assign(it->key(),std::move(it->value()));
                }
                else
                {
                    pos->value(std::move(it->value()));
                }
            }
        }

        void merge_or_update(iterator hint, const json_object& source)
        {
            auto hint2 = to_container_iterator(hint);
            std::size_t pos = hint2 - members_.begin();
            for (auto it = source.begin(); it != source.end(); ++it)
            {
                hint2 = insert_or_assign(hint2, it->key(),it->value());
                std::size_t newpos = hint2 - members_.begin();
                if (newpos == pos)
                {
                    ++hint2;
                    pos = hint2 - members_.begin();
                }
                else
                {
                    hint2 = members_.begin() + pos;
                }
            }
        }

        void merge_or_update(iterator hint, json_object&& source)
        {
            auto hint2 = to_container_iterator(hint);

            std::size_t pos = hint2 - members_.begin();
            auto it = std::make_move_iterator(source.begin());
            auto end = std::make_move_iterator(source.end());
            for (; it != end; ++it)
            {
                hint2 = insert_or_assign(hint2, it->key(),std::move(it->value()));
                std::size_t newpos = hint2 - members_.begin();
                if (newpos == pos)
                {
                    ++hint2;
                    pos = hint2 - members_.begin();
                }
                else
                {
                    hint2 = members_.begin() + pos;
                }
            }
        }

        // try_emplace

        template <class A=allocator_type, class... Args>
        typename std::enable_if<is_stateless<A>::value,std::pair<iterator,bool>>::type
        try_emplace(const string_view_type& name, Args&&... args)
        {
            auto result = insert_index_entry(name,members_.size());
            if (result.second)
            {
                members_.emplace_back(key_type(name.begin(), name.end()), std::forward<Args>(args)...);
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),true);
            }
            else
            {
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),false);
            }
        }

        template <class A=allocator_type, class... Args>
        typename std::enable_if<!is_stateless<A>::value,std::pair<iterator,bool>>::type
        try_emplace(const string_view_type& key, Args&&... args)
        {
            auto result = insert_index_entry(key,members_.size());
            if (result.second)
            {
                members_.emplace_back(key_type(key.begin(),key.end(), get_allocator()), 
                                      std::forward<Args>(args)...);
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),true);
            }
            else
            {
                auto it = members_.begin() + result.first;
                return std::make_pair(from_container_iterator(it),false);
            }
        }
     
        template <class A=allocator_type, class ... Args>
        typename std::enable_if<is_stateless<A>::value,iterator>::type
        try_emplace(iterator hint, const string_view_type& key, Args&&... args)
        {
            auto hint2 = to_container_iterator(hint);

            if (hint2 == members_.end())
            {
                auto result = try_emplace(key, std::forward<Args>(args)...);
                return from_container_iterator(result.first);
            }
            else
            {
                std::size_t pos = hint2 - members_.begin();
                auto result = insert_index_entry(key, pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint2, key_type(key.begin(), key.end()), std::forward<Args>(args)...);
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    return it;
                }
            }
        }

        template <class A=allocator_type, class ... Args>
        typename std::enable_if<!is_stateless<A>::value,iterator>::type
        try_emplace(iterator hint, const string_view_type& key, Args&&... args)
        {
            auto hint2 = to_container_iterator(hint);

            if (hint2 == members_.end())
            {
                auto result = try_emplace(key, std::forward<Args>(args)...);
                return from_container_iterator(result.first);
            }
            else
            {
                std::size_t pos = hint2 - members_.begin();
                auto result = insert_index_entry(key, pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint2, 
                                               key_type(key.begin(),key.end(), get_allocator()), 
                                               std::forward<Args>(args)...);
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    return it;
                }
            }
        }

        bool operator==(const json_object& rhs) const
        {
            return members_ == rhs.members_;
        }
     
        bool operator<(const json_object& rhs) const
        {
            return members_ < rhs.members_;
        }
    private:

        std::pair<std::size_t,bool> insert_index_entry(const string_view_type& key, std::size_t pos)
        {
            JSONCONS_ASSERT(pos <= index_.size());

            auto it = std::lower_bound(index_.begin(),index_.end(), key, 
                                        [&](std::size_t i, const string_view_type& k) -> bool {return string_view_type(members_.at(i).key()).compare(k) < 0;});        

            if (it == index_.end())
            {
                std::size_t count = index_.size() - pos;
                for (std::size_t i = 0; count > 0 && i < index_.size(); ++i)
                {
                    if (index_[i] >= pos)
                    {
                        ++index_[i];
                        --count;
                    }
                }
                index_.push_back(pos);
                return std::make_pair(pos,true);
            }
            else if (members_.at(*it).key() != key)
            {
                std::size_t count = index_.size() - pos;
                for (std::size_t i = 0; count > 0 && i < index_.size(); ++i)
                {
                    if (index_[i] >= pos)
                    {
                        ++index_[i];
                        --count;
                    }
                }
                auto it2 = index_.insert(it, pos);
                return std::make_pair(*it2,true);
            }
            else
            {
                return std::make_pair(*it,false);
            }
        }

        void erase_index_entries(std::size_t pos1, std::size_t pos2)
        {
            JSONCONS_ASSERT(pos1 <= pos2);
            JSONCONS_ASSERT(pos2 <= index_.size());

            const size_t offset = pos2 - pos1;
            const size_t n = index_.size() - offset;
            for (std::size_t i = 0; i < index_.size(); ++i)
            {
                if (offset == index_.size())
                {
                    index_.erase(index_.begin()+i,index_.end());
                    i += offset;
                }
                else if (index_[i] >= pos1 && index_[i] < pos2)
                {
                    index_.erase(index_.begin()+i);
                }
            }
            for (std::size_t i = 0; i < index_.size(); ++i)
            {
                if (index_[i] >= pos2)
                {
                    index_[i] -= offset;
                }
            }
            JSONCONS_ASSERT(index_.size() == n);
        }

        void build_index()
        {
            index_.clear();
            index_.reserve(members_.size());
            for (std::size_t i = 0; i < members_.size(); ++i)
            {
                index_.push_back(i);
            }
            std::stable_sort(index_.begin(),index_.end(),
                             [&](std::size_t a, std::size_t b) -> bool {return members_.at(a).key().compare(members_.at(b).key()) < 0;});
        }

        json_object& operator=(const json_object&) = delete;

        typename container_type::iterator to_container_iterator(iterator it)
        {
            return members_.begin() + (it - begin());
        }

        typename container_type::const_iterator to_container_iterator(const_iterator it) const
        {
            return members_.begin() + (it - begin());
        }

        iterator from_container_iterator(typename container_type::iterator it)
        {
            return iterator(members_.data() + (it - members_.begin()));
        }

        const_iterator from_container_iterator(typename container_type::const_iterator it) const
        {
            return const_iterator(members_.data() + (it - members_.begin()));
        }

        template <class A=allocator_type, class T>
        typename std::enable_if<is_stateless<A>::value,typename container_type::iterator>::type 
        insert_or_assign(typename container_type::iterator hint, const string_view_type& key, T&& value)
        {
            if (hint == members_.end())
            {
                auto result = insert_or_assign(key, std::forward<T>(value));
                return to_container_iterator(result.first);
            }
            else
            {
                std::size_t pos = hint - members_.begin();
                auto result = insert_index_entry(key,pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint, key_type(key.begin(), key.end()), std::forward<T>(value));
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    it->value(Json(std::forward<T>(value)));
                    return it;
                }
            }
        }

        template <class A=allocator_type, class T>
        typename std::enable_if<!is_stateless<A>::value,typename container_type::iterator>::type 
        insert_or_assign(typename container_type::iterator hint, const string_view_type& key, T&& value)
        {
            if (hint == members_.end())
            {
                auto result = insert_or_assign(key, std::forward<T>(value));
                return result.first;
            }
            else
            {
                std::size_t pos = hint - members_.begin();
                auto result = insert_index_entry(key,pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint, 
                                               key_type(key.begin(),key.end(),get_allocator()), 
                                               std::forward<T>(value),get_allocator());
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    it->value(Json(std::forward<T>(value),get_allocator()));
                    return it;
                }
            }
        }

        template <class A=allocator_type, class ... Args>
        typename std::enable_if<is_stateless<A>::value,typename container_type::iterator>::type
        try_emplace(typename container_type::iterator hint, const string_view_type& key, Args&&... args)
        {
            if (hint == members_.end())
            {
                auto result = try_emplace(key, std::forward<Args>(args)...);
                return to_container_iterator(result.first);
            }
            else
            {
                std::size_t pos = hint - members_.begin();
                auto result = insert_index_entry(key, pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint, key_type(key.begin(), key.end()), std::forward<Args>(args)...);
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    return it;
                }
            }
        }

        template <class A=allocator_type, class ... Args>
        typename std::enable_if<!is_stateless<A>::value,typename container_type::iterator>::type
        try_emplace(typename container_type::iterator hint, const string_view_type& key, Args&&... args)
        {
            if (hint == members_.end())
            {
                auto result = try_emplace(key, std::forward<Args>(args)...);
                return result.first;
            }
            else
            {
                std::size_t pos = hint - members_.begin();
                auto result = insert_index_entry(key, pos);

                if (result.second)
                {
                    auto it = members_.emplace(hint, 
                                               key_type(key.begin(),key.end(), get_allocator()), 
                                               std::forward<Args>(args)...);
                    return it;
                }
                else
                {
                    auto it = members_.begin() + result.first;
                    return it;
                }
            }
        }
    };

} // namespace jsoncons

#endif
