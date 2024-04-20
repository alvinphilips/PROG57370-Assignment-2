#include "EngineCore.h"
#include "NetworkEngine.h"

#include "SceneManager.h"

void NetworkEngine::LoadSettings()
{
	std::ifstream inputStream("../Assets/NetworkSettings.json");
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON document = json::JSON::Load(str);

	THROW_RUNTIME_ERROR(document.hasKey("ipaddress") == false, "Unable to determine ip address");
	ipAddress = document["ipaddress"].ToString();

	THROW_RUNTIME_ERROR(document.hasKey("port") == false, "Unable to determine ip address");
	port = document["port"].ToInt();
}

void NetworkEngine::PreUpdate()
{
	if (!is_server && !is_client) return;

	switch (state)
	{
	case NetworkState::DISABLED:
		break;
	case NetworkState::INITIALIZE_NETWORK:
		if (rakInterface->IsActive())
		{
			rakInterface->Shutdown(0);
		}
		InitializeNetwork();
		break;
	case NetworkState::RUNNING:
		ReceivePackets();
		break;
	}
}

void NetworkEngine::PostUpdate() const
{
	if (!is_client && !is_server) return;

	SceneManager::Instance().NetworkUpdate();
}

void NetworkEngine::ReceivePackets()
{
	unsigned char packet_id;
	RakNet::Packet* packet = rakInterface->Receive();

	while (packet != nullptr)
	{
		RakNet::BitStream bs(packet->data, packet->length, false);
		bs.Read(packet_id);

		switch (packet_id)
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			if (is_client)
			{
				LOG("\nConnected to " << packet->systemAddress.ToString(true) << "\n");
				connections.push_back(packet->guid);
				state = NetworkState::RUNNING;
			}
			break;

		case MSG_SCENE_MANAGER:
			SceneManager::Instance().ProcessPacket(bs);
			break;

		case ID_NEW_INCOMING_CONNECTION:
		{
			LOG("Got connection from " << packet->systemAddress.ToString(true) << "\n");
			LOG("Hello, " << packet->guid.ToString() << "\n");
			connections.push_back(packet->guid);
			if (is_server)
			{
				unsigned short client_count = (unsigned short)connections.size();

				RakNet::BitStream hello_bs;
				hello_bs.Write<unsigned char>(MSG_HI_NEW_FRIEND);
				hello_bs.Write<unsigned short>(client_count - 1);
				hello_bs.Write(packet->guid.ToString());
				SendPacket(hello_bs);

				RakNet::BitStream new_bs;
				new_bs.Write<unsigned char>(MSG_OMG_NEW_FRIEND_SAY_HI);
				new_bs.Write(client_count);
				SendPacket(new_bs);
			}
			if (connections.size() >= 2)
			{
				SceneManager::Instance().SerializeSnapshot();
			}
			break;
		}

		case MSG_OMG_NEW_FRIEND_SAY_HI:
		{
			bs.Read<unsigned short>(friend_count);
			LOG("We now have " << friend_count << " friends!");
			on_player_joined.Invoke();
			break;
		}

		case MSG_HI_NEW_FRIEND:
		{
			bs.Read<unsigned short>(my_index);
			const auto my_guid = new char[50];
			bs.Read(my_guid);
			LOG("My name is " << my_guid);
			me.FromString(my_guid);
			delete[] my_guid;
			break;
		}

		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
			LOG("Disconnected from " << packet->systemAddress.ToString(true) << "\n");
			connections.erase(std::find(connections.begin(), connections.end(), packet->guid));
			break;

		case ID_CONNECTION_ATTEMPT_FAILED:
			LOG_ERROR("*** Connection attempt failed ***");
			state = NetworkState::INITIALIZE_NETWORK;
			break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			LOG_ERROR("*** Server is full ***");
			state = NetworkState::INITIALIZE_NETWORK;
			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			LOG_ERROR("*** Incompatible protocol version ***");
			state = NetworkState::INITIALIZE_NETWORK;
			break;

		default:
			LOG_ERROR("*** Unknown connection request ***");
			state = NetworkState::INITIALIZE_NETWORK;
			break;
		}

		rakInterface->DeallocatePacket(packet);
		packet = rakInterface->Receive();
	}
}

void NetworkEngine::Initialize(const bool is_server)
{
	initialized = true;

	// TODO: Clean this bit up
	this->is_server = is_server;
	this->is_client = !is_server;

	LoadSettings();

	rakInterface = RakNet::RakPeerInterface::GetInstance();
	state = NetworkState::INITIALIZE_NETWORK;
}

void NetworkEngine::SendPacket(const RakNet::BitStream& bs) const
{
	for (const auto& connection : connections)
	{
		rakInterface->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, connection, false);
	}
}

void NetworkEngine::InitializeNetwork()
{
	RakNet::SocketDescriptor sd(is_server ? (unsigned short)port : 0, nullptr);
	const unsigned int max_connections = is_server ? server_max_connections : 1;

	if (rakInterface->Startup(max_connections, &sd, 1) != RakNet::RAKNET_STARTED)
	{
		LOG_ERROR("*** FAILED TO START NETWORK ***");
		exit(1);
	}

	if (is_client && rakInterface->Connect(ipAddress.c_str(), port, nullptr, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		LOG_ERROR("*** Failed to connect to server. Going to try again later :(");
		return;
	}

	if (is_server)
	{
		rakInterface->SetMaximumIncomingConnections(server_max_connections);
	}

	state = NetworkState::RUNNING;
}