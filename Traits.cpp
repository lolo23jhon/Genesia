#include <algorithm>
#include "Traits.h"	
#include "Organism.h"
#include "Engine.h"
#include "RandomGenerator.h"

Traits::Traits() {

}


////////////////////////////////////////////////////////////
Trait::Trait(const TraitId& t_id, const float& t_value, bool t_isActive, const float& t_inheritChance, TraitCallback t_effect) :
	m_id{ t_id }, m_value{ t_value }, m_isActive{ t_isActive }, m_inheritChance{ t_inheritChance }, m_effect{ t_effect }{}

////////////////////////////////////////////////////////////
Trait Trait::clone()const { return { m_id, m_value, m_isActive, m_inheritChance, m_effect }; }

////////////////////////////////////////////////////////////
void Trait::update(Organism* t_owner, const float& t_elapsed) {
	if (!m_isActive) { return; }
	m_effect(this, t_owner, t_elapsed);
}


////////////////////////////////////////////////////////////
const TraitStrings Traits::s_traitStrings{
{"OrganismTrait_MaxEnergy",				TraitId::MaxEnergy},
{"OrganismTrait_DigestiveEfficiency",	TraitId::DigestiveEfficiency},
{"OrganismTrait_RestingMetabolicRate",	TraitId::RestingMetabolicRate},
{"OrganismTrait_MovementSpeed",			TraitId::MovementSpeed},
{"OrganismTrait_TurningSpeed",			TraitId::TurningSpeed},
{"OrganismTrait_FoodDetectionRange",	TraitId::FoodDetectionRange},
{"OrganismTrait_Lifespan",				TraitId::Lifespan}
};


static Trait_MaxEnergy() // HERE implement trait functions

static const bool NON_VITAL_TRAIT{ false };
static const bool VITAL_TRAIT{ true };
static const bool INACTIVE_TRAIT{ false };
static const bool ACTIVE_TRAIT{ true };
static const float ALWAYS_INHERITED{ 1.f };



const TraitsMap Traits::s_defaultTraits{
	// ------------------------------------------------------------- Vital Traits ----------------------------------------------------------------------
	// Key									id										value		isVital				isActive			inheritChance
	{TraitId::MaxEnergy,					Trait(TraitId::MaxEnergy,				100.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED, [](Trait* t, Organism* o, const float& v) if (o->getEnergy() > t->m_value) { o->setEnergy(v); } if ()})},
	{TraitId::DigestiveEfficiency,			Trait(TraitId::DigestiveEfficiency,		0.5f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED)},
	{TraitId::RestingMetabolicRate,			Trait(TraitId::RestingMetabolicRate,	1.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED)},
	{TraitId::MovementSpeed,				Trait(TraitId::MovementSpeed,			20.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED)},
	{TraitId::TurningSpeed,					Trait(TraitId::TurningSpeed,			5.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED)},
	{TraitId::Lifespan,						Trait(TraitId::Lifespan,				100.f,		VITAL_TRAIT,		ACTIVE_TRAIT,		ALWAYS_INHERITED)},

	// ----------------------------------------------------------- Non-vital Traits --------------------------------------------------------------------
	// Key									id									value		isVital				isActive			inheritChance
	{TraitId::FoodDetectionRange,			{TraitId::FoodDetectionRange,		10.f,		NON_VITAL_TRAIT,	ACTIVE_TRAIT,		ALWAYS_INHERITED}}

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
	auto it{ std::find_if(s_traitStrings.cbegin(),s_traitStrings.cend(),
	[&t_id](const std::pair<std::string, TraitId>& t_p) {return t_p.second == t_id; }) };
	return (it == s_traitStrings.cend() ? emptyStr : it->first);
}

////////////////////////////////////////////////////////////
TraitId Traits::traitStrToId(const std::string& t_str) {
	auto it{ s_traitStrings.find(t_str) };
	return (it == s_traitStrings.cend() ? TraitId::INVALID_TRAID_ID : it->second);
}

////////////////////////////////////////////////////////////
const VitalTraits& Traits::getVitalTraits() { return s_vitalTraits; }


////////////////////////////////////////////////////////////
bool Traits::hasTrait(const TraitId& t_id)const {
	return (m_traits.find(t_id) != m_traits.cend());

}

////////////////////////////////////////////////////////////
void Traits::removeTrait(const TraitId& t_id) {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.end()) { return; } // Doesn't have the trait
	if (it->second.m_isVital) { return; } // Cant' remove a vital trait, its needed for lower level functions
	m_traits.erase(it);
}

////////////////////////////////////////////////////////////
void Traits::addTrait(const Trait& t_trait) {
	auto it{ m_traits.find(t_trait.m_id) };
	if (it == m_traits.end()) {
		m_traits.emplace(t_trait.m_id, t_trait);
		return;
	}
	it->second = t_trait;
}

////////////////////////////////////////////////////////////
void Traits::setTraitValue(const TraitId& t_id, const float& t_value) {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.end()) { return; }
	it->second.m_value = t_value;
}

////////////////////////////////////////////////////////////
bool Traits::getTraitValue(const TraitId& t_id, float& t_out_value)const {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.end()) { return false; }
	t_out_value = it->second.m_value;
	return true;
}

////////////////////////////////////////////////////////////
bool Traits::isTraitActive(const TraitId& t_id, bool& t_out_isActive)const {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.cend()) { return false; }
	t_out_isActive = it->second.m_isActive;
	return true;
}

////////////////////////////////////////////////////////////
bool Traits::activateTrait(const TraitId& t_id) {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.cend()) { return false; }
	it->second.m_isActive = true;
	return true;
}

////////////////////////////////////////////////////////////
bool Traits::deactivateTrait(const TraitId& t_id) {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.cend()) { return false; }
	it->second.m_isActive = false;
	return true;
}

////////////////////////////////////////////////////////////
bool Traits::getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.cend()) { return false; }
	t_out_inheritChance = it->second.m_inheritChance;
	return true;
}

////////////////////////////////////////////////////////////
bool Traits::setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance) {
	auto it{ m_traits.find(t_id) };
	if (it == m_traits.cend()) { return false; }
	it->second.m_inheritChance = t_inheritChance;
	return true;
}

////////////////////////////////////////////////////////////
const Trait* Traits::getTrait(const TraitId& t_id)const {
	auto it{ m_traits.find(t_id) };
	return (it == m_traits.cend() ? nullptr : &it->second);
}

////////////////////////////////////////////////////////////
Trait* Traits::getTrait(const TraitId& t_id) {
	auto it{ m_traits.find(t_id) };
	return (it == m_traits.cend() ? nullptr : &it->second);
}


////////////////////////////////////////////////////////////
TraitsPtr Traits::clone() {
	std::make_unique<Traits>
}

////////////////////////////////////////////////////////////
TraitsPtr Traits::reproduce(SharedContext& t_context); // Produces an offspring of the set of traits affected by their chance to be passed