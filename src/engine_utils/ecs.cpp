#include <cstdint> //for uint32_t
#include <vector>  // For storage
#include <memory>  // For std::shared_ptr, std::make_shared
#include "../vaultEngine_lib.h"

namespace ecs
{
    using Entity = uint32_t; // unsigned 32 bit int that is crossplattform

    //  -----------------------=== EntityManager ===-----------------------
    //      Creates and destroy entities, recycled destroyed entities

    class EntityManager
    {
    public:
        //? create and return a Entity
        Entity create()
        {
            Entity e;
            if (!freeList.empty())
            {
                e = freeList.back();
                freeList.pop_back();
                alive[e] = true;
            }
            else
            {
                e = (Entity)alive.size();
                alive.push_back(true);
            }
            ++aliveCount;
            return e;
        }
        //? Remove a entity
        void destroy(Entity e)
        {
            if (!is_alive(e))
            {
                LOG_WARN("Entity #[%d] is not alive!", e);
                return;
            }
            alive[e] = false;
            freeList.push_back(e);
            --aliveCount;
        }
        //? check entity
        bool is_alive(Entity e) const
        {
            return e < alive.size() && alive[e];
        }
        //? return the size of infos
        Entity capacity() const
        {
            return static_cast<Entity>(alive.size());
        }
        //? get number of alive entities
        Entity count_alive() const
        {
            return static_cast<Entity>(aliveCount);
        }

    private:
        std::vector<bool> alive;
        std::vector<Entity> freeList;
        size_t aliveCount = 0;
    };

    //  -----------------------=== ComponentStorage ===-----------------------
    //              A sparse set structure that holds all components of type T

    template <typename T>
    class ComponentStorage
    {
    public:
        std::vector<T> dense;         // all actual component
        std::vector<Entity> entities; // entities that owns components in dense
        std::vector<Entity> sparse;   // maps entity to dense

        //* Add component to a entity
        void add(Entity e, const T &comp)
        {
            if (e >= sparse.size()) // grow sparse
            {
                sparse.resize(e + 1, -1);
            }
            if (has(e)) // if entity already has component T
            {
                return;
            }
            sparse[e] = (Entity)dense.size();
            dense.push_back(comp);
            entities.push_back(e);
        }

        //* Remove component from a entity
        void remove(Entity e)
        {
            if (!has(e))
            {
                LOG_WARN("Entity #[%d] doesn't have component", e);
                return;
            }
            Entity idx = sparse[e];
            Entity last = (Entity)dense.size() - 1;

            // move last element into removed slot
            dense[idx] = dense[last];
            Entity movedEnt = denseEnt[last];
            denseEnt[idx] = movedEnt;
            sparse[movedEnt] = idx;

            // remove component from sparse set
            dense.pop_back();
            entities.pop_back();
            sparse[e] = -1;
        }

        //* Checks if entity has component
        bool has(Entity e) const
        {
            return e < sparse.size() && sparse[e] != -1;
        }

        //* Get component reference
        T &get(Entity e)
        {
            return dense[sparse[e]];
        }

        //* Get all entities with this component
        std::vector<Entity> &view()
        {
            return entities;
        }
    };

    // Components

    struct TransformHot
    {
        float x{}, y{};
    };
    struct Velocity
    {
        float dx{}, dy{};
    };

    struct Script
    {
        char *path;
    };

    // systems
    struct ISystem
    {
        virtual ~ISystem() = default;
        virtual void update(float dt) = 0;
    };

    struct MovementSystem : public ISystem
    {
        ComponentStorage<TransformHot> &pos;
        ComponentStorage<Velocity> &vel;

        MovementSystem(ComponentStorage<TransformHot> &p, ComponentStorage<Velocity> &v)
            : pos(p), vel(v) {}

        void update(float dt) override
        {
            for (Entity e : vel.view())
            {
                if (!pos.has(e))
                    continue;
                auto &p = pos.get(e);
                auto &v = vel.get(e);
                p.x += v.dx * dt;
                p.y += v.dy * dt;
            }
        }
    };

    struct ScriptSystem : public ISystem
    {
        ComponentStorage<Script> &scripts;

        ScriptSystem(ComponentStorage<Script> &s) : scripts(s) {}

        void update(float dt) override
        {
            for (Entity e : scripts.view())
            {
                auto &s = scripts.get(e);
                LOG_DEBUG("UPDATE script");
            }
        }
    };

    //  -----------------------=== World ===-----------------------
    //              Works like the central ECS manager

    class World
    {
    public:
        // -----------------------=== Entity ===-----------------------
        //? Create new entity
        Entity create_entity()
        {
            return em.create();
        }

        //? Destroy a entity
        void destroy_entity(Entity e)
        {
            em.destroy(e);
        }

        //? Checks if a entity is vaild
        bool is_alive(Entity e)
        {
            return em.is_alive(e);
        }

        //? return the size of infos
        Entity capacity() const
        {
            return em.capacity();
        }

        //? get number of alive entities
        Entity count_alive() const
        {
            return em.count_alive();
        }

        // -----------------------=== Component ===-----------------------
        // std::vector<ComponentStorage> componentStorages;

        // component storages
        ComponentStorage<TransformHot> transforms;
        ComponentStorage<Velocity> velocities;
        ComponentStorage<Script> scripts;

        // -----------------------=== System ===-----------------------

        std::vector<std::unique_ptr<ISystem>> systems;

        template <typename T, typename... Args>
        T *add_system(Args &&...args)
        {
            systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            return static_cast<T *>(systems.back().get());
        }

        void update_systems(float dt)
        {
            for (auto &sys : systems)
            {
                sys->update(dt);
            }
        }

        //? log all entities and their components
        void log_entities()
        {
            LOG_CUSTOM("\nWorld Entity Dump", textColorGreen, "");

            ecs::Entity entitySize = count_alive();
            ecs::Entity cap = capacity();
            LOG_CUSTOM("Number of alive entities:", textColorYellow,"% d \n Current capacity: % d", entitySize, cap);

            for (Entity e = 0; e < capacity(); ++e)
            {
                if (!is_alive(e))
                {
                    continue;
                }
                LOG_CUSTOM("", textColorOrange, "Entity %d has:", e);

                bool first = true;
            }
        }

    private:
        EntityManager em;
    };
}