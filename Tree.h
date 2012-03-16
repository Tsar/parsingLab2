#ifndef _TREE_H_
#define _TREE_H_

#include <string>
#include <vector>

class Tree {
public:
    Tree(std::string const& node)
        : node_(node) {
    }
    
    ~Tree() {
        for (size_t i = 0; i < children_.size(); ++i)
            delete children_[i];
    }
    
    void addChild(Tree* child) {
        children_.push_back(child);
    }
private:
    std::string node_;
    std::vector<Tree*> children_;
};

#endif //_TREE_H_
