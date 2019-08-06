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

    //  CONSTRUCTORS

    // default constructor
    Graph() : nodes_() {}

    // vector iterator constructor
    Graph(typename std::vector<N>::const_iterator start, typename std::vector<N>::const_iterator end) {
      for (auto it = start; it != end; ++it) {
        InsertNode(*it);
      }
    }

    // tuple iterator constructor
    Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start, typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
      for (auto it = start; it != end; ++it) {
        InsertNode(std::get<0>(*it));
        InsertNode(std::get<1>(*it));
        InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
      }
    }

    // initializer list constructor
    Graph(std::initializer_list<N> init) {
      for (auto it = init.begin(); it != init.end(); ++it){
        InsertNode(*it);
      }
    }

    // copy constructor
    Graph(const Graph<N, E> &g) {

      // FIRST ADD ALL THE NODES
      for (auto it = g.nodes_.begin(); it != g.nodes_.end(); ++it) {
        InsertNode(*(it->val));
      }

      // THEN ADD ALL THE EDGES
      for (auto it = g.begin(); it != g.end(); ++it) {
        // safety check
        if (!IsNode(std::get<0>(*it)) || !IsNode(std::get<1>(*it)) ) continue;

        // insert edge
        InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
      }
    }

    // move constructor
    Graph(Graph<N,E> &&g) : nodes_{std::move(g.nodes_)} {}

    // destructor
    ~Graph() = default;

    /* ---- CUSTOM EDGE COMPARATOR (FOR NODE CLASS) ---- */
    // used to sort edges in the set, based on Node then weight
    struct edge_set_comparator {
      bool operator() (const Edge& a, const Edge &b) const {
        if (*(a.first) == *(b.first)) {
          return *(a.second) < *(b.second);
        }

        return *(a.first) < *(b.first);
      }
    };


    /* ---- NODE CLASS (HELPER CLASS) ---- */

    struct Node {
      std::shared_ptr<N> val;
      std::set<Edge, edge_set_comparator> edges_;

      Node(const N &val) : val(std::make_shared<N>(val)), edges_() {}

      // check if 2 nodes are equal (have the same value)
      friend bool operator==(const Node &a, const Node &b) {
          return *(a.val) ==  *(b.val);
      }

      // check if one node is smaller than the other
      friend bool operator<(const Node &a, const Node &b) {
          return *(a.val) < *(b.val);
      }

      // insert outgoing edge from this node to a dst node with weight E
      bool InsertOutgoing (std::shared_ptr<N> dst, const E &weight) { // pass in shared ptr after finding node in set of nodes
          Edge e = std::make_pair(dst, std::make_shared<E>(weight));
          auto result = edges_.insert(std::move(e));
          return result.second;
      }

        // erase outgoing edge from this node to a dst node with weight E
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

      // replace existing outgoing edge from this node to old_dst with a new outgoing edge to new_dst with weight E
      void ReplaceOutgoing(const N& old_dst, std::shared_ptr<N> new_dst) {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == old_dst) {
            InsertOutgoing(new_dst, *(it->second)); // inserting the exact same edge, just new destination
          }
        }
        CleanOutgoing(old_dst);
      }

      // erase all outgoing edges from this node to src
      void CleanOutgoing(const N &src) {
        auto it = edges_.cbegin();
        while (it != edges_.cend()) {
          if (*(it->first) == src) { // found a dst which is an src
              edges_.erase(it++);
          } else {
              ++it;
          }
        }
      }

      // return true if this node has an outgoing edge to dst
      bool HasOutgoing(const N &dst) const {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst) {
              return true;
          }
        }
        return false;
      }

      // return iterator to an edge from this node to dst with a specified weight. If it doesn't exist, return .end()
      typename std::set<Edge>::const_iterator FindEdge(const N &dst, const E& weight) const {
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst && *(it->second) == weight) {
            return it;
          }
        }
        return edges_.cend();
      }

      // return a set of nodes connected by outgoing edges
      std::set<N> GetOutgoing() const {
        std::set<N> outgoing;
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          outgoing.emplace(*(it->first));
        }
        return outgoing;
      }

      // return a vector of edge weights from this node to dst
      std::vector<E> GetDestWeights(const N &dst) const {
        std::vector<E> weights;
        for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
          if (*(it->first) == dst){
            weights.emplace_back(*(it->second));
          }
        }
        return weights;
      }

      // return output stream which prints node contents and connecting edges
      friend std::ostream& operator<<(std::ostream &os, const Node &node) {
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

    // returns tuple pointer to by iterator
    reference operator*() const { 
      return {*(node_it_->val), *(edge_it_->first), *(edge_it_->second)};
    };

    pointer operator->() const { return &(operator*()); };

    // pre-increment
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
    };

    // post-increment
    const_iterator operator++(int) { auto cpy {*this}; operator++(); return cpy; }; // post is the second

    // pre-decrement
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

    // post-decrement
    const_iterator operator--(int) { auto cpy {*this}; operator--(); return cpy; };

    // checks if two iterators are equal
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

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // returns const custom iterator to the first <N, N, E> tuple in the graph
  const_iterator cbegin() const {
    return const_iterator{nodes_.begin(), (nodes_.begin())->edges_.begin()};
  }

  // returns const custom iterator to the last <N, N, E> tuple in the graph
  const_iterator cend() const {
    return const_iterator{nodes_.end(), (nodes_.end())->edges_.begin()};
  }

  // same as cbegin()
  const_iterator begin() const { return cbegin(); }

  // same as cend()
  const_iterator end() const { return cend(); }

  // Returns a const_reverse_iterator pointing to the last element in the container
  const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }

  // Returns a const_reverse_iterator pointing to the before-the-first element in the container
  const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }

  // same as crbegin()
  const_reverse_iterator rbegin() const { return crbegin(); }

  // same as crend()
  const_reverse_iterator rend() const { return crend(); }

  // Returns an iterator to the found edge in the graph.
  const_iterator find(const N& a, const N& b, const E& weight) const {
    const auto a_node = nodes_.find(Node{a});
    const auto b_node = a_node->FindEdge(b, weight);

    if (a_node == nodes_.end() || b_node == a_node->edges_.end()) {
      return cend();
    }

    return const_iterator(a_node, b_node);
  }

  // removes the position at the location the passed in iterator points to
  const_iterator erase(const_iterator it) {
    if (it == this->end()){
      return it;
    }

    auto [src, dst, w] = *it;
    // increment the iterator to return the next element after deleted element
    ++it;
    auto [after_src, after_dst, after_w] = *it;
    auto result = erase(src, dst, w);

    if (!result) {
      return end();
    }

    return find(after_src, after_dst, after_w);
  }

  // copy assignment
  Graph<N, E>& operator=(const Graph<N, E>& g) {
    Graph g_copy{g};
    std::swap(g_copy, this);
    return this;
  }

  // move assignment
  Graph<N, E>& operator=(
    Graph<N, E>&& g) {
    Graph g_move{g};
    std::swap(g_move, this);
    return this;
  }

  //    METHODS (implementations in .tpp file)

  // adds a new node with value val to the graph.
  bool InsertNode(const N& val);

  // adds a new edge from src to dst with weight w to the graph
  bool InsertEdge(const N& src, const N& dst, const E& w);

  // deletes existing node N from the graph. Does nothing if the node doesn't exist
  bool DeleteNode(const N&);

  // replaces existing node oldData with new node newData. Throws error if oldData doesn't exist
  bool Replace(const N& oldData, const N& newData);

  // All instances of node oldData in the graph are replaced with instances of newData. Throws exception if either node cannot be found in the graph
  void MergeReplace(const N& oldData, const N& newData);

  // removes all nodes and edges from the graph
  void Clear();

  // returns true if a node val exists in the graph
  bool IsNode(const N& val) const;

  // returns true if an edge from src to dst exists in the graph. Throws exception if src or dst not in graph
  bool IsConnected(const N& src, const N& dst) const;

  // returns a vector containing all existing nodes in the graph, sorted by increasing order of nodes
  std::vector<N> GetNodes() const;

  // returns a vector containing all nodes found by immediate outgoing edges from src, sorted by increasing order of nodes. Throws exception if src not in graph
  std::vector<N> GetConnected(const N& src) const;

  // returns vector of edge weights from src to dst, sorted by increasing order of weights. Throws exception if src or dst not in grpah
  std::vector<E> GetWeights(const N& src, const N& dst) const;

  // deletes an edge from src to dst with weight w, only if the edge exists in the graph
  bool erase(const N& src, const N& dst, const E& w);

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

  // prints out the graph in a specific order
  friend std::ostream& operator<<(std::ostream &os, const Graph<N, E> &graph) {
    for (auto it = graph.nodes_.cbegin(); it != graph.nodes_.cend(); ++it) {
        os << *(it);
    }

    if (graph.nodes_.size() == 0) {
      os << std::endl; 
      // as per forum question https://webcms3.cse.unsw.edu.au/COMP6771/19T2/forums/2732812
    }
    return os;
  }

private:
    std::set<Node> nodes_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_