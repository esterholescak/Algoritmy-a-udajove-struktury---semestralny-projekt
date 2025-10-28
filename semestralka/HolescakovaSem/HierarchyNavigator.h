#pragma once
#include "NodeData.h"
#include <libds/adt/tree.h>
#include <libds/adt/list.h>
#include <vector>
#include "StopFilter.h"

class HierarchyNavigator
{
public:
    using Tree = ds::adt::MultiwayTree<NodeData>;
    using Node = Tree::Node;

	HierarchyNavigator(Tree& tree); //strom v ktorom sa budem pohybovat

    void printCurrent() const;
    void listChildren() const;
    bool goToChild(size_t index);
    bool goToParent();
    ds::adt::SinglyLinkedList<StopData>* getCurrentStops() const;
    std::vector<StopData> getAllStopsInSubtree() const;

private:
    void collectStopsCount(Node* node, size_t& count) const;
    void collectAllStops(Node* node, std::vector<StopData>& allStopsVector) const;

    Tree& tree_;
    Node* current_;
};