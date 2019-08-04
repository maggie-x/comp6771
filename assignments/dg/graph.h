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
#include <iterator>

namespace gdwg {


template <typename N, typename E>
class Graph {
 public:
 typedef std::pair<std::shared_ptr<N>, std::shared_ptr<E>> Edge;

    /* ---- CUSTOM EDGE COMPARATOR (FOR NODE CLASS) ---- */
    // used to sort edges in the set, based on Node then weight
    struct edge_set_comparator {
      bool operator() (const Edge& a, const Edge &b) const {
        if (*(a.first) == *(b.first)) {
          return *(a.second) < *(b.second);
        }

        return *(a.first) < *(b.first);
      }
    }; // new


    /* ---- NODE CLASS (HELPER CLASS) ---- */

    struct Node {
      Node(const N &val) : val(std::make_shared<N>(val)), edges_() {}

      // Node move constructor
      // Node(Node &&node) : edges_(std::move(node.edges_)) {
      //   std::cout << "node which we moved to has edges " << edges_.size() << std::endl;
      // }
      std::shared_ptr<N> val;
      std::set<Edge, edge_set_comparator> edges_;


      friend bool operator==(const Node &a, const Node &b) {
          return *(a.val) ==  *(b.val);
      }

      friend bool operator<(const Node &a, const Node &b) {
          return *(a.val) < *(b.val);
      }

      bool InsertOutgoing (std::shared_ptr<N> dst, const E &weight) { // pass in shared ptr after finding node in set of nodes
          // std::cout << "InsertOutgoing(" << *dst << ", " << weight << ")" << std::endl;

          Edge e = std::make_pair(dst, std::make_shared<E>(weight));
          auto result = edges_.insert(std::move(e));
          // std::cout << *val << " has edges: " << edges_.size() << std::endl;
          return result.second;
      }

      bool EraseOutgoing(const N& dst, const E &weight) {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst && *(it->second) == weight) { // found
            edges_.erase(it);
            return true;
          }
        }
        // not found
        return false;
      }

