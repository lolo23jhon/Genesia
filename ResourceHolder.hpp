#ifndef RERSOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cassert>


template <typename Resource, typename Identifier>
class ResourceHolder {

	std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resources;


public:
	void load(Identifier t_id, const std::string& t_fileName);

	template <typename Parameter>
	void load(Identifier t_id, const std::string& t_fileName, const Parameter& t_secondParam);

	Resource& get(Identifier t_id);
	const Resource& get(Identifier t_id)const;

	void insertResource(Identifier t_id, std::unique_ptr<Resource> t_resource);   

};

#include "ResourceHolder.inl"

#endif // !RERSOURCE_HOLDER_H