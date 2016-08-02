/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDFLIST_H_
#define RDFLIST_H_

template<class T> struct RDFListNode {
  T item;
  RDFListNode<T>* next;
};

template<typename T> class RDFList {
 public:
  int length;

  RDFList();
  ~RDFList();

  T add(const T item);
  T get(const int index) const;

 protected:
  RDFListNode<T>* _root;
  RDFListNode<T>* _last;

  RDFListNode<T>* node(const int index) const;
};

template<typename T> RDFList<T>::RDFList()
    : length(0),
      _root(0),
      _last(0) {
}

template<typename T> RDFList<T>::~RDFList() {
  while (_root) {
    RDFListNode<T>* cur = _root;
    _root = _root->next;
    delete cur;
  }
}

template<typename T> RDFListNode<T>* RDFList<T>::node(const int index) const {
  int pos = 0;
  RDFListNode<T>* cur = _root;

  while (pos < index && cur) {
    cur = cur->next;
    pos++;
  }

  if (pos == index) {
    return cur;
  }

  return 0;
}

template<typename T> T RDFList<T>::add(const T item) {
  RDFListNode<T>* cur = new RDFListNode<T>();

  cur->item = item;
  cur->next = 0;

  if (_root) {
    _last->next = cur;
    _last = cur;
  } else {
    _root = _last = cur;
  }

  length++;

  return item;
}

template<typename T> T RDFList<T>::get(const int index) const {
  RDFListNode<T>* cur = node(index);

  if (cur) {
    return cur->item;
  } else {
    return 0;
  }
}

#endif  // RDFLIST_H_
