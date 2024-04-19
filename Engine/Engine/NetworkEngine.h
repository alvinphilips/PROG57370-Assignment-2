#pragma once

#ifndef _NETWORK_ENGINE_H_
#define _NETWORK_ENGINE_H_

#include "NetworkCommon.h"
#include "StandardTypes.h"

class NetworkEngine final
{
	DECLARE_SINGLETON(NetworkEngine)

public:
	enum class NetworkState
	{
		DISABLED,
		INITIALIZE_NETWORK,
		RUNNING,
	};
	bool IsInitialized() const { return initialized; }
	bool IsClient() const { return is_client; }
	bool IsServer() const { return is_server; }
	void SendPacket(const RakNet::BitStream& bs) const;
	unsigned short GetFriendCount() const { return friend_count; }
	unsigned short GetMyIndex() const { return my_index; }
	RakNet::RakNetGUID GetGUID() const { return me; } // Get gud lol
	EventListener on_player_joined;
private:
	void Initialize(bool is_server);
	void PreUpdate();
	void PostUpdate() const;
	void InitializeNetwork();
	void ReceivePackets();
	void LoadSettings();
private:
	int port;
	std::string ipAddress;
	NetworkState state;
	bool is_client = false;
	bool is_server = false;
	RakNet::RakPeerInterface* rakInterface;
	bool initialized = false;
	std::vector<RakNet::RakNetGUID> connections;
	unsigned short friend_count = 0;
	unsigned short my_index = MAXSHORT;
	RakNet::RakNetGUID me;
	const unsigned int server_max_connections = 8;

	friend class Engine;
};

#endif
