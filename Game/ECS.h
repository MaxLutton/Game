#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentId = std::size_t;
using Group = std::size_t;

inline ComponentId getNewComponentTypeId() {
	static ComponentId lastId = 0u;
	return lastId++;
}

template <typename T> inline ComponentId getComponentTypeId() noexcept {
	static ComponentId typeId = getNewComponentTypeId();
	return typeId;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32; //render layers, collision layers

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

class Component {
public:
	Entity* entity;
	virtual void init(){}
	virtual void update() {	}
	virtual void draw(){}

	virtual ~Component(){}
};

class Entity {
public:
	Entity(Manager& man) : manager(man) {}
	void update() {
		for (auto &c : components) {
			c->update();
		}
	}
	void draw() {
		for (auto &c : components) {
			c->draw();
		}
	}
	bool isActive() { return active;  }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}
	template<typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeId<T>()];
	}

	template<typename T, typename... Targs> T& addComponent(Targs&&... mArgs) {
		T* c(new T(std::forward<Targs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uptr{ c };
		components.emplace_back(std::move(uptr));

		componentArray[getComponentTypeId<T>()] = c;
		componentBitSet[getComponentTypeId<T>()] = true;

		c->init();
		return *c;

	}

	template<typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeId<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitset groupBitset;
	Manager& manager;

};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void update() {
		for (auto& e : entities) {
			e->update();
		}
	}
	void draw() {
		for (auto& e : entities) {
			e->draw();
		}
	}
	void refresh() {

		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}

	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};