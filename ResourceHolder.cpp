#include "ResourceHolder.h"
#include "Utilities.h"

////////////////////////////////////////////////////////////
const ResourceTypeStrings ResourceHolder::s_resourceTypeStrings{
{"Texture",	ResourceType::Texture},
{"Sound",	ResourceType::Sound},
{"Font",	ResourceType::Font}
};


////////////////////////////////////////////////////////////
const std::string& ResourceHolder::resourceTypeIdToStr(const ResourceType& t_id) {
	auto it{ std::find_if(s_resourceTypeStrings.cbegin(), s_resourceTypeStrings.cend(),
		[&t_id](const std::pair<std::string , ResourceType>& t_p) {return t_p.second == t_id; }) };
	return it->first;
}

////////////////////////////////////////////////////////////
ResourceType ResourceHolder::resourceTypeStrToId(const std::string& t_str) {
	auto it{ s_resourceTypeStrings.find(t_str) };
	return (it == s_resourceTypeStrings.cend() ? ResourceType::INVALID_RESOURCE_TYPE : it->second);
}

////////////////////////////////////////////////////////////
ResourceHolder::ResourceHolder() : m_workingDirPath{ utilities::getWorkingDirectory() } {}


////////////////////////////////////////////////////////////
bool ResourceHolder::loadResources(const std::string& t_cfgFile, const std::string& t_resourceIdentifier) {
	std::stringstream stream;
	const std::string cfg_fullFileNaname{ m_workingDirPath + t_cfgFile };
	if (!utilities::readFile(cfg_fullFileNaname, stream, true)) {return false; }
	std::string token;
	while (stream >> token) { // Read command idenfitier ("RESOURCE", unless made otherwise)
		if (token == t_resourceIdentifier) {
			stream >> token; // Read the resource type
			auto resourceType{ resourceTypeStrToId(token) }; // string -> enum class member
			if (resourceType == ResourceType::INVALID_RESOURCE_TYPE) { // Error message for unmatching type string and try again
				std::cout << "! WARNING: Invalid resource type \"" << token << "\" in file \"" << t_cfgFile << '\"' << std::endl;
				continue;
			}

			std::string resName;
			std::string fileName;
			stream >> resName >> fileName; // Read resource name and its file 
			// (the filename is already relative to the current working directory)

			std::string res_fullFileName{ m_workingDirPath + fileName };
			if (!loadResource(resourceType, resName, res_fullFileName)) {
				std::string msg{ resourceTypeIdToStr(resourceType) };
				std::cerr << "@ ERROR: Failed to load " << (msg.empty() ? "resource" : msg) << "from file \""<< res_fullFileName <<'\"' << std::endl;
				continue;
			}
		}
		else {
			continue;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////
bool ResourceHolder::loadResource(const ResourceType& t_type, const std::string& t_resourceName, const std::string& t_fileNameWithPath) {

	auto resource{ std::make_unique<Resource>() };
	bool load_result{ false };

	switch (t_type) {
	case ResourceType::Texture:
		*resource.get() = sf::Texture();
		load_result = std::get<sf::Texture>(*resource.get()).loadFromFile(t_fileNameWithPath);
		break;
	case ResourceType::Sound:
		*resource.get() = sf::SoundBuffer();
		load_result = std::get<sf::SoundBuffer>(*resource.get()).loadFromFile(t_fileNameWithPath);
		break;
	case ResourceType::Font:
		*resource.get() = sf::Font();
		load_result = std::get<sf::Font>(*resource.get()).loadFromFile(t_fileNameWithPath);
		break;
	}

	if (!load_result) {
		std::string msg{ resourceTypeIdToStr(t_type) };
		std::cerr << "@ ERROR: Failed to load " << (msg.empty() ? "resource" : msg) << " from file \"" << t_fileNameWithPath << '\"' << std::endl;
		return false;
	}

	auto type_it{ m_resources.find(t_type) };
	if (type_it == m_resources.end()) {

		std::unordered_map<std::string, std::unique_ptr<Resource>> temp_map;
		temp_map.emplace(t_resourceName, std::move(resource));
		m_resources.emplace(t_type, std::move(temp_map));
		return true;
	}

	auto str_it{ type_it->second.find(t_resourceName) };
	if (str_it == type_it->second.cend()) {
		type_it->second.emplace(t_resourceName, std::move(resource));
		return true;
	}

	str_it->second = std::move(resource);
	return true;
}



////////////////////////////////////////////////////////////
void ResourceHolder::releaseResource(const ResourceType& t_type, const std::string& t_resourceName) {
	auto type_it{ m_resources.find(t_type) };
	if (type_it == m_resources.end()) { return; }
	auto str_it{ type_it->second.find(t_resourceName) };
	if (str_it == type_it->second.cend()) { return; }
	type_it->second.erase(str_it);
}

////////////////////////////////////////////////////////////
Resource* ResourceHolder::getResource(const ResourceType& t_type, const std::string& t_resourceName) {
	auto type_it{ m_resources.find(t_type) };
	if (type_it == m_resources.end()) { return nullptr; }
	auto str_it{ type_it->second.find(t_resourceName) };
	if (str_it == type_it->second.cend()) { return nullptr; }
	return str_it->second.get();
}


////////////////////////////////////////////////////////////
void ResourceHolder::purgeResources() { m_resources.clear(); }