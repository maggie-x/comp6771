#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <tuple>
#include <set>
#include <utility>
#include <algorithm>

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
            std::cout << "InsertOutgoing(" << *dst << ", " << weight << ")" << std::endl;
            Edge e = std::make_pair(dst, weight);
            auto result = edges_.insert(e);
            std::cout << *val << " has edges: " << edges_.size() << std::endl;
            return result.second;
        }

        void CleanOutgoing(const N &src) {
            std::cout << "CleanOutgoing(" << src << ")" << std::endl;
            auto it = edges_.cbegin();
            while (it != edges_.cend()) {

                std::cout << " *(it->first): " << *(it->first) << std::endl;
                
                if (*(it->first) == src) { // found a dst which is an src
                    edges_.erase(it++);
                } else {
                    ++it;
                }
            }
        }

        bool HasOutgoing(const N &dst) {
            for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
                if (*(it->first) == dst) {
                    return true;
                }
            }

            return false;
        }

        std::set<N> GetOutgoing() {
          std::set<N> outgoing;
          for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
              outgoing.emplace(*(it->first));
          }

          return outgoing;
        }

        std::vector<E> GetDestWeights(const N &dst) {
          std::vector<E> weights;
          for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
            if (*(it->first) == dst){
              weights.emplace_back(it->second);
            }
          }
          std::sort(weights.begin(), weights.end());
          return weights;
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
  bool Replace(const N& oldData, const N& newData);
//   void MergeReplace(const N& oldData, const N& newData)
  void Clear();
  bool IsNode(const N& val);
  bool IsConnected(const N& src, const N& dst);
  std::vector<N> GetNodes();
  std::vector<N> GetConnected(const N& src);
  std::vector<E> GetWeights(const N& src, const N& dst);
//   const_iterator find(const N&, const N&, const E&);
//   bool erase(const N& src, const N& dst, const E& w)
//   const_iterator erase(graph_const_iterator it);
//   const_iterator begin();
//   const_iterator end();
//   const_iterator rbegin();
//   const_iterator rend();
//   const_iterator cbegin();
//   const_iterator cend();
//   const_iterator crbegin();
//   const_iterator crend();

  // FRIENDS

    friend bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
        // check both graphs have same nodes and edges
        for (auto n : g1.nodes_) {
            // checking nodes
            if (g2.nodes_.find(n) != g2.nodes_.end()) {
                auto n2_iterator = g2.nodes_.find(n);
                auto n2 = *n2_iterator;     // this is the corresponding node in g2
                // check edges
                for (auto e : n.edges_) {
                    // if the corresponding node in the second graph has the same edge, continue checking
                    // if it doesn't the graphs are not equal so return false
                    auto weight = e.second;
                    // iterate through the corresponding node in the second graph to find the same edge
                    unsigned long count = 0;
                    for (auto e2 : n2.edges_) {
                        if (e2.second == weight) break;
                        count++;
                        if (count == n2.edges_.size()) return false; // if we reach the end of n2 then the edge doesnt exist
                    }
                }
            }
            // if the second graph doesn't contain the same node, return false
            else {
                return false;
            }
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
    std::cout << "-- DeleteNode(" << val << ")" << std::endl;
    if (!IsNode(val)) return false; // if the node doesn't exist, there's nothing to delete

    auto val_it = nodes_.find(Node{val});
    nodes_.erase(val_it); 

     // then delete all the other shared_ptrs so ref_count == 0
     // and underlying object deleted ie no memory leak
    
    auto it = nodes_.begin();
    while (it != nodes_.end()) {

        auto clean_node = *it;
        clean_node.CleanOutgoing(val);
        nodes_.erase(it++);
        nodes_.insert(clean_node);
        
    }

    return false;
}

template <typename N, typename E>
bool Graph<N,E>::Replace(const N& oldData, const N& newData) {
    if (!IsNode(oldData)) {
        throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
    }


    if (IsNode(newData)) {
        return false;
    }

    auto replace_it = nodes_.find(Node{oldData});
    auto replace_node = *replace_it;
    nodes_.erase(replace_it);
    *(replace_node.val) = newData;
    auto result = nodes_.insert(replace_node);

    return result.second;
}

template <typename N, typename E>
bool Graph<N,E>::IsNode(const N &val) {
    return nodes_.find(Node{val}) != nodes_.end();
}


// template <typename N, typename E>    
// void Graph<N,E>::MergeReplace(const N& oldData, const N& newData) {}


template <typename N, typename E>
void Graph<N,E>::Clear() {

    /* will call destructors on all fields and child classes (i believe)
    and since we are using smart pointers, underlying object will be
    clean up automatically*/
    nodes_.clear();
}

template <typename N, typename E>
bool Graph<N,E>::IsConnected(const N& src, const N& dst){

    if (!IsNode(src) || !IsNode(dst)) {
        throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }

    auto src_node = *(nodes_.find(Node{src}));

    return src_node.HasOutgoing(dst);

}

template <typename N, typename E>
std::vector<N> Graph<N,E>::GetNodes() {
  std::vector<N> vector_of_nodes;
  for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
    vector_of_nodes.emplace_back((*(it->val)));
  }

  std::sort(vector_of_nodes.begin(), vector_of_nodes.end());
  return vector_of_nodes;
}

template <typename N, typename E>
std::vector<N> Graph<N,E>::GetConnected(const N& src) {
  if (!IsNode(src)) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  auto src_node = *(nodes_.find(Node{src}));
  auto connected_set = src_node.GetOutgoing();
  std::vector<N> connected_vector(connected_set.begin(), connected_set.end());
  return connected_vector;

}

template <typename N, typename E>
std::vector<E> Graph<N,E>::GetWeights(const N& src, const N& dst) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  auto src_node = *(nodes_.find(Node{src}));
  return src_node.GetDestWeights(dst);

}

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_