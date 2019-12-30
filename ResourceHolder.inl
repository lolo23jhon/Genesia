
////////////////////////////////////////////////////////////
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id, const std::string& t_fileName) {
	auto resource{ std::make_unique<Resource>() };
	if (!resource->loadFromFile(t_fileName)) {
		throw std::runtime_error("ResourceHolder::load - Failed to load " + t_filename);
	}

	insertResource(t_id, std::move(resource));
}


////////////////////////////////////////////////////////////
template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id, const std::string& t_fileName, const Parameter& t_secondParam) {
	auto resource{ std::make_unique<Resource>() };
	if (!resource->loadFromFile(t_fileName, t_secondParam)) {
		throw std::runtime_error("ResourceHolder::load - Failed to load " + t_fileName);
	}
	insertResource(t_id, std::move(resource));
}


////////////////////////////////////////////////////////////
template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id) {
	auto it{m_resources.find(t_id)};
	assert(it != m_resources.end());
	return *it->second;
}

////////////////////////////////////////////////////////////
template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id)const {
	auto it{ m_resources.find(t_id) };
	assert(it != m_resources.cend());
	return *it->second;
}

////////////////////////////////////////////////////////////
template <typename Resource, typenameIdentifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier t_id, std::unique_ptr<Resource> t_resource) {
	auto it{m_resources.insert(std::make_pair(t_id,std::move(t_resource)))};
	assert(inserted.second);
}
