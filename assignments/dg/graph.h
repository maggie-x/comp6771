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

    typedef std::tuple<std::shared_ptr<N>, std::shared_ptr<N>, E>  Edge;

 public:

    struct Node {

        Node(const N &val) : val(std::make_shared<N>(val)) {}
        std::shared_ptr<N> val;

        friend bool operator==(const Node &a, const Node &b) {
            return *(a.val) ==  *(b.val);
        }

        friend bool operator<(const Node &a, const Node &b) {
            return *(a.val) <  *(b.val);
        }

    };

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

private:
    std::set<Node> nodes_;
    std::set<Edge> edges_;
};

// creates a vector<N> containing val in
// the first index, and adds it to adj list
template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {
    Node shared_node{val};
    auto result = nodes_.insert(shared_node);
    // maggie's comment:
    // you need to return false if it already existed
    // do something like
    // auto result = nodes_.insert(temp_node);
    // return result.second; as per http://www.cplusplus.com/reference/set/set/insert/
    return result.second;
}

template <typename N, typename E>
bool Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w) {
    // If src or dst doesn't exist already, we should throw an error
    if (!IsNode(src) || !IsNode(dst)) return false; //AND THROW AN ERROR
    auto src_ptr = *(nodes_.find(Node{src}));
    auto dst_ptr = *(nodes_.find(Node{dst}));
    Edge e = std::make_tuple(src_ptr, dst_ptr, w);
    edges_.insert(e);
    return true;
}

template <typename N, typename E>
bool Graph<N,E>::DeleteNode(const N& val) {
    if (!IsNode(val)) return false; // if the node doesn't exist, there's nothing to delete
    delete nodes_.find(val);
    // for (typename std::set<Edge>::iterator it = edges_.begin(); it != edges_.end(); ++it) {
    //     if (std::get<0>(*it) == val || std::get<1>(*it) == val) { // if any of the connecting nodes in the edges is the node we're deleting, just delete the edge
    //         delete *it;
    //     }
    // }
    
    // delete all edge entries involving this node
}

template <typename N, typename E>
bool Graph<N,E>::IsNode(const N &val) {
    return nodes_.find(Node{val}) != nodes_.end();
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

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_