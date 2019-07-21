#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:

  class const_iterator {};

  // default constructor
  Graph(N nodes, E edges) {

  }

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
 /* bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData)
  void Clear()*/
  bool IsNode(const N& val);
  bool IsConnected(const N& src, const N& dst);
  /*std::vector<N> GetNodes()
  std::vector<N> GetConnected(const N& src)
  std::vector<E> GetWeights(const N& src, const N& dst)
  const_iterator find(const N&, const N&, const E&);
  bool erase(const N& src, const N& dst, const E& w)
  const_iterator erase(graph_const_iterator it);
  const_iterator begin();
  const_iterator end();
  const_iterator rbegin();
  const_iterator rend();
  const_iterator cbegin();
  const_iterator cend();
  const_iterator crbegin();
  const_iterator crend();*/

private:
    std::vector<std::vector<N>> adj_list;
};

// creates a vector<N> containing val in the first index, and adds it to adj list
template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {
    if (this->IsNode(val) == false) {
        std::vector<N> temp;
        temp.emplace_back(val);
        adj_list.emplace_back(temp);
    }
}

template <typename N, typename E>
bool Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {

}

template <typename N, typename E>
bool Graph<N, E>::DeleteNode(const N &) {

}

// iterates through adj list and checks if the val already exists
template <typename N, typename E>
bool Graph<N,E>::IsNode(const N& val){
    for (std::vector<N> n : adj_list) {
        if (n.at(0) == val) return true;
    }
    return false;
}

// iterates through the adjacency list and sees if the src is connected to the dst
template <typename N, typename E>
bool Graph<N, E>::IsConnected(const N &src, const N &dst) {
    for (std::vector<N> n : adj_list) {
        if (n.at(0) == src){
            for (N adj : n) {
                if (adj == dst) return true;
            }
        }
    }
    return false;
}

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
