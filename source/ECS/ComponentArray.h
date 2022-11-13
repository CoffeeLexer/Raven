//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_COMPONENT_ARRAY_H
#define RAVEN_COMPONENT_ARRAY_H

#include <array>
#include <unordered_map>
#include <string>
#include <stdexcept>

#include "IComponentArray.h"

namespace Raven {
    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void InsertData(Entity entity, T component)
        {
            std::size_t newIndex = mSize;
            mEntityToIndexMap[entity] = newIndex;
            mIndexToEntityMap[newIndex] = entity;
            mComponentArray[newIndex] = component;
            ++mSize;
        }

        void RemoveData(Entity entity)
        {
            if(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end()) {
                std::string error = "RAVEN::COMPONENT_ARRAY::REMOVE\n\t";
                error += "Removing non-existent Component: ";
                error += entity;
                throw std::runtime_error(error);
            }

            std::size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
            std::size_t indexOfLastElement = mSize - 1;
            mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

            Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            mEntityToIndexMap.erase(entity);
            mIndexToEntityMap.erase(indexOfLastElement);

            --mSize;
        }

        T& GetData(Entity entity)
        {
            if(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end()) {
                std::string error = "RAVEN::COMPONENT_ARRAY::GET\n\t";
                error += "Entity not found: ";
                error += entity;
                throw std::runtime_error(error);
            }

            return mComponentArray[mEntityToIndexMap[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
            {
                RemoveData(entity);
            }
        }

    private:
        std::array<T, ENTITY_MAX> mComponentArray;
        std::unordered_map<Entity, std::size_t> mEntityToIndexMap;
        std::unordered_map<std::size_t, Entity> mIndexToEntityMap;
        std::size_t mSize;
    };
}


#endif //RAVEN_COMPONENT_ARRAY_H
