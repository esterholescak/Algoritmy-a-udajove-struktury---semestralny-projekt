#include "HierarchyBuilder.h"
#include "StopData.h"
#include "NodeData.h"
#include <libds/adt/tree.h>
#include <libds/adt/list.h>
#include <vector>
#include <string>

HierarchyBuilder::Tree* HierarchyBuilder::build(const std::vector<StopData>& stops) {
    Tree* tree = new Tree();
    Node& root = tree->insertRoot();
    root.data_ = NodeData("GRT");

    for (const auto& stop : stops) {
        Node* municipalityNode = nullptr;
        //uzly obci
        for (size_t i = 0; i < tree->degree(root); ++i) {
            if (tree->accessSon(root, i)->data_.name == stop.municipality) {
                municipalityNode = tree->accessSon(root, i);
                break; //ak uz najdeme spravny uzol, nemusim prechadzat ostatne
            }
        }
		//ak sa nenasiel, vytvorim ho
        if (municipalityNode == nullptr) {
            Node& newMunicipalityNode = tree->emplaceSon(root, tree->degree(root)); //aktualny pocet deti, pridanie na koniec
            newMunicipalityNode.data_ = NodeData(stop.municipality); //priradim meno
            municipalityNode = &newMunicipalityNode;
        }

        Node* streetNode = nullptr;
		//uzly ulic
        for (size_t i = 0; i < tree->degree(*municipalityNode); ++i) {
            if (tree->accessSon(*municipalityNode, i)->data_.name == stop.street) {
                streetNode = tree->accessSon(*municipalityNode, i);
                break;
            }
        }
        if (streetNode == nullptr) {
            Node& newStreetNode = tree->emplaceSon(*municipalityNode, tree->degree(*municipalityNode));
            newStreetNode.data_ = NodeData(stop.street);
            streetNode = &newStreetNode;
        }

		//ak nahodou uzol ulice nema priradene zastavky
        if (streetNode->data_.stops == nullptr) {
            streetNode->data_.stops = new ds::adt::SinglyLinkedList<StopData>();
        }
		streetNode->data_.stops->insertLast(stop); //pridanie zastavky do ulice
    }

	return tree; //vrati strom s hierarchiou zastavok
}