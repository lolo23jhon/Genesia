#include <algorithm>
#include "TraitCollection.h"	
#include "Organism.h"
#include "Engine.h"
#include "RandomGenerator.h"


////////////////////////////////////////////////////////////
TraitCollection::TraitCollection() {}

////////////////////////////////////////////////////////////
void TraitCollection::update(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::Continuously) }; // Update only "continous" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second->getIsActive()) { t.second->update(t_owner, t_elapsed); }
	}
}

////////////////////////////////////////////////////////////
void TraitCollection::onOrganismConstruction(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::OnConstruction) }; // Update only "on construction" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second->getIsActive()) { t.second->update(t_owner, t_elapsed); }
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
			if (trait_it->second->getIsVital()) { return; } // Cannot remove a vital trait
			traitTime_it.second.erase(trait_it);
			return;
		}
	}
}

////////////////////////////////////////////////////////////
void TraitCollection::addTrait(TraitPtr t_trait) {
	auto traitTime_it{ m_traits.find(t_trait->getEffectTime()) };
	if (traitTime_it == m_traits.end()) {
		TraitsMap tmp;
		tmp.emplace(t_trait->getId(), std::move(t_trait));
		m_traits.emplace(std::move(tmp));
		return;
	}
	auto trait_it{ traitTime_it->second.find(t_trait->getId()) };
	if (trait_it == traitTime_it->second.end()) {
		traitTime_it->second.emplace(t_trait->getId(), std::move(t_trait));
		return;
	}
	trait_it->second = std::move(t_trait);
}

////////////////////////////////////////////////////////////
bool TraitCollection::setTraitValue(const TraitId& t_id, const float& t_value) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			if (Trait_Base::isTraitFloat(t_id)) {
				dynamic_cast<Trait_Float&>(*trait_it->second.get()).setValue(t_value);
				return true;
			}
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::getTraitValue(const TraitId& t_id, float& t_out_value)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			if (Trait_Base::isTraitFloat(t_id)) {
				t_out_value = dynamic_cast<const Trait_Float&>(*trait_it->second.get()).getValue();
				return true;
			}
		}
	}
	return false;
}

bool TraitCollection::setTraitColor(const TraitId& t_id, const sf::Color& t_color) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			if (Trait_Base::isTraitColor(t_id)) {
				dynamic_cast<Trait_Color&>(*trait_it->second.get()).setColor(t_color);
				return true;
			}
		}
	}
	return false;
}
bool TraitCollection::getTraitColor(const TraitId& t_id, sf::Color& t_out_color)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			if (Trait_Base::isTraitFloat(t_id)) {
				t_out_color = dynamic_cast<const Trait_Color&>(*trait_it->second.get()).getColor();
				return true;
			}
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool TraitCollection::isTraitActive(const TraitId& t_id, bool& t_out_isActive)const {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			t_out_isActive = trait_it->second->getIsActive();
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
			trait_it->second->setIsActive(true);
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
			trait_it->second->setIsActive(false);
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
			t_out_inheritChance = trait_it->second->getInheritChance();
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
			trait_it->second->setInheritChance(t_inheritChance);
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
			return trait_it->second.get();
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////
Trait_Base* TraitCollection::getTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			return trait_it->second.get();
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////
TraitCollectionPtr TraitCollection::clone() { return std::make_unique<TraitCollection>(*this); }


////////////////////////////////////////////////////////////
TraitCollectionPtr TraitCollection::reproduce(SharedContext& t_context) {
	auto copy{ std::make_unique<TraitCollection>(*this) };
	for (const auto& traitTime_it : m_traits) {
		for (const auto& trait_it : traitTime_it.second) {
			float pctChance{ trait_it.second->getInheritChance() };
			if (pctChance >= 1.f || pctChance > t_context.m_rng->generate(0.f, 1.f)) {
				copy->addTrait(trait_it.second->reproduce(t_context));
			}
		}
	}
}


// ----------------------------------------------------------------------- TRAITS ----------------------------------------------------------------------