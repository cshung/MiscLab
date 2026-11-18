#include "van_empe_boas.h"
#include <algorithm>
#include <cassert>
#include <stdio.h>

veb::veb() : bits(-1), children(nullptr), summary(nullptr), min(-1), max(-1) {}

veb::~veb() {
  if (this->children) {
    delete[] this->children;
  }
  if (this->summary) {
    delete this->summary;
  }
}

void veb::build(int bits) {
  this->bits = bits;
  if (bits > 1) {
    int children_count = this->children_count();
    this->children = new veb[children_count];
    for (int i = 0; i < children_count; i++) {
      this->children[i].build(this->low_bits());
    }
    this->summary = new veb();
    this->summary->build(this->high_bits());
  }
}

void veb::insert(int v) {
  assert(v >= 0);
  assert(v < 1 << this->bits);
  this->insert_impl(v);
}

void veb::remove(int v) {
  assert(v >= 0);
  assert(v < 1 << this->bits);
  this->remove_impl(v);
}

bool veb::contains(int v) const {
  assert(v >= 0);
  assert(v < 1 << this->bits);
  if (v == this->min || v == this->max) {
    return true;
  }
  if (this->bits == 1) {
    return false;
  }
  return this->children[this->which_child(v)].contains(this->child_index(v));
}

int veb::predecessor(int v) const {
  assert(v >= 0);
  assert(v < 1 << this->bits);
  if (v > this->max) {
    return this->max;
  }
  if (v <= this->min) {
    return -1;
  }
  if (this->bits == 1) {
    return 0;
  }
  int which = this->which_child(v);
  int index = this->child_index(v);
  if ((index > this->children[which].min) &&
      (this->children[which].min != -1)) {
    return this->children[which].predecessor(index) +
           which * this->child_size();
  } else {
    which = this->summary->predecessor(which);
    if (which == -1) {
      return this->min;
    } else {
      return this->children[which].max + which * this->child_size();
    }
  }
}

int veb::successor(int v) const {
  assert(v >= 0);
  assert(v < 1 << this->bits);
  if (v < this->min) {
    return this->min;
  }
  if (v >= this->max) {
    return -1;
  }
  if (this->bits == 1) {
    return 1;
  }
  int which = this->which_child(v);
  int index = this->child_index(v);
  if (index < this->children[which].max) {
    return this->children[which].successor(index) + which * this->child_size();
  } else {
    which = this->summary->successor(which);
    return this->children[which].min + which * this->child_size();
  }
}

int veb::low_bits() const { return this->bits / 2; }

int veb::high_bits() const { return this->bits - this->low_bits(); }

int veb::children_count() const { return 1 << this->high_bits(); }

int veb::child_size() const { return 1 << this->low_bits(); }

int veb::which_child(int v) const { return v / this->child_size(); }

int veb::child_index(int v) const { return v & (this->child_size() - 1); }

bool veb::insert_impl(int v) {
  assert(v != this->min);
  if (this->min == -1) {
    this->min = this->max = v;
    // The min always stay at the root as min only, it never gets down to the
    // children and it create a new non-empty node
    return true;
  }
  if (v > this->max) {
    this->max = v;
  }
  if (v < this->min) {
    std::swap(this->min, v);
  }
  if (this->bits > 1) {
    if (this->children[this->which_child(v)].insert_impl(
            this->child_index(v))) {
      this->summary->insert(this->which_child(v));
    }
  }
  // in this case it does not create a new non-empty node, so the caller is not
  // responsible for updating summary
  return false;
}

bool veb::remove_impl(int v) {
  if ((v == this->min) && (v == this->max)) {
    this->min = this->max = -1;
    return true;
  }
  if (this->bits == 1) {
    if (v == this->min) {
      this->min = this->max;
    } else {
      this->max = this->min;
    }
    return false;
  }

  if (v == this->min) {
    // I could be deleting the min, in which case I need to rebuild the min
    int min_child = this->summary->min;
    int min_value =
        this->children[min_child].min + min_child * this->child_size();
    this->min = min_value;
    v = min_value;
  }

  if (this->children[this->which_child(v)].remove_impl(this->child_index(v))) {
    this->summary->remove(this->which_child(v));
  }

  if (v == this->max) {
    // I could be deleting the max, in which case I need to rebuild the max
    int max_child = this->summary->max;
    if (max_child == -1) {
      this->max = this->min;
    } else {
      int max_value = this->children[max_child].max;
      this->max = max_value + max_child * this->child_size();
    }
  }
  return false;
}

std::vector<int> veb::enumerate() const {
  std::vector<int> result;
  if (this->min == -1) {
    assert(this->max == -1);
  } else {
    result.push_back(this->min);
  }
  if (this->bits > 1) {
    for (int i = 0; i < this->children_count(); i++) {
      auto child_result = this->children[i].enumerate();
      assert(child_result.empty() != this->summary->contains(i));
      for (auto v : child_result) {
        result.push_back(v + i * this->child_size());
      }
    }
  } else if (this->max != this->min) {
    result.push_back(this->max);
  }
  return result;
}
