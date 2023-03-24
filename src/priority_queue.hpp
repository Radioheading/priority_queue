#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#include <iostream>

namespace sjtu {

/**
/**
 * a container like std::priority_queue which is a heap internal.
 * to be specific, a Leftist Heap
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
 public:
  class LeftistNode {
    friend class priority_queue<T>;
   public:
    T value;
    LeftistNode *ls, *rs;

    friend bool operator<(const LeftistNode &obj_1, const LeftistNode &obj_2) {
      return Compare{}(obj_1.value, obj_2.value);
    }

    LeftistNode(T _value = NULL, LeftistNode *_ls = nullptr, LeftistNode *_rs = nullptr) :
        value(_value), ls(_ls), rs(_rs) {}

    LeftistNode(const LeftistNode &other) :
        value(other.value), ls(other.ls), rs(other.rs) {}

    ~LeftistNode() = default;
    /**
   * --------------------supplementary functions-------------------------
   * these operations are involved when it comes to copy constructions
   * or destruction of priority queues, so they're packed into functions
   * to avoid redundant codes
   */

    /**
     * function: ClearNode
     * used to delete a leftist heap recursively (like DFS)
     * by using priority_queue.ClearNode(root), we can destruct it
     * */
    friend void ClearNode(LeftistNode *&now) {
      if (!now) return;
      ClearNode(now->ls), ClearNode(now->rs);
      delete now;
      now = nullptr;
    }

    /**
     * function: Traverse
     * used to output the leftist heap
     * */
    friend void Traverse(LeftistNode *now) {
      if (!now) return;
      std::cout << now->value << ' ';
      if (now->ls) Traverse(now->ls);
      if (now->rs) Traverse(now->rs);
    }

    /**
     * function: MergeNode
     * used to merge two nodes into one recursively
     * for starters, we let the bigger element to be the root
     * then we keep its left_son while merging its right_son with
     * the other node
     * of course, the leftist property ought to be maintained,
     * thus left_son and right_son may be swapped
     * */
    friend LeftistNode *MergeNode(LeftistNode *a, LeftistNode *b) {
      if (!a) return b;
      if (!b) return a;
      if (*a < *b) {
        std::swap(a, b);
      }
      a->rs = MergeNode(a->rs, b);
      std::swap(a->ls, a->rs);
      return a;
    }

    /**
     * function: copy
     * it serves the copy constructor by simply using copy(root, other.root)
     * */
    friend void copy(LeftistNode *&one, const LeftistNode *another) {
      if (!another) return;
      one = new LeftistNode(another->value, nullptr, nullptr);
      copy(one->ls, another->ls), copy(one->rs, another->rs);
    }
    //------------------------------------------------------------------------
  };

 private:
  size_t capacity;
  LeftistNode *root;

 public:
  /**
   * constructors
   */
  priority_queue() : capacity(0), root(nullptr) {}

  priority_queue(LeftistNode *_root, size_t _capacity = 0) : capacity(_capacity) {
    copy(root, _root);
  }

  priority_queue(const priority_queue &other) :
      capacity(other.capacity) {
    copy(root, other.root);
  }
  /**
   * destructor
   */
  ~priority_queue() {
    ClearNode(root);
  }
  /**
   * Assignment operator
   */
  priority_queue &operator=(const priority_queue &other) {
    if (&other == this) return *this;
    ClearNode(root);
    copy(root, other.root);
    capacity = other.capacity;
    return *this;
  }
  /**
   * get the top of the queue.
   * @return a reference of the top element.
   * throw container_is_empty if empty() returns true;
   */
  const T &top() const {
    if (empty()) throw container_is_empty();
    return root->value;
  }
  /**
   * push new element to the priority queue.
   */
  void push(const T &e) {
    LeftistNode *node_pointer = new LeftistNode(e, nullptr, nullptr);
    try {
      root = MergeNode(root, node_pointer);
    } catch (...) {
      delete node_pointer;
      return;
    }
    ++capacity;
  }
  /**
   * delete the top element.
   * throw container_is_empty if empty() returns true;
   */
  void pop() {
    if (empty()) throw container_is_empty();
    int siz = capacity - 1;
    LeftistNode *before = root;
    try {
      root = MergeNode(before->ls, before->rs);
    } catch (...) {
      return;
    }
    capacity = siz;
    delete before;
  }
  /**
   * return the number of the elements.
   */
  size_t size() const {
    return capacity;
  }
  /**
   * check if the container has at least an element.
   * @return true if it is empty, false if it has at least an element.
   */
  bool empty() const {
    return !capacity;
  }
  /**
   * merge two priority_queues with at least O(logn) complexity.
   * clear the other priority_queue.
   */
  void merge(priority_queue &other) {
    try {
      root = MergeNode(root, other.root);
      capacity += other.capacity;
      other.capacity = 0, other.root = nullptr;
    } catch (...) {
      return;
    }
  }

  void traverse() {
    if (empty()) return;
    Traverse(root);
  }
};

}

#endif