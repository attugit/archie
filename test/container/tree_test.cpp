#include <archie/tree.hpp>
#include <catch.hpp>
#include <string>
#include <iostream>

namespace {
using namespace archie;
using snode = node_t<std::string>;
TEST_CASE("canMakeNode")
{
  snode::list_type list;
  auto nit = snode::make_root(list, "lorem");
  REQUIRE(nit->is_root());
  REQUIRE(nit->size() == 0);
  REQUIRE(nit->value() == "lorem");
  auto kid = nit->emplace("ipsum");
  REQUIRE(!kid->is_root());
  REQUIRE(nit->size() == 1);
  REQUIRE(kid->size() == 0);
  REQUIRE(kid->value() == "ipsum");
  REQUIRE(kid->empty());
  {
    tree_iterator<snode> end{std::end(list)};
    tree_iterator<snode> begin{kid};
    REQUIRE(begin->value() == "ipsum");
    REQUIRE(begin != end);
    ++begin;
    REQUIRE(begin == end);
  }
  kid->emplace("dolor");
  kid->emplace("sit");
  nit->emplace("amet")->emplace("consectetur");
  {
    tree_iterator<snode> end{std::end(list)};
    tree_iterator<snode> it{nit->begin()};
    REQUIRE(it->value() == "ipsum");
    ++it;
    REQUIRE(it->value() == "dolor");
    ++it;
    REQUIRE(it->value() == "sit");
    ++it;
    REQUIRE(it->value() == "amet");
    ++it;
    REQUIRE(it->value() == "consectetur");
    ++it;
    REQUIRE(it == end);
  }
}
TEST_CASE("canGetPath")
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
  REQUIRE(join(a->path()) == "/root/a");
  auto b = a->emplace("b");
  REQUIRE(join(b->path()) == "/root/a/b");
  auto c = b->emplace("c");
  REQUIRE(join(c->path()) == "/root/a/b/c");
  auto d = b->emplace("d");
  REQUIRE(join(d->path()) == "/root/a/b/d");
  auto e = a->emplace("e");
  REQUIRE(join(e->path()) == "/root/a/e");
  auto f = e->emplace("f");
  REQUIRE(join(f->path()) == "/root/a/e/f");
}
using stree = tree_t<std::string>;
TEST_CASE("canMakeTree")
{
  stree tree;
  REQUIRE(tree.size() == 0);
  REQUIRE(tree.empty());
  REQUIRE(std::begin(tree) == std::end(tree));
}
}
