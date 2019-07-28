#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <tuple>
#include <set>

namespace gdwg {


template <typename N, typename E>
class Graph {
    typedef N Node;
    typedef std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>, E>  Edge;

 public:

//   class const_iterator {};

  // default constructor
  Graph() : nodes_(), edges_() {}

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

  friend bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
  friend bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);

private:
    std::set<Node> nodes_;
    std::set<Edge> edges_;

    /*std::vector<Node> adj_list_;
    std::map<N, std::unique_ptr<N>> object_map_;
    int node_count_;*/
};

// creates a vector<N> containing val in
// the first index, and adds it to adj list
template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {
    Node temp_node = val;
    nodes_.insert(temp_node);
    return true; // since its a set, it automatically checks if the node already exists and only inserts if it doesn't
}

template <typename N, typename E>
bool Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w) {
    // If src or dst doesn't exist already, we should throw an error
    if (!IsNode(src) || !IsNode(dst)) return false; //AND THROW AN ERROR
    std::shared_ptr src_ptr = std::make_shared<Node>(src);
    std::shared_ptr dst_ptr = std::make_shared<Node>(dst);
    Edge e = std::make_tuple(src_ptr, dst_ptr, w);
    edges_.insert(e);
    return true;
}

template <typename N, typename E>
bool Graph<N,E>::DeleteNode(const N& val) {
    if (!IsNode(val)) return false; // if the node doesn't exist, there's nothing to delete
    for (typename std::set<Edge>::iterator it = edges_.begin(); it != edges_.end(); ++it) {
        if (std::get<0>(*it) == val || std::get<1>(*it) == val) { // if any of the connecting nodes in the edges is the node we're deleting, just delete the edge
            delete *it;
        }
    }
    // once we finish the loop, all the edges that have the target node connected will be deleted
    // this will delete all the shared pointers to that target node, effectively deleting it and the edges at once
}

template <typename N, typename E>
bool Graph<N,E>::IsNode(const N &val) {
    if (nodes_.contains(val)) return true;
    return false;
}

/*bool Graph<N, E>::InsertNode(const N &val) {
  if (this->IsNode(val) == false) {
    auto new_node_ptr = std::make_unique<N>(val);
    E empty{};
    Edge new_node{new_node_ptr, empty};

    std::cout << "value unique ptr : " << *new_node_ptr;
    std::cout << "empty: " << empty;

    std::vector<Edge> new_vertex{new_node};

    Node new_vertex_ptr = std::make_unique<std::vector<Edge>>(new_vertex); // does this make a copy of the object?
    adj_list_.emplace_back(new_vertex_ptr);

    this->object_map_[val] = new_node_ptr; // add this mapping of object value to 

    return true;
  }

  return false;
}*/

template <typename N, typename E>
bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
    // check each node exists
    for (N n : g1.nodes_) {
        if (g2.nodes_.contains(n)) continue;
        else return false;
    }

    // check each edge exists
    for (std::tuple<std::shared_ptr<N>, std::shared_ptr<N>, E> e : g1.edges_) {
        if (g2.edges_.contains(e)) continue;
        else return false;
    }

    // if we reach here then that means each edge/node exists in both graphs
    return true;
}

template <typename N, typename E>
bool operator!=(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
    // using previously defined == operator
    if (g1 == g2) return false;
    return true;
}

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_