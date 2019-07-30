#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <tuple>
#include <set>
#include <utility>

namespace gdwg {


template <typename N, typename E>
class Graph {

 public:

    struct Node {
        typedef std::pair<std::shared_ptr<N>, E>  Edge;
        Node(const N &val) : val(std::make_shared<N>(val)), edges_() {}
        std::shared_ptr<N> val;
        std::set<Edge> edges_;

        friend bool operator==(const Node &a, const Node &b) {
            return *(a.val) ==  *(b.val);
        }

        friend bool operator<(const Node &a, const Node &b) {
            return *(a.val) < *(b.val);
        }

        bool InsertOutgoing(std::shared_ptr<N> dst, const E &weight) { // pass in shared ptr after finding node in set of nodes
            std::cout << "number of edges for node " << *val << " is " << edges_.size() << std::endl;
            std::cout << "  inserting new outgoing edge" << std::endl;
            std::cout << "  making pair from" << dst << " " << weight << std::endl;
            Edge e = std::make_pair(dst, weight);
            auto result = edges_.insert(e);
            std::cout << "number of edges for node " << *val << " is " << edges_.size() << std::endl;
            return result.second;
        }

        void CleanOutgoing(const N &src) {
            auto it = edges_.cbegin();
            while (it != edges_.cend()) {
                if (it->second == src) { // found a dst which is an src
                    edges_.erase(it++);
                } else {
                    ++it;
                }
            }
        }

        friend std::ostream& operator<<(std::ostream &os, const Node &node) {
            std::cout << "number of edges for node " << *(node.val) << " is " << node.edges_.size() << std::endl;
            os << *(node.val) << " (" << std::endl;
            for (auto it = node.edges_.cbegin(); it != node.edges_.cend(); ++it) {
                os << "  " << *((*it).first) << " | " <<  (*it).second << std::endl;
            }
            os << ")" << std::endl;

            return os;
        }
    };

//   class const_iterator {};

  // default constructor
  Graph() : nodes_() {}

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

  // FRIENDS

    friend bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
        // check both graphs have same nodes and edges
        for (auto n : g1.nodes_) {
            // checking nodes
            if (g2.nodes_.find(n) != g2.nodes_.end()) {
                auto n2_iterator = g2.nodes_.find(n);
                auto n2 = *n2_iterator;
                // check edges
                for (auto e : n.edges_) {
                    // if the corresponding node in the second graph has the same edge, continue checking
                    // if it doesn't the graphs are not equal so return false
                    if (n2.edges_.find(e) != n2.edges_.end()) continue;
                    else return false;
                }
            }
            // if the second graph doesn't contain the same node, return false
            else return false;
        }
        return true;
    }

    // using previously define == operator
    friend bool operator!=(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
        if (g1 == g2) return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream &os, const Graph<N, E> &graph) {
        std::cout << "--- PRINTING OUT GRAPH --- " << std::endl;
        std::cout << "nodes in graph: " << graph.nodes_.size() << std::endl;
        for (auto it = graph.nodes_.cbegin(); it != graph.nodes_.cend(); ++it) {
            os << *(it);
        }

        return os;
    }


private:
    std::set<Node> nodes_;
    // std::set<Edge> edges_;
};



template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) {
    Node new_node{val};
    auto result = nodes_.insert(new_node);
    return result.second;
}

template <typename N, typename E>
bool Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w) {
    if (!IsNode(src) || !IsNode(dst)) return false; //AND THROW AN ERROR
    auto src_it = nodes_.find(Node{src});
    auto src_node = *(src_it); 
    auto dst_node = *(nodes_.find(Node{dst}));
    auto result = src_node.InsertOutgoing(dst_node.val, w);
    nodes_.erase(src_it);
    nodes_.insert(src_node);
    return result;
}

template <typename N, typename E>
bool Graph<N,E>::DeleteNode(const N& val) {
    if (!IsNode(val)) return false; // if the node doesn't exist, there's nothing to delete

    auto val_it = nodes_.find(Node{val});
    nodes_.erase(val_it); 
     // then delete all the other shared_ptrs so no memory leak
    
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {

        auto curr_node = *it;
        curr_node.CleanOutgoing(val);
        
    }

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


}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_