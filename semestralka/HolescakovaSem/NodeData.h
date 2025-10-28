#pragma once
#include "StopData.h"
#include <libds/adt/list.h>
#include <string>

struct NodeData //uzol stromu
{
    std::string name;
    ds::adt::SinglyLinkedList<StopData>* stops; //zastavky pre ulicu

    NodeData() : stops(nullptr) {} //konstruktor pre uzly bez nazvu, zastavok

	NodeData(const std::string& name) : name(name) //konstruktor pre uzly s nazvom, bez zastavok
    {
        stops = new ds::adt::SinglyLinkedList<StopData>();
    }

	NodeData(const std::string& name, const StopData& stop) : name(name) //konstruktor pre uzly s nazvom a zastavkami - ulice
    {
        stops = new ds::adt::SinglyLinkedList<StopData>();
        stops->insertLast(stop); //-> pre pointer (*stops).
    }

    ~NodeData()
    {
        delete stops;
    }

	NodeData(const NodeData& other) : name(other.name) //kopirovaci konstruktor
    {
        //podmienka ? true : false
        stops = other.stops ? new ds::adt::SinglyLinkedList<StopData>(*other.stops) : nullptr;
    }

    NodeData& operator=(const NodeData& other)
    {
        if (this != &other)
        {
            name = other.name;
            delete stops; //musime najskor zmazat stary zoznam
            stops = other.stops ? new ds::adt::SinglyLinkedList<StopData>(*other.stops) : nullptr;
        }
        return *this;
    }

    bool operator==(const NodeData& other) const
    {
        return name == other.name;
    }
};