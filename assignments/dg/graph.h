#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <memory>
#include <iostream>
#include <map>

namespace gdwg {


template <typename N, typename E>
class Graph {
  typedef std::pair<std::unique_ptr<N>, E> Edge;
  typedef std::unique_ptr<std::vector<Edge>> Vertex;

 public:

//   class const_iterator {};

  // default constructor
  Graph() : adj_list_(), object_map_(), node_count_(0) {
     
  }

  // regular iterator constructor
//   Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) {
//       for (typename std::vector<N>::const_iterator it = begin; it != end; it++) {
//           // call insertNode method to insert each node from the vector
//       }
//   }

//   Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
//           typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
//       for (typename std::vector<std::tuple<N, N, E>>::const_iterator it = begin; it != end; it++) {
//         for (int i = 0; i < 3; ++i) {
//             // iterate through the tuple and insert the nodes/edges IF THEY DONT ALREADY EXIST
//         }
//       }
//   }

  //    METHODS

  // inserts node into a graph
  bool InsertNode(const N& val);
  // inserts edge into graph
  // bool InsertEdge(const N& src, const N& dst, const E& w);
  // deletes node
  // bool DeleteNode(const N&);
 /* bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData)
  void Clear()*/
  // bool IsNode(const N& val);
  // bool IsConnected(const N& src, const N& dst);
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
    std::vector<Vertex> adj_list_;
    std::map<N, std::unique_ptr<N>> object_map_;
    int node_count_;
};

// creates a vector<N> containing val in the first index, and adds it to adj list
template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {
  if (this->IsNode(val) == false) {
    auto new_node_ptr = std::make_unique<N>(val);
    E empty{};
    Edge new_node{new_node_ptr, empty};

    std::cout << "value unique ptr : " << *new_node_ptr;
    std::cout << "empty: " << empty;

    std::vector<Edge> new_vertex{new_node};

    Vertex new_vertex_ptr = std::make_unique<std::vector<Edge>>(new_vertex); // does this make a copy of the object?
    adj_list_.emplace_back(new_vertex_ptr);

    this->object_map_[val] = new_node_ptr; // add this mapping of object value to 

    return true;
  }

  return false;
}

// template <typename N, typename E>
// bool Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {
//     if (object_map_.find(src) == object_map_.end() || object_map_.find(dst) == object_map_.end()) {
//         throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
//     }



// }

// template <typename N, typename E>
// bool Graph<N, E>::DeleteNode(const N &) {

// }

// // iterates through adj list and checks if the val already exists
// template <typename N, typename E>
// bool Graph<N,E>::IsNode(const N& val){
//     for (std::vector<N> n : adj_list_) {
//         if (*(n.at(0)) == val) return true;
//     }
//     return false;
// }

// // iterates through the adjacency list and sees if the src is connected to the dst
// template <typename N, typename E>
// bool Graph<N, E>::IsConnected(const N &src, const N &dst) {
//     for (std::vector<N> n : adj_list_) {
//         if (n.at(0) == src){
//             for (N adj : n) {
//                 if (adj == dst) return true;
//             }
//         }
//     }
//     return false;
// }

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
