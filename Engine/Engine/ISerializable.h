#pragma once

#ifndef _I_SERIALIZABLE_H_
#define _I_SERIALIZABLE_H_

#include <BitStream.h> // RakNet bitstreams

class ISerializable
{
public:
    virtual ~ISerializable() = default;

    virtual void SerializeCreate(RakNet::BitStream& bitStream) const {}
    virtual void DeserializeCreate(RakNet::BitStream& bitStream) {}

    virtual void Serialize(RakNet::BitStream& bitStream) const {}
    virtual void Deserialize(RakNet::BitStream& bitStream) {}
};

#endif