#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "Vector2.h"


class Component;
class Entity;
class Manager;
class PacMan;

using ComponentID = std::size_t;
using Group = std::size_t;

enum EntityState
{
	walking,
	awaiting,
	dead,
	superpower,
	none
};

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;
using ComponentBitSet = std::bitset<maxComponents>;

using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};


class Entity
{
private:
	Manager& manager;
	bool active = true;
	Vector2 prevPosition;
	std::vector<std::unique_ptr<Component>> components;
	EntityState state = EntityState::none;


	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:

	Entity(Manager& mManager) : manager(mManager) {}


	void update()
	{
		for (auto& component : components)
		{
			component->update();
		}
		
	}

	void setPreviousPos(Vector2 vec)
	{
		prevPosition = vec;
	}

	void draw() {
		for (auto& component : components)
		{
			component->draw();
		}
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }
	Vector2 previousPosition() { return prevPosition; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);

	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	EntityState getState()
	{
		return state;
	}

	void setState(EntityState mState)
	{
		state = mState;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* component(new T(std::forward<TArgs>(mArgs)...));
		component->entity = this;
		std::unique_ptr<Component> uPtr{ component };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = component;
		componentBitSet[getComponentTypeID<T>()] = true;

		component->init();
		return *component;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void update()
	{
		for (auto& entity : entities) entity->update();
	}
	
	void draw()
	{
		for (auto& entity : entities) entity->draw();
	}

	void refresh()
	{
		for (auto i(0u); i<maxGroups; i++)
		{ 
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	PacMan& addPacMan()
	{

	}


};