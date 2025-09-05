/*
#include <vector>  // For storage
#include <cstdint> // For uint32_t, UINT32_MAX
#include <cassert> // For assert()
#include <functional>
#include <typeindex>
#include <memory>        // For std::shared_ptr, std::make_shared
#include <unordered_map> // For Registry’s component storage

#include "../vaultEngine_lib.h"

namespace ecs
{
    //  -----------------------=== Basic types ===-----------------------

    using Entity = uint32_t; // Entity is uint32_t
    static constexpr Entity INVALID_ENTITY = UINT32_MAX;

    //  -----------------------=== Type ID system ===-----------------------

    using TypeId = std::size_t;

    TypeId nextTypeId = 0;

    template <typename T>
    TypeId type_id()
    {
        static TypeId id = nextTypeId++;
        return id;
    }

    //  -----------------------=== EntityManager ===-----------------------
    //      Creates and destroy entities, recycled destroyed entities

    class EntityManager
    {
    public:
        //? create and return a Entity
        Entity create()
        {
            //? handle create() if freeList has any entity
            if (!freeList.empty())
            {
                Entity idx = freeList.back(); // Get last Entity in freeList
                freeList.pop_back();          // remove last Entity in freelist

                infos[idx].alive = true; // Activate entity
                infos[idx].generation++; // Keep track of generation
                infos[idx].sparseIndex = INVALID_ENTITY;
                return idx; // return entity
            }
            else //? Take the next avilible entity and add to infos
            {
                Entity idx = static_cast<Entity>(infos.size()); // Get avalible idx
                infos.emplace_back();                           // add new element to infos
                infos[idx].alive = true;                        // Activate entity
                return idx;                                     // return entity
            }
        }
        //? Remove a entity
        void destroy(Entity e)
        {
            LOG_ASSERT(valid(e), "Entity: %d is not valid!", e);

            infos[e].alive = false;
            infos[e].sparseIndex = INVALID_ENTITY;
            freeList.push_back(e); // Remove entity
        }
        //? check entity
        bool valid(Entity e) const
        {
            return e < infos.size() && infos[e].alive;
        }
        //? return the size of infos
        Entity capacity() const
        {
            return static_cast<Entity>(infos.size());
        }

    private:
        struct EntityInfo // Hold info of a entity
        {
            Entity generation = 0;               // Tracks times reused (to ensure destroyed entity dont become vaild)
            Entity sparseIndex = INVALID_ENTITY; // maps entity id in dense array (INVALID = not have component)
            bool alive = false;                  // if entity is active?
        };
        std::vector<EntityInfo> infos;
        std::vector<Entity> freeList;
    };

    //  -----------------------=== ComponentStorage ===-----------------------
    //              A sparse set structure that holds all components T

    template <typename T>
    class ComponentStorage
    {
    public:
        ComponentStorage() {}

        void ensure_capacity(Entity maxE)
        {
            if (entityToDense.size() <= maxE)
            {
                entityToDense.resize(maxE + 1, INVALID_ENTITY);
            }
        }

        bool has(Entity e) const
        {
            return e < entityToDense.size() && entityToDense[e] != INVALID_ENTITY;
        }

        T *get(Entity e)
        {
            if (!has(e))
                return nullptr;
            return &data[entityToDense[e]];
        }

        const T *get(Entity e) const
        {
            if (e >= entityToDense.size())
                return nullptr;
            auto d = entityToDense[e];
            if (d == INVALID_ENTITY)
                return nullptr;
            return &data[d];
        }

        void add(Entity e, T comp)
        {
            if (e >= entityToDense.size())
                entityToDense.resize(e + 1, INVALID_ENTITY);
            if (entityToDense[e] != INVALID_ENTITY)
            {
                data[entityToDense[e]] = std::move(comp);
                dirty[entityToDense[e]] = true;
                return;
            }
            Entity denseIndex = static_cast<Entity>(data.size());
            data.push_back(std::move(comp));
            denseToEntity.push_back(e);
            entityToDense[e] = denseIndex;
            dirty.push_back(true);
        }

        void remove(Entity e)
        {
            assert(e < entityToDense.size());
            Entity d = entityToDense[e];
            if (d == INVALID_ENTITY)
                return;
            Entity last = static_cast<Entity>(data.size() - 1);
            if (d != last)
            {
                data[d] = std::move(data[last]);
                Entity movedEntity = denseToEntity[last];
                denseToEntity[d] = movedEntity;
                entityToDense[movedEntity] = d;
                dirty[d] = dirty[last];
            }
            data.pop_back();
            denseToEntity.pop_back();
            dirty.pop_back();
            entityToDense[e] = INVALID_ENTITY;
        }

        Entity size() const { return static_cast<Entity>(data.size()); }

        const std::vector<T> &dense_data() const { return data; }
        const std::vector<Entity> &dense_to_entity() const { return denseToEntity; }

        std::vector<T> data;               // dense array of all components of type T
        std::vector<Entity> entityToDense; // entity id -> data id
        std::vector<Entity> denseToEntity; // inverse entityToDense used for removing
        std::vector<char> dirty;           // Tracks components that have been modified this loop (called dirty)
    };

    //  -----------------------=== CommandBuffer ===-----------------------
    //              Apply add/remove commands at the end of the loop

    enum class CommandType : uint8_t
    {
        AddComponent,
        RemoveComponent,
        DestroyEntity
    };
    // Base structure for commands
    struct CommandBase
    {
        CommandType type;
        Entity e;
        ecs::TypeId compType;
        std::shared_ptr<void> payload;
    };
    
    class CommandBuffer
    {
    public:
        template <typename T>
        void add_component(Entity e, const T component)
        {
            auto p = std::make_shared<T>(std::move(component));
            CommandBase c{CommandType::AddComponent, e, type_id<T>(), p};
            commands.push_back(std::move(c));
        }

        template <typename T>
        void remove_component(Entity e)
        {
            CommandBase c{CommandType::RemoveComponent, e, type_id<T>(), nullptr};
            commands.push_back(std::move(c));
        }

        void destroy_entity(Entity e)
        {
            CommandBase c{CommandType::DestroyEntity, e, std::type_index(typeid(void)), nullptr};
            commands.push_back(std::move(c));
        }

        void clear()
        {
            commands.clear();
        }

        const std::vector<CommandBase> &all() const
        {
            return commands;
        }

        void internal_push(CommandBase c)
        {
            commands.push_back(std::move(c));
        }

    private:
        std::vector<CommandBase> commands;
    };

    //  -----------------------=== Registry ===-----------------------
    //              Works like the central ECS manager
    
    class Registry
    {
    public:
        //? Create new entity
        Entity create_entity()
        {
            return em.create();
        }

        //? Destroy a entity
        void destroy_entity(Entity e)
        {
            CommandBase c{CommandType::DestroyEntity, e, 0, nullptr};
            cmd.internal_push(std::move(c));
        }

        //? Checks if a entity is vaild
        bool valid_entity(Entity e)
        {
            return em.valid(e);
        }

        //? get the componentStorage if it exist othervise create a new one
        template <typename T>
        ComponentStorage<T> &storage()
        {
            auto ti = std::type_index(typeid(T));
            if (storages.find(ti) == storages.end())
            {
                storages[ti] = std::make_shared<AnyStorage<T>>();
            }
            return static_cast<AnyStorage<T> *>(storages[ti].get())->storage;
        }

        Entity capacity()
        {
            return em.capacity();
        }

        template <typename T>
        void add_component(Entity e, T comp)
        {
            auto p = std::make_shared<T>(std::move(comp));
            CommandBase c{CommandType::AddComponent, e, type_id<T>(), p};
            cmd.internal_push(std::move(c));
        }

        template <typename T>
        void remove_component(Entity e)
        {
            CommandBase c{CommandType::RemoveComponent, e, type_id<T>(), nullptr};
            cmd.internal_push(std::move(c));
        }

        template <typename T>
        bool has_component(Entity e)
        {
            CommandBase c{CommandType::RemoveComponent, e, type_id<T>(), nullptr};
            cmd.internal_push(std::move(c));
        }

        template <typename T>
        T *get_component(Entity e)
        {
            auto ti = std::type_index(typeid(T));
            auto it = storages.find(ti);
            if (it == storages.end())
                return nullptr;
            return static_cast<AnyStorage<T> *>(it->second.get())->storage.get(e);
        }

        //? Commit all the commandsbuffers commands
        void commit()
        {
            for (auto &c : cmd.all())
            {
                if (c.type == CommandType::AddComponent)
                {
                    if (c.compType < applyAddHandlers.size() && applyAddHandlers[c.compType])
                    {
                        applyAddHandlers[c.compType](c.e, c.payload);
                    }
                }
                else if (c.type == CommandType::RemoveComponent)
                {
                    if (c.compType < applyRemHandlers.size() && applyRemHandlers[c.compType])
                    {
                        applyRemHandlers[c.compType](c.e);
                    }
                }
                else if (c.type == CommandType::DestroyEntity)
                {
                    for (auto &s : storages)
                        if (s)
                            s->removeEntity(c.e);
                    em.destroy(c.e);
                }
            }
            cmd.clear();
        }
        
        //? register a component type for the ecs, have to register a type before add_component()
        template <typename T>
        void register_type()
        {
            TypeId id = type_id<T>();
            if (id >= applyAddHandlers.size())
            {
                applyAddHandlers.resize(id + 1);
                applyRemHandlers.resize(id + 1);
            }
            if (!applyAddHandlers[id])
            {
                applyAddHandlers[id] = [this](Entity e, std::shared_ptr<void> payload)
                {
                    auto p = std::static_pointer_cast<T>(payload);
                    this->storage<T>().add(e, *p);
                };
                applyRemHandlers[id] = [this](Entity e)
                {
                    this->remove_component<T>(e);
                };
            }
        }
        
        //* view one component in system
        template <typename A, typename Fn>
        void view(Fn &&fn)
        {
            auto &s = storage<A>();
            const auto &data = s.dense_data();
            const auto &map = s.dense_to_entity();
            for (Entity i = 0; i < s.size(); ++i)
            {
                fn(map[i], data[i]);
            }
        }

    private:
        EntityManager em;
        CommandBuffer cmd;

        using TypeId = std::size_t;

        struct IAnyStorage
        {
            virtual ~IAnyStorage() = default;
            virtual void removeEntity(Entity e) = 0;
        };
        template <typename T>
        struct AnyStorage : IAnyStorage
        {
            ComponentStorage<T> storage;
            void removeEntity(Entity e) override { storage.remove(e); }
        };
        std::vector<std::shared_ptr<IAnyStorage>> storages;
        std::vector<std::function<void(Entity, std::shared_ptr<void>)>> applyAddHandlers;
        std::vector<std::function<void(Entity)>> applyRemHandlers;
    };
    //  -----------------------=== Components ===-----------------------
    
    struct TransformHot
    {
        float x, y;
        float sx, sy;
        float angle;
    };
}
    */