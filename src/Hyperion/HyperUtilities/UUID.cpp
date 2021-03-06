#include "Hyperion/HyperUtilities/UUID.h"

#include <iomanip>
#include <sstream>

namespace Hyperion
{
	UUID::UUID(uint64_t mostSignificantBits, uint64_t leastSignificantBits)
		: m_MostSignificantBits{ mostSignificantBits }, m_LeastSignificantBits{ leastSignificantBits }
	{
	}

	UUID::UUID(const std::string& uuid)
		: m_MostSignificantBits{ 0 }, m_LeastSignificantBits{ 0 }
	{
		std::vector<std::string> byteSegments;
		for (size_t i = 0; i < uuid.size(); i += 2)
			byteSegments.push_back(uuid.substr(i, 2));

		std::vector<uint8_t> bytes;
		for (std::string byteSegment : byteSegments)
		{
			uint8_t byte;
			std::stringstream ss;
			ss << std::hex << byteSegment;
			ss >> byte;
			bytes.push_back(byte);
		}

		for (size_t i = 0; i < sizeof(uint64_t); i++)
			m_MostSignificantBits |= ((bytes.at(i) & 0xFF) << static_cast<uint64_t>((sizeof(uint64_t) - i - 1) * 8));

		for (size_t i = 0; i < sizeof(uint64_t); i++)
			m_LeastSignificantBits |= ((bytes.at(i + sizeof(int64_t)) & 0xFF) << static_cast<uint64_t>((sizeof(uint64_t) - i - 1) * 8));
	}

	std::vector<uint8_t> UUID::GetBytes() const
	{
		std::vector<uint8_t> bytes;

		for (size_t i = 0; i < sizeof(uint64_t); i++)
			bytes.push_back(static_cast<uint8_t>((m_MostSignificantBits >> (i * 8)) & 0xFF));

		for (size_t i = 0; i < sizeof(uint64_t); i++)
			bytes.push_back(static_cast<uint8_t>((m_LeastSignificantBits >> (i * 8)) & 0xFF));

		return bytes;
	}

	UUID::operator std::string() const
	{
		std::stringstream ss;
		ss << std::hex;

		std::vector<uint8_t> bytes = GetBytes();
		for (size_t i = 0; i < bytes.size(); i++)
			ss << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(bytes[i]);

		return ss.str();
	}
}
