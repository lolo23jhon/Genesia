#include <algorithm>
#include "Traits.h"	
#include "Organism.h"
#include "Engine.h"
#include "RandomGenerator.h"



////////////////////////////////////////////////////////////
Trait::Trait(Traits* t_traitsObj, const TraitId& t_id, const float& t_value, bool t_isActive, const float& t_inheritChance, const TraitEffectTime& t_effectTime, TraitFunctor t_effect) :
	m_id{ t_id }, m_value{ t_value }, m_isActive{ t_isActive }, m_inheritChance{ t_inheritChance }, m_effect{ std::bind(t_effect,t_traitsObj ,std::placeholders::_1, std::placeholders::_2,std::placeholders::_3) }{}

////////////////////////////////////////////////////////////
void Traits::onOrganismConstruction(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::OnConstruction) }; // Update only "on construction" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second.m_isActive) { t.second.update(t_owner, t_elapsed); }
	}
}

////////////////////////////////////////////////////////////
Trait Trait::clone()const { return *this; }

////////////////////////////////////////////////////////////
Traits::Traits() {}

////////////////////////////////////////////////////////////
void Traits::update(Organism* t_owner, const float& t_elapsed) {
	auto it{ m_traits.find(TraitEffectTime::Continuously) }; // Update only "continous" traits
	if (it == m_traits.end()) { return; }
	for (auto& t : it->second) {
		if (t.second.m_isActive) { t.second.update(t_owner, t_elapsed); }
	}
}


// ------------------------------------------------------------- TRAITS' IMPLEMENTATION ------------------------------------------------------------------

////////////////////////////////////////////////////////////
void Traits::Trait_MaxEnergy(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_maxEnergy = t_trait->m_value; }

////////////////////////////////////////////////////////////
void Traits::Trait_DigestiveEfficiency(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_digestiveEfficiency = t_trait->m_value; }

////////////////////////////////////////////////////////////
void Traits::Trait_RestingMetabolicRate(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_restingMetabolicRate = t_trait->m_value; }

////////////////////////////////////////////////////////////
void Traits::Trait_MovementSpeed(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_movementSpeed = t_trait->m_value; }

////////////////////////////////////////////////////////////
void Traits::Trait_TurningSpeed(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_turningSpeed = t_trait->m_value; }

////////////////////////////////////////////////////////////
void Traits::Trait_Lifespan(Trait* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_lifespan = t_trait->m_value; }

// ------------------------------------------------------------- traits' implementation ------------------------------------------------------------------


static const bool NON_VITAL_TRAIT{ false };
static const bool VITAL_TRAIT{ true };
static const bool INACTIVE_TRAIT{ false };
static const bool ACTIVE_TRAIT{ true };
static const float ALWAYS_INHERITED{ 1.f };


static enum { TRAIT_TUPLE_STRING, TRAIT_TUPLE_VALUE, TRAIT_TUPLE_IS_ACTIVE, TRAIT_TUPLE_IS_VITAL, TRAIT_TUPLE_EFFECT_TIME, TRAIT_TUPLE_EFFECT };

////////////////////////////////////////////////////////////
const DefaultTraits Traits::s_defaultTraits{
	// ------------------------------------------------------------------------------------------------ Vital Traits ----------------------------------------------------------------------------------------------------------------
	// Key								string							value		isVital				isActive			inheritChance			effectTime							functor
	{TraitId::MaxEnergy,				{"Trait_MaxEnergy",				100.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_MaxEnergy			}},
	{TraitId::DigestiveEfficiency,		{"Trait_DigestiveEfficiency",	0.5f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_DigestiveEfficiency	}},
	{TraitId::RestingMetabolicRate,		{"Trait_RestingMetabolicRate",	1.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_RestingMetabolicRate }},
	{TraitId::MovementSpeed,			{"Trait_MovementSpeed",			20.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_MovementSpeed		}},
	{TraitId::TurningSpeed,				{"Trait_TurningSpeed",			5.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_TurningSpeed			}},
	{TraitId::Lifespan,					{"Trait_Lifespan",				100.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED,		TraitEffectTime::OnConstruction,	&Traits::Trait_Lifespan				}}

	// ---------------------------------------------------------------------------------------------- Non vital Traits --------------------------------------------------------------------------------------------------------------
	// Key								string							value		isVital				isActive			inheritChance			effectTime							functor


};

////////////////////////////////////////////////////////////
const VitalTraits  Traits::s_vitalTraits{
	TraitId::MaxEnergy,
	TraitId::DigestiveEfficiency,
	TraitId::RestingMetabolicRate,
	TraitId::MovementSpeed,
	TraitId::TurningSpeed,
	TraitId::Lifespan
};

////////////////////////////////////////////////////////////
const std::string& Traits::traitIdToStr(const TraitId& t_id) {
	static const std::string emptyStr{ "" };
	auto it{ s_defaultTraits.find(t_id) };
	return (it != s_defaultTraits.cend() ? std::get<TRAIT_TUPLE_STRING>(it->second) : emptyStr);
}

////////////////////////////////////////////////////////////
TraitId Traits::traitStrToId(const std::string& t_str) {
	auto it{ std::find_if(s_defaultTraits.cbegin(),s_defaultTraits.cend(),
		[&t_str](const std::pair<TraitId, TraitTuple>& t_p) {return std::get<TRAIT_TUPLE_STRING>(t_p.second) == t_str; }) };
	return (it != s_defaultTraits.cend() ? it->first : TraitId::INVALID_TRAID_ID);
}

////////////////////////////////////////////////////////////
const VitalTraits& Traits::getVitalTraits() { return s_vitalTraits; }


////////////////////////////////////////////////////////////
bool Traits::hasTrait(const TraitId& t_id)const {
	for (const auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) { return true; }
	}
	return false;
}

////////////////////////////////////////////////////////////
void Traits::removeTrait(const TraitId& t_id) {
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
void Traits::addTrait(const Trait& t_trait) {
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
bool Traits::setTraitValue(const TraitId& t_id, const float& t_value) {
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
bool Traits::getTraitValue(const TraitId& t_id, float& t_out_value)const {
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
bool Traits::isTraitActive(const TraitId& t_id, bool& t_out_isActive)const {
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
bool Traits::activateTrait(const TraitId& t_id) {
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
bool Traits::deactivateTrait(const TraitId& t_id) {
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
bool Traits::getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const {
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
bool Traits::setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance) {
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
const Trait* Traits::getTrait(const TraitId& t_id)const {
	for (const auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			return &trait_it->second;
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////
Trait* Traits::getTrait(const TraitId& t_id) {
	for (auto& traitTime_it : m_traits) {
		auto trait_it{ traitTime_it.second.find(t_id) };
		if (trait_it != traitTime_it.second.cend()) {
			return &trait_it->second;
		}
	}
	return nullptr;
}


////////////////////////////////////////////////////////////
TraitsPtr Traits::clone() {
	auto copy{ std::make_unique<Traits>() };
	copy->m_traits = m_traits;
	return std::move(copy);
}

////////////////////////////////////////////////////////////
TraitsPtr Traits::reproduce(SharedContext& t_context); // Produces an offspring of the set of traits affected by their chance to be passed


// TODO: Design the inheritance system