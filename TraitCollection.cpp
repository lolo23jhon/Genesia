#include <algorithm>
#include "TraitCollection.h"	
#include "Organism.h"
#include "Engine.h"
#include "RandomGenerator.h"


// ----------------------------------------------------------------------- TRAITS ----------------------------------------------------------------------

////////////////////////////////////////////////////////////
const float TraitCollection::s_traitsStdDev{ 0.3f };

////////////////////////////////////////////////////////////
TraitCollection::TraitCollection() {}

////////////////////////////////////////////////////////////
void TraitCollection::update(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::Continuously) }; // Update only "continous" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second.m_isActive) { t.second.update(t_owner, t_elapsed); }
	}
}

////////////////////////////////////////////////////////////
void TraitCollection::onOrganismConstruction(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::OnConstruction) }; // Update only "on construction" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second.m_isActive) { t.second.update(t_owner, t_elapsed); }
	}
}



////////////////////////////////////////////////////////////
bool TraitCollection::hasTrait(const TraitId& t_id)const {
	for (const auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) { return true; }
	}
	return false;
}

////////////////////////////////////////////////////////////
void TraitCollection::removeTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.end()) {
			if (trait_it->second.m_isVital) { return; } // Cannot remove a vital trait
			traitTime_it.second.erase(trait_it);
			return;
		}
	}
}

////////////////////////////////////////////////////////////
void TraitCollection::addTrait(const Trait_Base& t_trait) {
	auto traitTime_it{ m_traits.find(t_trait.m_effectTime) };
	if (traitTime_it == m_traits.end()) {
		TraitsMap tmp;
		tmp.emplace(t_trait.m_id, t_trait);
		m_traits.emplace(std::move(tmp));
		return;
	}
	auto trait_it{ traitTime_it->second.find(t_trait.m_id) };
	if (trait_it == traitTime_it->second.end()) {
		traitTime_it->second.emplace(t_trait.m_id, t_trait);
		return;
	}
	trait_it->second = t_trait;
}

////////////////////////////////////////////////////////////
bool TraitCollection::setTraitValue(const TraitId& t_id, const float& t_value) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			trait_it->second.m_value = t_value;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::getTraitValue(const TraitId& t_id, float& t_out_value)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			t_out_value = trait_it->second.m_value;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::isTraitActive(const TraitId& t_id, bool& t_out_isActive)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			t_out_isActive = trait_it->second.m_isActive;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::activateTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			trait_it->second.m_isActive = true;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::deactivateTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			trait_it->second.m_isActive = false;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			t_out_inheritChance = trait_it->second.m_inheritChance;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			trait_it->second.m_inheritChance = t_inheritChance;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
const Trait_Base* TraitCollection::getTrait(const TraitId& t_id)const {
	for (const auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			return &trait_it->second;
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////
Trait_Base* TraitCollection::getTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			return &trait_it->second;
		}
	}
	return nullptr;
}


////////////////////////////////////////////////////////////
TraitsPtr TraitCollection::clone() {
	auto copy{ std::make_unique<TraitCollection>() };
	copy->m_traits = m_traits;
	return std::move(copy);
}

////////////////////////////////////////////////////////////
TraitsPtr TraitCollection::reproduce(SharedContext& t_context) {
	auto offspring{ std::make_unique<TraitCollection>() };
	for (const auto& tefft_it : m_traits) {
		for (const auto& tid_it : tefft_it.second) {
			if (tid_it.second.m_inheritChance == ALWAYS_INHERITED || t_context.m_rng->generate(0.f, 1.f) <= 1.f) { // Checkf for hit of inheritance chance
				auto copy{ tid_it.second.clone() };											 // Make perfect copy of the trait
				float factor{ t_context.m_rng->normalDisttribution(1.f, s_traitsStdDev) };	 // Get a change factor from normal distribution with mean of 1
				if (factor < 0.f) { factor = 0.f; }											 // Normal distribution might result negative
				copy.m_value *= factor;														 // Modify its value with a bell-shaped distribution
				offspring->addTrait(copy);													 // Give the new changed trait to the offspring
			}
		}
	}
	return std::move(offspring);
}

// ----------------------------------------------------------------------- TRAITS ----------------------------------------------------------------------