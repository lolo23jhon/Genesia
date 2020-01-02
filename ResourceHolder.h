#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <variant>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


using Resource = std::variant<sf::Texture, sf::SoundBuffer, sf::Font>;
enum class ResourceType;
using Resources = std::unordered_map<ResourceType, std::unordered_map<std::string, std::unique_ptr<Resource>>>;
using ResourceTypeStrings = std::unordered_map<std::string, ResourceType>;

enum class ResourceType {
	INVALID_RESOURCE_TYPE = -1,
	Texture,
	Sound,
	Font
};

class ResourceHolder {

	std::string m_workingDirPath;
	Resources m_resources;
	static const ResourceTypeStrings s_resourceTypeStrings;

	sf::Mutex m_mutex;

public:
	static const std::string& resourceTypeIdToStr(const ResourceType& t_id);
	static ResourceType resourceTypeStrToId(const std::string& t_str);

	ResourceHolder();
	bool loadResources(const std::string& t_cfgFile, const std::string& t_resourceIdentifier = "RESOURCE");
	bool loadResource(const ResourceType& t_type, const std::string& t_resourceName, const std::string& t_fileNameWithPath);
	void releaseResource(const ResourceType& t_type, const std::string& t_resourceName);
	Resource* getResource(const ResourceType& t_type, const std::string& t_resourceName);
	void purgeResources();
};

#endif // !RESOURCE_HOLDER_H
