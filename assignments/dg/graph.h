#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  class const_iterator {};

  // insertnode:
  // malloc it, make unique (smart pointer to heap object)

 private:
 // adjacency list? easier to add nodes...
 // list of vectors 

 // adjacency matrix? easier to semantically determine src and dst

 // each node needs to store:
 // value
 // incoming and outgoing edges and their weights.

 // list of outgoing edges
 // vector<dest, weight>

 // new std::vector... to put it on heap;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
