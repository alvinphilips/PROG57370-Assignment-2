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
	if (isServer)
	{
		_ServerUpdate();
	}

	if (isClient)
	{
		_ClientUpdate();
	}
}

void NetworkEngine::_ClientUpdate()
{
	if (!initialized)
	{
		return;
	}

	switch (state)
	{
	case DISABLED:
		break;

	case WAITING_TO_CONNECT:
		if (rakInterface->Connect(ipAddress.c_str(), port, NULL, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			std::cerr << "*** Failed to connect to server. Going to try later....." << std::endl;
		}
		else
		{
			state = WAITING_FOR_FIRST_PACKET;
		}
		break;

	case WAITING_FOR_FIRST_PACKET:
		WaitingForFirstPacket();
		break;

	case RUNNING:
		_UpdateClient();
		break;

	default:
		break;
	}
}

void NetworkEngine::_ServerUpdate()
{
	if (!initialized)
	{
		return;
	}

	switch (state)
	{
	case DISABLED:
		break;

	case CREATE_SERVER:
		SetupServer();
		break;

	case RUNNING:
		_UpdateServer();
		break;

	default:
		break;
	}
}

void NetworkEngine::Initialize(bool is_server)
{
	initialized = true;
	LoadSettings();
	rakInterface = RakNet::RakPeerInterface::GetInstance();
	state = CREATE_SERVER;

	// TODO: Clean this bit up
	this->isServer = is_server;
	this->isClient = !is_server;

	if (isClient)
	{
		state = WAITING_TO_CONNECT;
		RakNet::SocketDescriptor sd(0, nullptr);
		rakInterface->Startup(1, &sd, 1);
	}
}

void NetworkEngine::SendPacket(RakNet::BitStream& bit_stream)
{
	if (isClient)
	{
		rakInterface->Send(&bit_stream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, serverGUID, false);
	}

	if (isServer)
	{
		for (const auto& connection : connections)
		{
			rakInterface->Send(&bit_stream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, connection, false);
		}
	}
}

void NetworkEngine::_UpdateClient()
{
	unsigned char packetId;

	RakNet::Packet* packet = rakInterface->Receive();
	while (packet != nullptr)
	{
		RakNet::BitStream bs(packet->data, packet->length, false);
		bs.Read(packetId);

		switch (packetId)
		{
		case ID_DISCONNECTION_NOTIFICATION:
		case ID_CONNECTION_LOST:
			state = NETWORK_ERROR;
			std::cout << "Disconnected from server" << std::endl;
			break;

		case MSG_SCENE_MANAGER:
			SceneManager::Instance().ProcessPacket(bs);
			break;
		}

		rakInterface->DeallocatePacket(packet);
		packet = rakInterface->Receive();
	}
}

void NetworkEngine::_UpdateServer()
{
	// get a packet
	RakNet::Packet* packet = rakInterface->Receive();

	while (packet != NULL)
	{
		RakNet::BitStream bs(packet->data, packet->length, false);

		unsigned char packetId;
		bs.Read(packetId);

		switch (packetId) {

		case ID_NEW_INCOMING_CONNECTION:
			// Somebody connected
			std::cout << "Got connection from " << packet->systemAddress.ToString(true) << std::endl;
			connections.push_back(packet->guid);
			SceneManager::Instance().SerializeSnapshot();
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			std::cout << "Disconnected from " << packet->systemAddress.ToString(true) << std::endl;
			{
				std::vector<RakNet::RakNetGUID>::iterator position = std::find(connections.begin(), connections.end(), packet->guid);
				if (position != connections.end())
				{
					connections.erase(position);
				}
			}
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally terminated
			std::cout << "Connection lost to " << packet->systemAddress.ToString(true) << std::endl;
			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			std::cerr << "Incomplatible protocol version from " << packet->systemAddress.ToString(true) << std::endl;
			break;

		case MSG_SCENE_MANAGER:
			SceneManager::Instance().ProcessPacket(bs);
			break;

		default:
			std::cout << "Oops, received an unhandled packet with id " << (unsigned)packetId << std::endl;
			break;
		}

		rakInterface->DeallocatePacket(packet);

		// get next packet
		packet = rakInterface->Receive();
	}
}

void NetworkEngine::WaitingForFirstPacket()
{
	// wait for the first packet to arrive
	RakNet::Packet* packet = rakInterface->Receive();
	if (packet == nullptr)
	{
		return;
	}

	// get the packet type identifier
	unsigned char packetId = packet->data[0];
	switch (packetId) {
	case ID_CONNECTION_REQUEST_ACCEPTED:
		std::cout << "\nConnected to " << packet->systemAddress.ToString(true) << std::endl;
		state = RUNNING;
		break;

	case ID_CONNECTION_ATTEMPT_FAILED:
		std::cerr << "*** Connection attempt failed" << std::endl;
		state = NETWORK_ERROR;
		break;

	case ID_NO_FREE_INCOMING_CONNECTIONS:
		std::cerr << "*** Server is full" << std::endl;
		state = NETWORK_ERROR;
		break;

	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
		std::cerr << "*** Incompatible protocol version" << std::endl;
		state = NETWORK_ERROR;
		break;

	default:
		std::cerr << "*** Unknown connection response" << std::endl;
		state = NETWORK_ERROR;
		break;
	}

	serverGUID = packet->guid;

	rakInterface->DeallocatePacket(packet);
}

void NetworkEngine::SetupServer()
{
	RakNet::SocketDescriptor sd(port, NULL);
	if (rakInterface->Startup(8, &sd, 1) != RakNet::RAKNET_STARTED)
	{
		std::cerr << "*** Failed to start server on port " << port << std::endl;
		exit(1);
	}

	// set maximum number of incoming connections
	rakInterface->SetMaximumIncomingConnections(8);

	std::cout << "Listening on port " << port << std::endl;
	std::cout << "IP addresses:" << std::endl;
	for (unsigned i = 0; i < rakInterface->GetNumberOfAddresses(); i++)
	{
		std::cout << rakInterface->GetLocalIP(i) << std::endl;
	}

	state = RUNNING;
}