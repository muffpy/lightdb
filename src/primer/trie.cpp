#include "primer/trie.h"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.

  auto scan = root_;

  if (scan == nullptr) {
    return nullptr;
  }

  for (auto &ch : key) {
    auto children = scan->children_;
    if (auto search = children.find(ch); search != children.end()) {
      scan = search->second;  // obtain child
    } else {
      return nullptr;
    }
  }

  auto node_with_val = dynamic_cast<const TrieNodeWithValue<T> *>(scan.get());

  if (node_with_val == nullptr) {
    return nullptr;
  }

  return node_with_val->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.

  std::shared_ptr<T> value_ptr = std::make_shared<T>(std::move(value));

  if (key.empty()) {
    return Trie(std::make_shared<TrieNodeWithValue<T>>(root_->children_, value_ptr));
  }

  // ptr to root of old trie
  std::shared_ptr<const TrieNode> old_node = root_;
  // one of the two-pointers; this points to the most recently created node of the newly con'd trie;
  // initially, set to root of old trie if it exists or creates a new node
  std::shared_ptr<TrieNode> new_node = old_node ? old_node->Clone() : std::make_shared<TrieNode>();
  // one of the two-pointers; this points to the parent of new_node; initially, same as new_node
  std::shared_ptr<TrieNode> new_node_prev = new_node;
  // ptr to root of new trie
  std::shared_ptr<const TrieNode> new_root = new_node;

  // generate copy-on-write trie
  for (auto it = key.begin(); it != key.end(); it++) {
    if (old_node) {
      old_node = old_node->children_.count(*it) > 0 ? old_node->children_.at(*it) : nullptr;
    }

    if (it == key.end() - 1) {  // value/terminal node
      new_node = old_node ? std::make_shared<TrieNodeWithValue<T>>(old_node->children_, value_ptr)
                          : std::make_shared<TrieNodeWithValue<T>>(value_ptr);
    } else {  // New non-terminal entry in trie
      new_node = old_node ? old_node->Clone() : std::make_shared<TrieNode>();
    }

    new_node_prev->children_[*it] = new_node;  // connect new node to new trie
    new_node_prev = new_node;
  }

  return Trie(new_root);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.

  if (!root_) {
    return {};
  }  // case: trie1 = Trie(); trie1.remove(..)

  if (key.empty()) {                        // case: trie1 = Trie(); trie1.put(...); ...; trie1.remove("")
    return Trie(root_->TrieNode::Clone());  // do not clone value
  }

  std::shared_ptr<const TrieNode> old_node = root_;
  std::shared_ptr<TrieNode> new_node = root_->Clone();
  std::shared_ptr<TrieNode> new_node_prev = new_node;
  std::shared_ptr<const TrieNode> new_root = new_node;
  // If terminal node needs to be removed, then its ancestors might be empty and need to be
  // removed too. We need to make a cut at the cut_node from the cut_child to the terminal node.
  std::shared_ptr<TrieNode> cut_node = new_node->is_value_node_ ? new_node : nullptr;
  char cut_child;

  for (auto it = key.begin(); it != key.end(); it++) {
    old_node = old_node->children_.count(*it) > 0 ? old_node->children_.at(*it) : nullptr;

    if (old_node) {
      if (it == key.end() - 1) {
        if (old_node->children_.empty()) {  // reached terminal node
          if (!cut_node) {                  // cut node is root and root has no value
            return {};
          }
          // std::cout << "Cutting off " << cut_child << " from cut_node: " << cut_node << std::endl;
          cut_node->children_.erase(cut_child);  // cut all descendants below cut_node from cut_child to terminal node
          break;
        }

        new_node = old_node->TrieNode::Clone();  // non-terminal node is cloned without the value
      }

      else {  // normal clone
        new_node = old_node->Clone();
      }

    } else {
      return Trie(root_->Clone());  // key is invalid... clone the same trie
    }

    // keep track of the cut path which consists of cut_node and cut_child
    if (new_node->is_value_node_ || new_node->children_.size() > 1) {
      cut_node = new_node;
      cut_child = *std::next(it);
    }

    new_node_prev->children_[*it] = new_node;  // connect new node to new trie
    new_node_prev = new_node;
  }

  return Trie(new_root);
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
