#include "primer/trie_store.h"
#include <memory>
#include <mutex>
#include <optional>
#include <type_traits>
#include "common/exception.h"
#include "primer/trie.h"

namespace bustub {

template <class T>
auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<T>> {
  // Pseudo-code:

  // (1) Take the root lock, get the root, and release the root lock. Don't lookup the value in the
  //     trie while holding the root lock.
  root_lock_.lock();
  Trie trie_snapshot = root_;
  root_lock_.unlock();

  // (2) Lookup the value in the trie.
  const T* value = trie_snapshot.Get<T>(key);

  // (3) If the value is found, return a ValueGuard object that holds a reference to the value and the
  //     root. Otherwise, return std::nullopt.
  if (value){
    return std::optional<ValueGuard<T>>(ValueGuard<T>(trie_snapshot, *value));
  }

  return std::nullopt;
}

template <class T>
void TrieStore::Put(std::string_view key, T value) {
  // You will need to ensure there is only one writer at a time. Think of how you can achieve this.
  // The logic should be somehow similar to `TrieStore::Get`.

  // (1) Take the write lock, only one writer allowed at a time
  write_lock_.lock();

  // (2) Get the root
  Trie trie_snapshot = root_;

  // (3) Create new trie
  Trie new_trie_root = root_.Put(key, std::move(value));
  
  // (4) Get root lock, assign new_trie_root as global trie for all threads, unlock the root lock
  root_lock_.lock();
  root_ = new_trie_root;
  root_lock_.unlock();

  // (5) Unlock the root lock
  write_lock_.unlock();
}

void TrieStore::Remove(std::string_view key) {
  // You will need to ensure there is only one writer at a time. Think of how you can achieve this.
  // The logic should be somehow similar to `TrieStore::Get`.

  // (1) Take the write lock, only one writer allowed at a time
  write_lock_.lock();

  // (2) Get the root
  Trie trie_snapshot = root_;

  // (3) Create new trie
  Trie new_trie_root = root_.Remove(key);
  
  // (4) Get root lock, assign new_trie_root as global trie for all threads, unlock the root lock
  root_lock_.lock();
  root_ = new_trie_root;
  root_lock_.unlock();

  // (5) Unlock the root lock
  write_lock_.unlock();
}

// Below are explicit instantiation of template functions.

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<uint32_t>>;
template void TrieStore::Put(std::string_view key, uint32_t value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<std::string>>;
template void TrieStore::Put(std::string_view key, std::string value);

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<Integer>>;
template void TrieStore::Put(std::string_view key, Integer value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<MoveBlocked>>;
template void TrieStore::Put(std::string_view key, MoveBlocked value);

}  // namespace bustub
