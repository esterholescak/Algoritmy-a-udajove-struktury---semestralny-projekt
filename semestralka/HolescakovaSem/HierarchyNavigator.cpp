#include "HierarchyNavigator.h"
#include "StopData.h"
#include <iostream>
#include <functional>

HierarchyNavigator::HierarchyNavigator(Tree& tree) : tree_(tree), current_(tree_.accessRoot()) {}

void HierarchyNavigator::printCurrent() const {
    if (current_) { //kontrola ci nie je nullptr
        std::cout << "\nCurrent node: " << current_->data_.name << "\n";
        size_t numStops = 0;
        collectStopsCount(current_, numStops);
        std::cout << "(" << numStops << " stops, " << tree_.degree(*current_) << " children)\n";
    }
    else {
        std::cout << "Current node is invalid.\n";
    }
}

void HierarchyNavigator::listChildren() const {
    if (current_) {
        size_t deg = tree_.degree(*current_);
        for (size_t i = 0; i < deg; ++i)
        {
            Node* child = tree_.accessSon(*current_, i);
            if (child) {
                std::cout << "  [" << i << "] " << child->data_.name << "\n";
            }
        }
    }
    else {
        std::cout << "No current node - no children to list.\n";
    }
}

bool HierarchyNavigator::goToChild(size_t index) { //presun current_ na syna/dieta s indexom
    if (current_) {
        if (index < tree_.degree(*current_))
        {
            Node* child = tree_.accessSon(*current_, index);
            if (child) {
                current_ = child;
                return true;
            }
        }
        return false;
    }
    return false;
}

bool HierarchyNavigator::goToParent() { //presun current_ na otca/rodica
    if (current_) {
        Node* parent = tree_.accessParent(*current_);
        if (parent)
        {
            current_ = parent;
            return true;
        }
    }
    return false;
}

ds::adt::SinglyLinkedList<StopData>* HierarchyNavigator::getCurrentStops() const {
	if (current_ && current_->data_.stops) { //kontrola ci current_ nie je nullptr a ci ma priradene zastavky
        return current_->data_.stops;
    }
    return nullptr;
}

std::vector<StopData> HierarchyNavigator::getAllStopsInSubtree() const { //vrati podstrom ako vektor - na filtrovanie
    std::vector<StopData> allStops;
    if (current_) {
        collectAllStops(current_, allStops);
    }
    return allStops;
}

void HierarchyNavigator::collectStopsCount(Node* node, size_t& count) const {
    if (node) {
        if (node->data_.stops)
        {
            count += node->data_.stops->size();
        }
        size_t deg = tree_.degree(*node);
        for (size_t i = 0; i < deg; ++i)
        {
            Node* child = tree_.accessSon(*node, i);
            if (child) {
                collectStopsCount(child, count);
            }
        }
    }
}

void HierarchyNavigator::collectAllStops(Node* node, std::vector<StopData>& allStopsVector) const { //prejde strom a naplni vektor - pre filtrovanie
    if (node) {
        if (node->data_.stops) {
            for (size_t i = 0; i < node->data_.stops->size(); ++i) {
                allStopsVector.push_back(node->data_.stops->access(i));
            }
        }

        size_t deg = tree_.degree(*node);
        for (size_t i = 0; i < deg; ++i) {
            Node* child = tree_.accessSon(*node, i);
            if (child) {
                collectAllStops(child, allStopsVector);
            }
        }
    }
}