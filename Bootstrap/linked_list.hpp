#pragma once

namespace __std {
  template <class T>
  struct LinkedList {
    T item;
    LinkedList<T>* next = NULL;
    LinkedList<T>* prev = NULL;
    LinkedList() = default;
    LinkedList(T val) : item(val) {}
    LinkedList(T val, LinkedList<T>* other) : prev(other) item(val) {}
    ~LinkedList() { if (next != NULL) delete next; }

    void movef() { this = this->next; }
    void moveb() { this = this->prev; }
    void moves() {
      LinkedList<T>* prev_ = this->prev;
      while (prev_ != NULL) { prev_ = prev_->prev }
      this = prev_;
    }
    void push_back(T new_val) {
      auto* this_ = this;
      while (this_->next != NULL) this_ = this_->next;
      this_->next = new LinkedList<T>(new_val, this); 
    }
    LinkedList<T> operator[](size_t index) {
      LinkedList<T>* output = this;
      if (index < 0) { // traverse backwards
        while (index != 0) {
          output = output->prev;
          ++index;
        }
      } else { // traverse forwards
        while (index != 0) {
          output = output->next;
          --index;
        }
      }
      return *output;
    }
    T operator* () { return item; }
  };
};