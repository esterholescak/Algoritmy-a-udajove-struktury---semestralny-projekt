#pragma once
#include "StopData.h" 
#include "NodeData.h"  
#include <libds/adt/tree.h> 

class HierarchyBuilder {
public:
    //alias
    using Tree = ds::adt::MultiwayTree<NodeData>;
    using Node = Tree::Node;

    static Tree* build(const std::vector<StopData>& stops);
};