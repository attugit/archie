#pragma once
#include <numeric>
#include <memory>
#include <list>
#include <experimental/optional>
#include <boost/iterator/iterator_facade.hpp>

namespace archie {
struct root_t {
};

static auto const root = root_t{};

template <typename T, typename Allocator = std::allocator<T>>
struct node_t {
  using value_type = T;
  using list_type = std::list<node_t, Allocator>;
  using iterator = typename list_type::iterator;
  using size_type = typename list_type::size_type;

  template <typename... Args>
  static iterator make_root(list_type& siblings, Args&&... args)
  {
    auto it = siblings.insert(siblings.end(), node_t{root, std::forward<Args>(args)...});
    it->self_ = it;
    return it;
  }

  bool is_root() const { return !parent_; }
  auto parent() { return parent_.value(); }
  auto parent() const { return parent_.value(); }
  std::list<iterator> path() const
  {
    std::list<iterator> ret;
    auto it = self_;
    while (!it->is_root()) {
      ret.push_front(it);
      it = it->parent();
    }
    ret.push_front(it);
    return ret;
  }

  auto& value() { return value_; }
  auto const& value() const { return value_; }
  auto begin() { return children_.begin(); }
  auto begin() const { return children_.begin(); }
  auto end() { return children_.end(); }
  auto end() const { return children_.end(); }
  size_type size() const { return children_.size(); }
  bool empty() const { return children_.empty(); }
  template <typename... Args>
  auto emplace(Args&&... args)
  {
    auto it = children_.insert(children_.end(), node_t{self_, std::forward<Args>(args)...});
    it->self_ = it;
    return it;
  }

private:
  using parent_t = std::experimental::optional<iterator>;

  template <typename... Args>
  explicit node_t(iterator parent, Args&&... args)
      : value_(std::forward<Args>(args)...), parent_(parent), self_(), children_()
  {
  }

  template <typename... Args>
  explicit node_t(root_t, Args&&... args)
      : value_(std::forward<Args>(args)...),
        parent_(std::experimental::nullopt),
        self_(),
        children_()
  {
  }

  value_type value_;
  parent_t parent_;
  iterator self_;
  list_type children_;
};

template <typename Node>
struct tree_iterator
    : boost::iterator_facade<tree_iterator<Node>, Node, boost::forward_traversal_tag> {
  using node_iterator = typename Node::iterator;
  tree_iterator() = default;
  explicit tree_iterator(node_iterator n) : it(n) {}
private:
  template <typename T>
  bool equal(tree_iterator<T> const& other) const
  {
    return this->it == other.it;
  }

  void rollback()
  {
    auto parent = it->parent();
    while (true) {
      if (++it != parent->end()) return;
      it = parent;
      if (it->is_root()) {
        ++it;
        return;
      }
      parent = it->parent();
    }
  }

  void increment()
  {
    if (!it->empty()) {
      it = it->begin();
    }
    else if (it->is_root())
    {
      ++it;
    }
    else
    {
      rollback();
    }
  }

  auto& dereference() const { return *it; }
  friend class boost::iterator_core_access;
  node_iterator it;
};

template <typename T, typename Allocator = std::allocator<T>>
struct tree_t {
  using value_type = node_t<T, Allocator>;
  using size_type = typename value_type::size_type;
  using iterator = tree_iterator<value_type>;

private:
  template <typename Nodes>
  static size_type node_size(Nodes const&);

public:
  tree_t() = default;

  size_type size() const { return node_size(list_); }
  bool empty() const { return list_.empty(); }
  iterator begin() { return iterator{std::begin(list_)}; }
  iterator end() { return iterator{std::end(list_)}; }
  template <typename... Args>
  void emplace(Args&&... args)
  {
    value_type::make_root(list_, std::forward<Args>(args)...);
  }

private:
  typename value_type::list_type list_;
};

template <typename T, typename Allocator>
template <typename Nodes>
typename tree_t<T, Allocator>::size_type tree_t<T, Allocator>::node_size(Nodes const& n)
{
  static auto const fun = [](size_type s, value_type const& node) { return s + node_size(node); };
  return std::accumulate(std::begin(n), std::end(n), n.size(), fun);
}
}
