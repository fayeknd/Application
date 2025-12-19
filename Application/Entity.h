#ifndef KTY_OBJ
#define KTY_OBJ

#include "main.h"
#include <bitset>
#include <queue>
#include <array>
#include <unordered_map>
#include <set>
#include "Transform.h"

using EntityID = std::uint32_t;
using ComponentType = std::uint8_t;

const EntityID MaxEntities = 5000;
const ComponentType MaxComponents = 32;

class Entity;
using EntitySignature = std::bitset<MaxComponents>;

class EntityComponent {
public:

	Entity* entity = nullptr;

	bool isActive = true;
	bool hasRun = false;

	/// <summary>Runs once at component initialisation. NOTE: Construct as "void Start() override".</summary>
	virtual void Start() {}
	/// <summary>Runs once every frame. NOTE: Construct as "void Update() override".</summary>
	virtual void Update() {}
	/// <summary>Runs once after the initial update. Use sparingly, usually for rendering (Objects drawn after values set in all Update() functions.) NOTE: Construct as "void RenderFunc() override".</summary>
	virtual void RenderFunc() {}
};
class EntityManager {
	// Queue of unused EntityID IDs
	std::queue<EntityID> mAvailableEntities{};

	// Array of signatures where the index corresponds to the EntityID ID
	std::array<EntitySignature, MaxEntities> mSignatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t mLivingEntityCount{};
	template <class DstType, class SrcType>
	bool IsType(const SrcType* src)
	{
		return dynamic_cast<const DstType*>(src) != nullptr;
	}
public:
	EntityManager() {
		for (EntityID e = 0; e < MaxEntities; e++) {
			mAvailableEntities.push(e);
		}
	}
	EntityID CreateEntity() {
		assert(mLivingEntityCount < MaxEntities && "Max EntityID count reached!");

		EntityID thisId = mAvailableEntities.front();
		mAvailableEntities.pop();
		mLivingEntityCount++;
		return thisId;
	}
	void DestroyEntity(EntityID e) {
		assert(e < MaxEntities && "EntityID out of range.");

		mSignatures[e].reset();
		mAvailableEntities.push(e);
		mLivingEntityCount--;
	}
	void SetEntitySignature(EntityID e, EntitySignature es) {
		assert(e < MaxEntities && "EntityID out of range.");
		mSignatures[e] = es;

	}
	EntitySignature GetSignature(EntityID e)
	{
		assert(e < MaxEntities && "EntityID out of range.");
		return mSignatures[e];
	}
};
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(EntityID e) = 0;
	virtual void UpdateBehaviour() {}
	virtual void UpdatePostBehaviour() {}
};
template <typename T>
class ComponentArray : public IComponentArray {
private:
	// Map from an EntityID ID to an array index.
	std::unordered_map<EntityID, size_t> mEntityToIndexMap;

	// Map from an array index to an EntityID ID.
	std::unordered_map<size_t, EntityID> mIndexToEntityMap;

