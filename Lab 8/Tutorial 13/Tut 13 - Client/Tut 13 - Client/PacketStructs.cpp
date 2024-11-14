#include "PacketStructs.h"

namespace PS
{
	/// <summary>
	/// This is a chat message contained in a packet.
	/// </summary>
	ChatMessage::ChatMessage(const std::string & msg)
		:m_message(msg)
	{
	}

	/// <summary>
	/// This creates the packet and then assigns its data.
	/// This one contains a chat message.
	/// </summary>
	std::shared_ptr<Packet> ChatMessage::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::ChatMessage);
		p->Append(m_message.size());
		p->Append(m_message);
		return p;
	}

	/// <summary>
	/// This creates the packet and then assigns its data.
	/// This one contains a packet type.
	/// </summary>
	std::shared_ptr<Packet> FileDataBuffer::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::FileTransferByteBuffer);
		p->Append(m_size);
		p->Append(m_databuffer, m_size);
		return p;
	}
}