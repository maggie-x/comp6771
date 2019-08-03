#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you?", 3);

  std::set<gdwg::Graph<std::string, int>::Node> nodes = g.get_nodes();
  std::cout << "printing graph pre move\n";
  for (auto n : nodes) {
    std::cout << n.val<<"\n";
  }
  gdwg::Graph<std::string, int> g_move{std::move(g)};
  std::set<gdwg::Graph<std::string, int>::Node> nodes2 = g_move.get_nodes();
  std::cout << "printing newly move constructed graph\n";
  for (auto n : nodes2) {
    std::cout << n.val<<"\n";
  }
  std::set<gdwg::Graph<std::string, int>::Node> nodes3 = g.get_nodes();
  std::cout << "printing og graph\n";
  for (auto n : nodes3) {
    std::cout << n.val<<"\n";
  }
/*
  gdwg::Graph<std::string, int> g2{g};

  std::cout << g2 << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }*/
}