	// Total size of valid entries in the array.
	size_t mSize;
public:
	std::array<T, MaxEntities> mComponentArray;
	void UpdateBehaviour() override {

		for (int i = 0; i < mSize; i++) {
			if (mComponentArray[i].isActive && mComponentArray[i].hasRun) {
				mComponentArray[i].Update();
			}
		}
	}
	void UpdatePostBehaviour() override {

		for (int i = 0; i < mSize; i++) {
			if (mComponentArray[i].isActive && mComponentArray[i].hasRun) {
				mComponentArray[i].RenderFunc();
			}
		}
	}
	EntityComponent* InsertData(Entity* entity, EntityID e, T component) {
		assert(mEntityToIndexMap.find(e) == mEntityToIndexMap.end() && "Component added to same EntityID more than once.");

		size_t newIndex = mSize;
		mEntityToIndexMap[e] = newIndex;
		mIndexToEntityMap[newIndex] = e;
		mComponentArray[newIndex] = component;

		//mComponentArray[newIndex].parent = entity;
		mSize++;
		return &mComponentArray[newIndex];
	}
	void RemoveData(EntityID e) {
		assert(mEntityToIndexMap.find(e) == mEntityToIndexMap.end() && "Removing non-existent component!");

		size_t mIndexOfRemovedEntity = mEntityToIndexMap[e];
		size_t mIndexOfLastElement = mSize - 1;
		mComponentArray[mIndexOfRemovedEntity] = mComponentArray[mIndexOfLastElement];

		//Update map
		EntityID lastElementEntity = mIndexToEntityMap[mIndexOfLastElement];
		mEntityToIndexMap[lastElementEntity] = mIndexOfRemovedEntity;
		mIndexToEntityMap[mIndexOfRemovedEntity] = lastElementEntity;

		mEntityToIndexMap.erase(e);
		mIndexToEntityMap.erase(mIndexOfLastElement);

		mSize--;
	}
	T& GetData(EntityID e) {
		assert(mEntityToIndexMap.find(e) != mEntityToIndexMap.end() && "Retrieving nonexistent EntityID! " + e);
		return mComponentArray[mEntityToIndexMap[e]];
	}
	void EntityDestroyed(EntityID e) override {
		if (mEntityToIndexMap.find(e) != mEntityToIndexMap.end())
		{
			RemoveData(e);
		}
	}
};
class ComponentManager {
private:
	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> mComponentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType mNextComponentType{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.

	template <typename SrcType, typename T>
	bool IsType(const SrcType* src)
	{
		return (dynamic_cast<T>(src) != NULL);
	}
public:
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end()) {
			RegisterComponent<T>();
		}

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });

		mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		mNextComponentType++;
	}
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}
	template<typename T>
	EntityComponent* AddComponent(Entity* entity, EntityID e, T component)
	{
		return GetComponentArray<T>()->InsertData(entity, e, component);
	}

	template<typename T>
	void RemoveComponent(EntityID e)
	{
		if (typeid(T) == typeid(Transform)) {
			Debug::Warning("Cannot remove transform from component.");
			return;
		}
		GetComponentArray<T>()->RemoveData(e);
	}

	template<typename T>
	T& GetComponent(EntityID e)
	{
		return GetComponentArray<T>()->GetData(e);
	}

	void EntityDestroyed(EntityID e)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(e);
		}
	}
	void Update() {
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->UpdateBehaviour();
		}
	}
	void RenderFunc() {
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->UpdatePostBehaviour();		
		}
	}
};

class EntitySystem {
public:
	std::set<EntityID> mEntities;
};
class EntitySystemManager {
private:
	// Map from system type string pointer to a signature
	std::unordered_map<const char*, EntitySignature> mSignatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<EntitySystem>> mSystems{};
public:
	template <typename T>
	std::shared_ptr<T> RegisterEntitySystem() {
		const char* typeName = typeid(T).name;
		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}
	template <typename T>
	void SetSignature(EntitySignature es) {
		const char* typeName = typeid(T).name;
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
		mSignatures.insert({ typeName, es });
	}
	void EntityDestroyed(EntityID e) {
		for (auto const& pair : mSystems) {
			auto const& system = pair.second;
			system->mEntities.erase(e);
		}
	}
	void EntitySignatureChanged(EntityID e, EntitySignature es) {
		for (auto const& pair : mSystems) {

			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((es & systemSignature) == systemSignature) {
				system->mEntities.insert(e);
			}
			else { system->mEntities.erase(e); }
		}
	}
};
class Object {
private:
	static inline DebugLogger ecsLog;
	static std::unique_ptr<EntityManager> mEntityManager;
	static std::unique_ptr<EntitySystemManager> mSystemManager;
public:
	static std::unique_ptr<ComponentManager> mComponentManager;
	static void Init() {
		ecsLog.defaultColour = DBG_BLUE;
		ecsLog.LoggerName = "ECS";
		ecsLog.CLog("Entity Component System active.");
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<EntitySystemManager>();
	}
	static void Update() {
		mComponentManager->Update();
	}
	static void RenderFunc() {
		mComponentManager->RenderFunc();
	}
	static EntityID CreateEntity(Entity* entity) {
		EntityID e = mEntityManager->CreateEntity();
		return e;
	}
	static void DestroyEntity(EntityID e)
	{
		mEntityManager->DestroyEntity(e);

		mComponentManager->EntityDestroyed(e);

		mSystemManager->EntityDestroyed(e);
	}
	template <typename T>
	static void RegisterComponent() {
		mComponentManager->RegisterComponent<T>();
	}
	template<typename T>
	static EntityComponent* AddComponent(Entity* entity, EntityID e, T component)
	{
		EntityComponent* r = mComponentManager->AddComponent<T>(entity, e, component);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetEntitySignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
		return r;
	}
	template<typename T>
	static void RemoveComponent(EntityID e)
	{
		mComponentManager->RemoveComponent<T>(e);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetEntitySignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}
	template<typename T>
	static T& GetComponent(EntityID e)
	{
		return mComponentManager->GetComponent<T>(e);
	}

