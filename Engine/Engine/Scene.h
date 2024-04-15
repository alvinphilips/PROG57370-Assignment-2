#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

class Entity;

class Scene final : public Object
{
public:
	bool IsEnabled() const { return isEnabled; }
	void SetEnabled(bool enabled) { isEnabled = enabled; }
	Entity* CreateEntity();
	Entity* FindEntity(std::string entityGuid);
	Entity* FindEntity(STRCODE entityId);
	std::list<Entity*> FindEntityByName(std::string entityName);  // entities can have same name
	std::list<Entity*> FindEntityWithComponent(std::string componentClassName);
	bool RemoveEntity(std::string entityGuid);
	bool RemoveEntity(STRCODE entityId);

	void Serialize(RakNet::BitStream& bitStream) const override;
	void Deserialize(RakNet::BitStream& bitStream) override;
	void SerializeCreateEntity(Entity* entity, RakNet::BitStream& bitStream) const;
	void DeserializeCreateEntity(RakNet::BitStream& bitStream);
	void DeserializeCreateEntityComponent(RakNet::BitStream& bitStream);
	void SerializeSnapshot(RakNet::BitStream& bitStream);
	void DeserializeSnapshot(RakNet::BitStream& bitStream);

protected:
	void Load(json::JSON&);
	void Initialize();
	void Destroy();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void InvokeRPC(RakNet::BitStream& bitStream);

private:
	// Only enabled scenes get updated & rendered
	bool isEnabled = true;

	std::list<Entity*> entitiesToBeAdded;
	std::list<Entity*> entities;
	std::list<Entity*> entitiesToDestroy;

	// Keep a record of all the assets loaded by a scene
	// as they get unloaded with the scene
	std::list<std::string> assetsGUIDs;

	friend class SceneManager;
};

#endif
