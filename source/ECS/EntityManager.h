//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_ENTITY_MANAGER_H
#define RAVEN_ENTITY_MANAGER_H

#include <array>
#include <queue>
#include <stdexcept>

#include "Entity.h"
#include "Component.h"

namespace Raven {

    class EntityManager {
        std::queue<Entity> mAvailableEntities{};
        std::array<Signature, ENTITY_MAX> mSignatures{};
        uint32_t mLivingEntityCount{};

    public:
        EntityManager() {
            for(Entity entity = 0; entity < ENTITY_MAX; entity++) {
                mAvailableEntities.push(entity);
            }
        }
        Entity CreateEntity() {
            if(mLivingEntityCount == ENTITY_MAX) {
                std::string error = "RAVEN::ENTITY_MANAGER::CREATE_ENTITY\n\t";
                error += "Reached maximum amount of entities: ";
                error += ENTITY_MAX;
                throw std::runtime_error(error);
            }
            Entity id = mAvailableEntities.front();
            mAvailableEntities.pop();
            mLivingEntityCount--;

            return id;
        }
        void DestroyEntity(Entity entity) {
            if(mLivingEntityCount == 0) {
                std::string error = "RAVEN::ENTITY_MANAGER::DESTROY_ENTITY\n\t";
                error += "Entity out of range: ";
                error += mLivingEntityCount;
                throw std::runtime_error(error);
            }
            mAvailableEntities.push(entity);
            mLivingEntityCount--;
        }
        void SetSignature(Entity entity, Signature signature) {
            if(mLivingEntityCount == 0) {
                std::string error = "RAVEN::ENTITY_MANAGER::DESTROY_ENTITY\n\t";
                error += "Entity out of range: ";
                error += mLivingEntityCount;
                throw std::runtime_error(error);
            }
            mSignatures[entity] = signature;
        }
        Signature GetSignature(Entity entity) {
            if(mLivingEntityCount == 0) {
                std::string error = "RAVEN::ENTITY_MANAGER::DESTROY_ENTITY\n\t";
                error += "Entity out of range: ";
                error += mLivingEntityCount;
                throw std::runtime_error(error);
            }
            return mSignatures[entity];
        }
    };

}

#endif //RAVEN_ENTITY_MANAGER_H