	template<typename T>
	static ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}
	// System methods
	template<typename T>
	static std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterEntitySystem<T>();
	}

	template<typename T>
	static void SetSystemSignature(EntitySignature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}
};

class Entity {
private:
	Entity* parent;
	std::vector<Entity*> children;
public:

#define NAMEPREFIX(name, string) "[" + name + "] " + string 

	std::string name = "New Entity";
	EntityID thisEntityID;

	Transform transform;

	Entity* Parent() { return parent; }
	Entity* SetParent(Entity* newParent) {
		if (newParent == nullptr) {
			Debug::Warning(NAMEPREFIX(name, "Cannot set a null parent. Use UnsetParent() to unparent an entity."));
			return this;
		}
		if (newParent == parent) {
			Debug::Warning(NAMEPREFIX(name, "Parent is already set to this parent!"));
			return this;
		}
		if (parent != nullptr)
			UnsetParent();
		parent = newParent;
		newParent->children.push_back(this);
		return newParent; 
	}
	void UnsetParent() {
		if (parent == nullptr) {
			Debug::Warning(NAMEPREFIX(name, "Parent already cleared!"));
		}
		for (int i = 0; i < parent->children.size(); i++) {
			if (parent->children[i]->thisEntityID == thisEntityID) {
				parent->children.erase(parent->children.begin() + i);
				parent = nullptr;
			}
			Debug::Warning(NAMEPREFIX(name, "Self not found in parent child vector"));
			parent = nullptr;
			return; 
		}
	}

	/// <summary>
	/// Tries finding a child given a childEntityID. 
	/// </summary>
	/// <param name="childEntityName"></param>
	/// <returns></returns>
	Entity* FindChild(EntityID childEntityID) {
		for (auto const& entity : children) {
			if (entity->thisEntityID == childEntityID)
				return entity; 
		}
		Debug::Error("Child entity " + std::to_string(childEntityID) + std::string(" was not found. Defaulting to parent."));
		return this;
	}
	/// <summary>
	/// Tries finding a child given a childEntityName.
	/// </summary>
	/// <param name="childEntityName"></param>
	/// <returns></returns>
	Entity* FindChild(std::string childEntityName) {
		for (auto const& entity : children) {
			if (entity->name == childEntityName)
				return entity;
		}
		Debug::Error("Child entity " + childEntityName + " was not found. Defaulting to parent.");
	}
	
	template <typename T>
	T& GetComponent() {
		return Object::GetComponent<T>(thisEntityID);
	}
	template <typename T>
	void AddComponent(T newComponent = T{}) {
		EntityComponent* component = Object::AddComponent(this, thisEntityID, newComponent);
		component->entity = this;
		if (component->isActive && !component->hasRun) {
			component->Start();
			component->hasRun = true;
		}
	}
	template <typename T>
	void RemoveComponent() {
		Object::RemoveComponen < t<T>(thisEntityID);
	}
	template <typename T>
	ComponentType GetComponentType() {
		return Object::GetComponentType<T>();
	}
	static Entity CreateEntity(std::string _name = "New Entity") {
		Entity e;
		e.name = _name;
		e.thisEntityID = Object::CreateEntity(&e);
		return e;
	}
	Entity() {};
	Entity(std::string _name) {
		name = _name;
		thisEntityID = Object::CreateEntity(this);
	}

};
#endif