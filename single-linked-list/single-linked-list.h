#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node)
            : node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {
        }

        ~BasicIterator() = default;

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            if (rhs.node_ == nullptr && this->node_ == nullptr) {
                return true;
            }
            if (rhs.node_ == nullptr || this->node_ == nullptr) {
                return false;
            }
            return rhs.node_->next_node == this->node_->next_node;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            if (rhs.node_ == nullptr && this->node_ == nullptr) {
                return true;
            }
            if (rhs.node_ == nullptr || this->node_ == nullptr) {
                return false;
            }
            return rhs.node_->next_node == this->node_->next_node;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            this->node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator temp(*this);
            this->node_ = node_->next_node;
            return temp;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return this->node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &(this->node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        Node* ptr_at_last = &head_;
        for (const auto& value : values) {
            Node* new_node = new Node(value, nullptr);
            ptr_at_last->next_node = new_node;
            ptr_at_last = new_node;
            ++size_;
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        this->Clear();
        Node* ptr_at_last = &head_;
        for (auto it = other.begin(); it != other.end(); ++it) {
            Node* new_node = new Node(*it, nullptr);
            ptr_at_last->next_node = new_node;
            ptr_at_last = new_node;
            ++size_;
        }
    }

    ~SingleLinkedList() {
        this->Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0) ? true : false;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        Node* to_delete = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete to_delete;
        --size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) {
        Node* to_delete = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete to_delete;
        --size_;
        return Iterator(pos.node_->next_node);
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* to_delete = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete to_delete;
            to_delete = nullptr;
            --size_;
        }
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbegin();
    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList temp_list = SingleLinkedList(rhs);
        this->swap(temp_list);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        Node temp_node;
        temp_node.next_node = this->head_.next_node;
        size_t temp_size = this->size_;
        this->head_.next_node = other.head_.next_node;
        this->size_ = other.size_;
        other.head_.next_node = temp_node.next_node;
        other.size_ = temp_size;
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs) && lhs != rhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
