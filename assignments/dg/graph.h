#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>

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
  // default constructor
  Graph(N nodes, E edges)  { }

  // regular iterator constructor
  Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) {
      for (typename std::vector<N>::const_iterator it = begin; it != end; it++) {
          // call insertNode method to insert each node from the vector
      }
  }

  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
          typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
      for (typename std::vector<std::tuple<N, N, E>>::const_iterator it = begin; it != end; it++) {
        for (int i = 0; i < 3; ++i) {
            // iterate through the tuple and insert the nodes/edges IF THEY DONT ALREADY EXIST
        }
      }
  }

  //    METHODS

  // inserts node into a graph
  bool InsertNode(const N& val);
  // inserts edge into graph
  bool InsertEdge(const N& src, const N& dst, const E& w);
  // deletes node
  bool DeleteNode(const N&);

 private:
    std::vector<std::vector<N>> adj_list;
};

template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {

}

template <typename N, typename E>
bool Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {

}

template <typename N, typename E>
bool Graph<N, E>::DeleteNode(const N &) {

}

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
