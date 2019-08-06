#ifndef GRAPH_TPP
#define GRAPH_TPP

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
  Node new_node{val};
  auto result = nodes_.insert(new_node);
  return result.second;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  auto src_it = nodes_.find(Node{src});
  auto src_node = *(src_it);
  auto dst_node = *(nodes_.find(Node{dst}));
  auto result = src_node.InsertOutgoing(dst_node.val, w);

  // since we can't modify set elements, we need to delete the previous elem
  // and then insert the new node with new edge connections
  nodes_.erase(src_it);
  nodes_.insert(src_node);
  return result;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) noexcept {
  if (!IsNode(val))
    return false;  // if the node doesn't exist, there's nothing to delete

  auto val_it = nodes_.find(Node{val});
  nodes_.erase(val_it);

  // then delete all the other shared_ptrs so ref_count == 0
  // and underlying object deleted ie no memory leak

  auto it = nodes_.begin();
  while (it != nodes_.end()) {

    auto clean_node = *it;
    clean_node.CleanOutgoing(val);
    nodes_.erase(it++);
    auto result = nodes_.insert(clean_node);
    if (result.second == false)
      return false;
  }

  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
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
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  if (oldData == newData)
    return;  // do nothing if old and new are the same

  if (!IsNode(oldData) || !IsNode(newData)) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }

  auto old_node_it = nodes_.find(Node{oldData});
  auto old_node = *old_node_it;  // this is the node being replaced

  auto new_node_it = nodes_.find(Node{newData});
  auto new_node = *new_node_it;  // this is the replacing node

  // iterate through each edge in the old node and replace with edge sourcing from the new node
  for (Edge e : old_node.edges_) {
    InsertEdge(*(new_node.val), *(e.first),
               *(e.second));  // insert a new edge from the replacing node to the previous dst
  }

  // now we need to connect all the incoming edges to the replacing node
  // iterate through each node in the graph and find ones that are outgoing to the old node

  for (auto n : nodes_) {
    // n.ReplaceOutgoing(new_node, old_node);
    for (auto e : n.edges_) {  // iterating through each edge in the node
      if (*(e.first) == oldData) {
        InsertEdge(*(n.val), *(new_node.val), *(e.second));  // insert the new edge
        n.edges_.erase(e);                                   // delete the old edge
      }
    }
  }
  nodes_.erase(old_node_it);  // delete the old node from the graph (along with it's outgoing edges,
                              // but incoming edges still exist)
  return;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
  return nodes_.find(Node{val}) != nodes_.end();
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() noexcept {

  /* will call destructors on all fields and child classes (i believe)
  and since we are using smart pointers, underlying object will be
  clean up automatically*/
  nodes_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {

  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }

  auto src_node = *(nodes_.find(Node{src}));

  return src_node.HasOutgoing(dst);
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> vector_of_nodes;
  for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
    vector_of_nodes.emplace_back((*(it->val)));
  }
  return vector_of_nodes;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  if (!IsNode(src)) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  auto src_node = *(nodes_.find(Node{src}));
  auto connected_set = src_node.GetOutgoing();
  std::vector<N> connected_vector(connected_set.begin(), connected_set.end());
  return connected_vector;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::out_of_range(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  auto src_node = *(nodes_.find(Node{src}));
  return src_node.GetDestWeights(dst);
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {

  if ((src == dst) || !IsNode(src) || !IsNode(dst)) return false;
  if (find(src, dst, w) == end()) return false;

  auto src_it = nodes_.find(Node{src});
  auto src_node = *(src_it);
  auto dst_node = *(nodes_.find(Node{dst}));

  auto result = src_node.EraseOutgoing(dst, w);

  // since we can't modify set elements, we need to delete the previous elem
  //d then insert the new node with new edge connections
  nodes_.erase(src_it);
  nodes_.insert(src_node);

  return result;
}

#endif  // GRAPH_TPP
