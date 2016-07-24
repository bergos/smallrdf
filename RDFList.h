#ifndef __RDF_LIST_H__
#define __RDF_LIST_H__

template<class T> struct RDFListNode {
  T item;
  RDFListNode<T>* next;
};

template <typename T> class RDFList {
  public:
	int length;

    RDFList();
    ~RDFList();

    T add(T item);
    T get(int index);

  protected:
    RDFListNode<T>* _root;
    RDFListNode<T>* _last;

    RDFListNode<T>* node(int index);
};

template<typename T> RDFList<T>::RDFList() : length(0), _root(NULL), _last(NULL) {}

template<typename T> RDFList<T>::~RDFList() {
  while (_root) {
    RDFListNode<T>* cur = _root;
    _root = _root->next;
    delete cur;
  }
}

template<typename T> RDFListNode<T>* RDFList<T>::node(int index) {
  int pos = 0;
  RDFListNode<T>* cur = _root;

  while (pos < index && cur) {
    cur = cur->next;
    pos++;
  }

  if (pos == index) {
    return cur;
  }

  return NULL;
}

template<typename T> T RDFList<T>::add(T item) {
  RDFListNode<T>* cur = new RDFListNode<T>();

  cur->item = item;
  cur->next = NULL;

  if (_root) {
    _last-> next = cur;
    _last = cur;
  } else {
    _root = _last = cur;
  }

  length++;

  return item;
}

template<typename T> T RDFList<T>::get(int index) {
  RDFListNode<T>* cur = node(index);

  if (cur) {
    return cur->item;
  } else {
    return NULL;
  }
}

#endif // __RDF_LIST_H__
