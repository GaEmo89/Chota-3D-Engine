#pragma once
#include <vector>
#include <unordered_map>
#include <cassert>
#include <utility>

#include "Entity.h"

// ==============================
// Minimal ECS Registry (fixed for MSVC)
//
// - create entities
// - add/get/has components by type T
// - view entities with component T
//
// Storage is static per component type.
// ==============================

class Registry
{
public:
    Entity Create()
    {
        Entity e{ ++m_NextId };
        m_Alive.push_back(e.id);
        return e;
    }

    template<typename T>
    bool Has(Entity e) const
    {
        const auto& store = Store<T>();
        return store.find(e.id) != store.end();
    }

    template<typename T, typename... Args>
    T& Add(Entity e, Args&&... args)
    {
        auto& store = Store<T>();

        auto it = store.find(e.id);
        if (it != store.end())
            return it->second;

        auto [insIt, ok] = store.emplace(
            e.id,
            T{ std::forward<Args>(args)... }
        );
        (void)ok;
        return insIt->second;
    }

    template<typename T>
    T& Get(Entity e)
    {
        auto& store = Store<T>();
        auto it = store.find(e.id);
        assert(it != store.end() && "Component not found!");
        return it->second;
    }

    template<typename T>
    const T& Get(Entity e) const
    {
        const auto& store = Store<T>();
        auto it = store.find(e.id);
        assert(it != store.end() && "Component not found!");
        return it->second;
    }

    template<typename T>
    std::vector<Entity> View() const
    {
        std::vector<Entity> result;
        const auto& store = Store<T>();
        result.reserve(store.size());
        for (auto& kv : store)
            result.push_back(Entity{ kv.first });
        return result;
    }

private:
    template<typename T>
    static std::unordered_map<uint32_t, T>& Store()
    {
        static std::unordered_map<uint32_t, T> s_Store;
        return s_Store;
    }

private:
    uint32_t m_NextId = 0;
    std::vector<uint32_t> m_Alive;
};
