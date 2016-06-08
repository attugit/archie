#include <archie/tree.hpp>
#include <gtest/gtest.h>
#include <string>
#include <iostream>

namespace {
using namespace archie;
using snode = node_t<std::string>;
TEST(tree, canMakeNode)
{
  snode::list_type list;
  auto nit = snode::make_root(list, "lorem");
  EXPECT_TRUE(nit->is_root());
  EXPECT_EQ(nit->size(), 0u);
  EXPECT_EQ(nit->value(), "lorem");
  auto kid = nit->emplace("ipsum");
  EXPECT_FALSE(kid->is_root());
  EXPECT_EQ(nit->size(), 1u);
  EXPECT_EQ(kid->size(), 0u);
  EXPECT_EQ(kid->value(), "ipsum");
  EXPECT_TRUE(kid->empty());
  {
    tree_iterator<snode> end{std::end(list)};
    tree_iterator<snode> begin{kid};
    EXPECT_EQ(begin->value(), "ipsum");
    EXPECT_NE(begin, end);
    ++begin;
    EXPECT_EQ(begin, end);
  }
  kid->emplace("dolor");
  kid->emplace("sit");
  nit->emplace("amet")->emplace("consectetur");
  {
    tree_iterator<snode> end{std::end(list)};
    tree_iterator<snode> it{nit->begin()};
    EXPECT_EQ(it->value(), "ipsum");
    ++it;
    EXPECT_EQ(it->value(), "dolor");
    ++it;
    EXPECT_EQ(it->value(), "sit");
    ++it;
    EXPECT_EQ(it->value(), "amet");
    ++it;
    EXPECT_EQ(it->value(), "consectetur");
    ++it;
    EXPECT_EQ(it, end);
  }
}
TEST(tree, canGetPath)
{
  auto const join = [](auto const& lst) {
    std::string str;
    for (auto p : lst) {
      str += "/";
      str += p->value();
    }
    return str;
  };
  snode::list_type list;
  auto r = snode::make_root(list, "root");
  auto a = r->emplace("a");
  EXPECT_EQ(join(a->path()), "/root/a");
  auto b = a->emplace("b");
  EXPECT_EQ(join(b->path()), "/root/a/b");
  auto c = b->emplace("c");
  EXPECT_EQ(join(c->path()), "/root/a/b/c");
  auto d = b->emplace("d");
  EXPECT_EQ(join(d->path()), "/root/a/b/d");
  auto e = a->emplace("e");
  EXPECT_EQ(join(e->path()), "/root/a/e");
  auto f = e->emplace("f");
  EXPECT_EQ(join(f->path()), "/root/a/e/f");
}
using stree = tree_t<std::string>;
TEST(tree, canMakeTree)
{
  stree tree;
  EXPECT_EQ(tree.size(), 0u);
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(std::begin(tree), std::end(tree));
  tree.emplace("a");
  EXPECT_NE(std::begin(tree), std::end(tree));
  EXPECT_EQ(tree.size(), 1u);
}
}
