//
// Created by tartarus on 11/13/22.
//

#ifndef RAVEN_SYSTEM_MANAGER_H
#define RAVEN_SYSTEM_MANAGER_H

#include <unordered_map>
#include <memory>
#include <stdexcept>

#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace Raven {
    class SystemManager
    {
    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();


            if(mSystems.find(typeName) != mSystems.end()) {
                std::string error = "RAVEN::SYSTEM_MANAGER::REGISTER_SYSTEM\n\t";
                error += "System is being registered second time!";
                throw std::runtime_error(error);
            }


            // Create a pointer to the system and return it so it can be used externally
            auto system = std::make_shared<T>();
            mSystems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();
            if(mSystems.find(typeName) == mSystems.end()) {
                std::string error = "RAVEN::SYSTEM_MANAGER::SET_SIGNATURE\n\t";
                error += "System used before registration!";
                throw std::runtime_error(error);
            }
            mSignatures.insert({typeName, signature});
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const& pair : mSystems)
            {
                auto const& system = pair.second;

                system->mEntities.erase(entity);
            }
        }

        void EntitySignatureChanged(Entity entity, Signature entitySignature)
        {
            for (auto const& pair : mSystems)
            {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = mSignatures[type];

                if ((entitySignature & systemSignature) == systemSignature)
                {
                    system->mEntities.insert(entity);
                }
                else
                {
                    system->mEntities.erase(entity);
                }
            }
        }

    private:
        std::unordered_map<const char*, Signature> mSignatures{};
        std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
    };
}

#endif //RAVEN_SYSTEM_MANAGER_H