      void ReplaceOutgoing(const N& old_dst, std::shared_ptr<N> new_dst) {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == old_dst) {
            InsertOutgoing(new_dst, *(it->second)); // inserting the exact same edge, just new destination
          }
        }

        CleanOutgoing(old_dst);
      }

      void CleanOutgoing(const N &src) {
        // std::cout << "CleanOutgoing(" << src << ")" << std::endl;
        auto it = edges_.cbegin();
        while (it != edges_.cend()) {
          // std::cout << " *(it->first): " << *(it->first) << std::endl;
          
          if (*(it->first) == src) { // found a dst which is an src
              edges_.erase(it++);
          } else {
              ++it;
          }
        }
      }

      bool HasOutgoing(const N &dst) const {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst) {
              return true;
          }
        }
        return false;
      }

      typename std::set<Edge>::const_iterator FindEdge(const N &dst, const E& weight) const {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst && *(it->second) == weight) {
            return it;
          }
        }
        return edges_.cend();
      }

      std::set<N> GetOutgoing() const {
        std::set<N> outgoing;
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          outgoing.emplace(*(it->first));
        }

        return outgoing;
      }

      std::vector<E> GetDestWeights(const N &dst) const {
        std::cout << *this;
        std::vector<E> weights;
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst){
            weights.emplace_back(*(it->second));
          }
        }
        // std::sort(weights.begin(), weights.end());
        return weights;
      }

      friend std::ostream& operator<<(std::ostream &os, const Node &node) {
          // std::cout << "number of edges for node " << *(node.val) << " is " << node.edges_.size() << std::endl;
          os << *(node.val) << " (" << std::endl;
          for (auto it = node.edges_.cbegin(); it != node.edges_.cend(); ++it) {
              os << "  " << *(it->first) << " | " <<  *(it->second) << std::endl;
          }
          os << ")" << std::endl;

          return os;
      }
    };



  /* ---- GRAPH ITERATOR ---- */
  class const_iterator {
    public:
    friend class Graph;
    using difference_type = std::ptrdiff_t;
    using value_type = std::tuple<const N, const N, const E>;
    using pointer = std::tuple<const N&, const N&, const E&>*;
    using reference = std::tuple<const N&, const N&, const E&>;
    using iterator_category = std::bidirectional_iterator_tag;

    explicit const_iterator(typename std::set<Node>::const_iterator nodes_pos, typename std::set<Edge>::const_iterator edges_pos) 
    : node_it_{ nodes_pos }, 
      edge_it_{ edges_pos } {};

    reference operator*() const { 
      return {*(node_it_->val), *(edge_it_->first), *(edge_it_->second)};
    };
    pointer operator->() const { return &(operator*()); };

    const_iterator& operator++() { 
      ++edge_it_;

      if (edge_it_ == node_it_->edges_.end()) {
        ++node_it_;
        // only iterating over edges, so if node has no edges, skip
        while(node_it_->edges_.size() == 0) { 
          ++node_it_;
        }
        edge_it_ = node_it_->edges_.begin();
      }

      return *this;
    }; // pre is the first one
    const_iterator operator++(int) { auto cpy {*this}; operator++(); return cpy; }; // post is the second

    const_iterator& operator--() {
        if (edge_it_ == node_it_->edges_.begin()) { // == begin
            --node_it_;

            while(node_it_->edges_.size() == 0) {
                --node_it_;
            }

            edge_it_ = node_it_->edges_.end(); // == end
        }
        --edge_it_;
        return *this;
    };
    const_iterator operator--(int) { auto cpy {*this}; operator--(); return cpy; };
  
    friend bool operator==(const_iterator a, const_iterator b) {
      return a.node_it_ == b.node_it_ && a.edge_it_ == b.edge_it_;
    }

    friend bool operator!=(const_iterator a, const_iterator b){
      return !(a == b);
    }

    private:
      typename std::set<Node>::const_iterator node_it_;
      typename std::set<Edge>::const_iterator edge_it_;
  }; 

  // using const_iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  
  const_iterator cbegin() const {
    // std::cout << nodes_.size() << std::endl;
    return const_iterator{nodes_.begin(), (nodes_.begin())->edges_.begin()};
  }
  const_iterator cend() const {
    // std::cout << (--(nodes_.end()))->edges_.size() << std::endl;
    return const_iterator{nodes_.end(), (nodes_.end())->edges_.begin()}; // you cant start at .end(), thats null
                                                                            // solution: have a prev pointer? like node* tail.
                                                                            // or set the end to be a dummy node
                                                                            // then have cend return the tail iterator of dummy
  }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }
  const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }
  const_reverse_iterator rbegin() const { return crbegin(); }
  const_reverse_iterator rend() const { return crend(); }

  const_iterator find(const N& a, const N& b, const E& weight) {
    const auto a_node = nodes_.find(Node{a});
    const auto b_node = a_node->FindEdge(b, weight);

    if (a_node == nodes_.end() || b_node == a_node->edges_.end()) {
      return cend();
    }

    return const_iterator(a_node, b_node);
  }

  bool erase(const N& src, const N& dst, const E& w);

  const_iterator erase(const_iterator it) {

    if (it == end()){
      return it;
    }

    auto src = std::get<0>(*it);
    auto dst = std::get<1>(*it);
    auto w = std::get<2>(*it);
    // increment the iterator to return the next element after deleted element
    ++it;
    auto result = erase(src, dst, w);

    if (!result) {
      return end();
    }

    
    return it;
  }
  
  //  CONSTRUCTORS

  // default constructor
  Graph() : nodes_() {}

  Graph(typename std::vector<N>::const_iterator start, typename std::vector<N>::const_iterator end) {
    for (auto it = start; it != end; ++it) {
      InsertNode(*it);
    }
  }

  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start, typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
    for (auto it = start; it != end; ++it) {
      InsertNode(std::get<0>(*it));
      InsertNode(std::get<1>(*it));
      InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
  }

  Graph(std::initializer_list<N> init) {
    for (auto it = init.begin(); it != init.end(); ++it){
      InsertNode(*it);
    }
  }

  // DESTRUCTOR
  ~Graph() = default;

  // COPY CONSTRUCTOR
  Graph(const Graph<N, E> &g) {

    // we just populate a completely new graph which its own copy of the objects on the heap
    for (auto it = g.begin(); it != g.end(); ++it) {
      InsertNode(std::get<0>(*it));
      InsertNode(std::get<1>(*it));
      InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    } 
  }

  // MOVE CONSTRUCTOR
  Graph(Graph<N,E> &&g) : nodes_{std::move(g.nodes_)} {
    //std::cout << "MOVE CONSTRUCTOR CALLED" << std::endl;
  }

  // COPY ASSIGNMENT
  Graph<N, E>& operator=(const Graph<N, E>& g) {
    Graph g_copy{g};
    std::swap(g_copy, this);
    return this;
  }

  // MOVE ASSIGNMENT
  Graph<N, E>& operator=(
    Graph<N, E>&& g) {
    Graph g_move{g};
    std::swap(g_move, this);
    return this;
  }

  //    METHODS (implementations in .tpp file)

  bool InsertNode(const N& val);
  bool InsertEdge(const N& src, const N& dst, const E& w);
  bool DeleteNode(const N&);
  bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData);
  void Clear();
  bool IsNode(const N& val);
  bool IsConnected(const N& src, const N& dst);
  std::vector<N> GetNodes();
  std::vector<N> GetConnected(const N& src);
  std::vector<E> GetWeights(const N& src, const N& dst);
  
  
  //  FRIENDS (stay in the .h file)

  friend bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
    // check both graphs have same nodes and edges
    for (const auto n : g1.nodes_) {
        // for (const auto it = g1.nodes_.begin(); it != g1.nodes_.end(); ++it) {
        // checking nodes
        if (g2.nodes_.find(n) != g2.nodes_.end()) {
            auto n2 = *(g2.nodes_.find(n));     // this is the corresponding node in g2
            // check edges
            for (auto e : n.edges_) {
                // if the corresponding node in the second graph has the same edge, continue checking
                // if it doesn't the graphs are not equal so return false
                auto weight = *(e.second);
                // iterate through the corresponding node in the second graph to find the same edge
                unsigned long count = 0;
                for (auto e2 : n2.edges_) {
                    if (*(e2.second) == weight) break;
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
  friend bool operator!=(const Graph<N, E>& g1, const Graph<N, E>& g2) {
      if (g1 == g2) return false;
      return true;
  }

  friend std::ostream& operator<<(std::ostream &os, const Graph<N, E> &graph) {
    //std::cout << "--- PRINTING OUT GRAPH --- " << std::endl;
    for (auto it = graph.nodes_.cbegin(); it != graph.nodes_.cend(); ++it) {
        os << *(it);
    }

    if (graph.nodes_.size() == 0) {
      os << std::endl; 
      // as per forum question https://webcms3.cse.unsw.edu.au/COMP6771/19T2/forums/2732812
    }
    return os;
  }
  
  std::set<Node> get_nodes() {return nodes_;}
private:
    std::set<Node> nodes_;
};






}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_