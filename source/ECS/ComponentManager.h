//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_COMPONENT_MANAGER_H
#define RAVEN_COMPONENT_MANAGER_H

#include <typeinfo>
#include <unordered_map>
#include <memory>

#include "Entity.h"
#include "Component.h"
#include "IComponentArray.h"
#include "ComponentArray.h"

namespace Raven {

class ComponentManager
{
    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();
            if(mComponentTypes.find(typeName) != mComponentTypes.end()) {
                std::string error = "RAVEN::COMPONENT_MANAGER::REGISTER_COMPONENT\n\t";
                error += "Component type being registered second time!";
                throw std::runtime_error(error);
            }
            mComponentTypes.insert({typeName, mNextComponentType});
            mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
            ++mNextComponentType;
        }

        template<typename T>
        Component GetComponentType()
        {
            const char* typeName = typeid(T).name();
            if(mComponentTypes.find(typeName) == mComponentTypes.end()) {
                std::string error = "RAVEN::COMPONENT_MANAGER::GET_COMPONENT\n\t";
                error += "Component type not registered!";
                throw std::runtime_error(error);
            }
            return mComponentTypes[typeName];
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->InsertData(entity, component);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->RemoveData(entity);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetData(entity);
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const& pair : mComponentArrays)
            {
                auto const& component = pair.second;
                component->EntityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<const char*, Component> mComponentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
        Component mNextComponentType{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            if(mComponentTypes.find(typeName) == mComponentTypes.end()) {
                std::string error = "RAVEN::COMPONENT_MANAGER::GET_COMPONENT_ARRAY\n\t";
                error += "Component type not registered!";
                throw std::runtime_error(error);
            }

            return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
        }
    };
}

#endif //RAVEN_COMPONENT_MANAGER_H
