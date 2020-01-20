#include <cassert>
#include "Trait.h"
#include "Organism.h"
#include "SharedContext.h"
#include "RandomGenerator.h"


static TraitCallback bind(TraitFunctor t_functor) { return std::bind(t_functor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3); }

// Used some macros to cut down the space needed for the table initializer
#define TB Trait_Base						// T.B	  = Trait Base
#define MT std::make_tuple					// M.T.	  = Make Tuple
#define TID TraitId							// T.I.D  = Trait ID
#define	MF std::make_unique<Trait_Float>	// M.K.   = Make Float
#define MC std::make_unique<Trait_Color>	// M.C.   = Make Color
#define TEF TraitEffectTime					// T.E.T. = Trait Effect Time
#define SFC sf::Color						// S.F.C. = Simple Fast Color (SFML)

static const bool NON_VITAL_TRAIT{ false };
static const bool VITAL_TRAIT{ true };
static const bool INACTIVE_TRAIT{ false };
static const bool ACTIVE_TRAIT{ true };
static const float ALWAYS_INHERITED{ 1.f };

// Enum identifiers for std::get
enum { STRING, CALLBACK, EFFECT_TIME };
// ------------------------------------------------------- TRAIT BASE ------------------------------------------------------- 

////////////////////////////////////////////////////////////
const std::string& Trait_Base::getTraitName(const TraitId& t_id) {
	static const std::string emptyStr{ "" };
	auto it{ s_traitNamesAndCallbacks.find(t_id) };
	return (it != s_traitNamesAndCallbacks.cend() ? std::get<STRING>(it->second) : emptyStr);
}

////////////////////////////////////////////////////////////
TraitCallback Trait_Base::getTraitCallback(const TraitId& t_id) {
	assert(t_id != TraitId::INVALID_TRAIT_ID && "Trait_Base::getTraitCallback(const TraitId&): INVALID_TRAIT_ID is not a trait id!");
	return std::get<CALLBACK>(s_traitNamesAndCallbacks.at(t_id));
}

////////////////////////////////////////////////////////////
const TraitEffectTime& Trait_Base::getTraitEffectTime(const TraitId& t_id) {
	assert(t_id != TraitId::INVALID_TRAIT_ID && "Trait_Base::getTraitEffectTime(const TraitId&): INVALID_TRAIT_ID is not a trait id!");
	return std::get<EFFECT_TIME>(s_traitNamesAndCallbacks.at(t_id));
}


////////////////////////////////////////////////////////////
bool Trait_Base::isTraitVital(const TraitId& t_id) { return s_vitalTraits.find(t_id) != s_vitalTraits.cend(); }

////////////////////////////////////////////////////////////
const TraitSet& Trait_Base::getVitalTraits() { return s_vitalTraits; }

////////////////////////////////////////////////////////////
bool Trait_Base::isTraitFloat(const TraitId& t_id) { return s_floatTraits.find(t_id) != s_floatTraits.cend(); }

////////////////////////////////////////////////////////////
bool Trait_Base::isTraitColor(const TraitId& t_id) { return s_colorTraits.find(t_id) != s_colorTraits.cend(); }

////////////////////////////////////////////////////////////
const float& Trait_Base::getTraitStdDev() { return s_traitsStdDev; }

////////////////////////////////////////////////////////////
Trait_Base::Trait_Base(const TraitId& t_id, bool t_isActive, const float& t_inheritChance) :
	m_id{ t_id },
	m_isActive{ t_isActive },
	m_inheritChance{ t_inheritChance },
	m_effectTime{ getTraitEffectTime(t_id) },
	m_effect{ getTraitCallback(t_id) }{}

////////////////////////////////////////////////////////////
void Trait_Base::update(Organism* t_owner, const float& t_elapsed) {
	m_effect(this, t_owner, t_elapsed);
}

////////////////////////////////////////////////////////////
TraitPtr Trait_Base::cloneDefaultTrait(const TraitId& t_id) {
	auto it{ s_defaultTraits.find(t_id) };
	return (it != s_defaultTraits.cend() ? std::move(it->second->clone()) : nullptr);
}

////////////////////////////////////////////////////////////
TraitPtr Trait_Base::reproduceDefaultTrait(SharedContext& t_context, const TraitId& t_id) {
	auto it{ s_defaultTraits.find(t_id) };
	return (it != s_defaultTraits.cend() ? std::move(it->second->reproduce(t_context)) : nullptr);
}

////////////////////////////////////////////////////////////
const TraitId& Trait_Base::getId()const { return m_id; }

////////////////////////////////////////////////////////////
bool Trait_Base::getIsActive()const { return m_isActive; }

////////////////////////////////////////////////////////////
void Trait_Base::setIsActive(bool t_isActive) { m_isActive = t_isActive; }

////////////////////////////////////////////////////////////
bool Trait_Base::getIsVital()const { return s_vitalTraits.find(m_id) != s_vitalTraits.cend(); }

////////////////////////////////////////////////////////////
const float& Trait_Base::getInheritChance()const { return m_inheritChance; }

////////////////////////////////////////////////////////////
void Trait_Base::setInheritChance(const float& t_inheritChance) { m_inheritChance = t_inheritChance; }

////////////////////////////////////////////////////////////
const TraitEffectTime& Trait_Base::getEffectTime()const { return getTraitEffectTime(m_id); }

// ---------- STATIC MEMBERS ---------- 
////////////////////////////////////////////////////////////
const float Trait_Base::s_minTraitFactor{ 0.2f };

////////////////////////////////////////////////////////////
const float Trait_Base::s_traitsStdDev{ 0.3f };

////////////////////////////////////////////////////////////
const TraitSet  Trait_Base::s_vitalTraits{
	TraitId::MaxEnergy,
	TraitId::DigestiveEfficiency,
	TraitId::RestingMetabolicRate,
	TraitId::MovementSpeed,
	TraitId::TurningSpeed,
	TraitId::Lifespan,
	TraitId::Color,
	TraitId::Size,
};

////////////////////////////////////////////////////////////
const TraitSet Trait_Base::s_floatTraits{
	TraitId::MaxEnergy,
	TraitId::DigestiveEfficiency,
	TraitId::RestingMetabolicRate,
	TraitId::MovementSpeed,
	TraitId::TurningSpeed,
	TraitId::FoodDetectionRange,
	TraitId::Lifespan,
	TraitId::Size
};

////////////////////////////////////////////////////////////
const TraitSet Trait_Base::s_colorTraits{
	TraitId::Color
};

////////////////////////////////////////////////////////////
const TraitTable Trait_Base::s_traitNamesAndCallbacks{
	//	key							string							callback								tratiEffectTime
		{TID::MaxEnergy,			{"Trait_MaxEnergy",				bind(TB::TraitFn_MaxEnergy),			TEF::OnConstruction}},
		{TID::DigestiveEfficiency,	{"Trait_DigestiveEfficiency",	bind(TB::TraitFn_DigestiveEfficiency),	TEF::OnConstruction}},
		{TID::RestingMetabolicRate,	{"Trait_RestingMetabolicRate",	bind(TB::TraitFn_RestingMetabolicRate),	TEF::OnConstruction}},
		{TID::MovementSpeed,		{"Trait_MovementSpeed",			bind(TB::TraitFn_MovementSpeed),		TEF::OnConstruction}},
		{TID::TurningSpeed,			{"Trait_TurningSpeed",			bind(TB::TraitFn_TurningSpeed),			TEF::OnConstruction}},
		{TID::Lifespan,				{"Trait_Lifespan",				bind(TB::TraitFn_Lifespan),				TEF::OnConstruction}},
		{TID::Size,					{"Trait_Size",					bind(TB::TraitFn_Size),					TEF::OnConstruction}},
		{TID::Color,				{"Trait_Color",					bind(TB::TraitFn_Color),				TEF::OnConstruction}}
};

////////////////////////////////////////////////////////////

static const sf::Color S_GREEN{ 0,255,0,255 };

////////////////////////////////////////////////////////////
const TraitMap Trait_Base::s_defaultTraits{ []() {
	TraitMap tmp;
	//				 key						id							isActive	 inheritChance		value
	tmp.emplace(TID::MaxEnergy,				MF(TID::MaxEnergy,				ACTIVE_TRAIT, ALWAYS_INHERITED, 500.f));
	tmp.emplace(TID::DigestiveEfficiency,	MF(TID::DigestiveEfficiency,	ACTIVE_TRAIT, ALWAYS_INHERITED, 0.5f));
	tmp.emplace(TID::RestingMetabolicRate,	MF(TID::RestingMetabolicRate,	ACTIVE_TRAIT, ALWAYS_INHERITED, 1.f));
	tmp.emplace(TID::MovementSpeed,			MF(TID::MovementSpeed,			ACTIVE_TRAIT, ALWAYS_INHERITED, 20.f));
	tmp.emplace(TID::TurningSpeed,			MF(TID::TurningSpeed,			ACTIVE_TRAIT, ALWAYS_INHERITED, 20.f));
	tmp.emplace(TID::Lifespan,				MF(TID::Lifespan,				ACTIVE_TRAIT, ALWAYS_INHERITED, 60.f));
	tmp.emplace(TID::Size,					MF(TID::Size,					ACTIVE_TRAIT, ALWAYS_INHERITED, 1.f));
	tmp.emplace(TID::Color,					MC(TID::Color,					ACTIVE_TRAIT, ALWAYS_INHERITED, S_GREEN));
	return std::move(tmp);
}() }; // Initialized with a lambda because of unique_ptr


// ------------------------------------------------------- trait base --------------------------------------------------------
// ------------------------------------------------------- TRAIT FLOAT ------------------------------------------------------- 

////////////////////////////////////////////////////////////
Trait_Float::Trait_Float(const TraitId& t_id, bool t_isActive, const float& t_inheritChance, const float& t_value) :
	Trait_Base(t_id, t_isActive, t_inheritChance), m_value{ t_value }
{}

////////////////////////////////////////////////////////////
TraitPtr Trait_Float::clone() const { return std::make_unique<Trait_Float>(m_id, m_isActive, m_inheritChance, m_value); }

////////////////////////////////////////////////////////////
TraitPtr Trait_Float::reproduce(SharedContext& t_context)const {
	float factor{ t_context.m_rng->normalDisttribution(1.f,s_traitsStdDev) };
	if (factor < s_minTraitFactor) { factor = s_minTraitFactor; }
	return std::make_unique<Trait_Float>(m_id, m_isActive, m_inheritChance, m_value * factor);
}
////////////////////////////////////////////////////////////
const float& Trait_Float::getValue() const { return m_value; }

////////////////////////////////////////////////////////////
void Trait_Float::setValue(const float& t_value) { m_value = t_value; }

// ------------------------------------------------------- trait float ------------------------------------------------------- 
// ------------------------------------------------------- TRAIT COLOR ------------------------------------------------------- 

////////////////////////////////////////////////////////////
Trait_Color::Trait_Color(const TraitId& t_id, bool t_isActive, const float& t_inheritChance, const sf::Color& t_color) :
	Trait_Base(t_id, t_isActive, t_inheritChance),
	m_color{ t_color }
{}

////////////////////////////////////////////////////////////
TraitPtr Trait_Color::clone()const { return std::make_unique<Trait_Color>(m_id, m_isActive, m_inheritChance, m_color); }

////////////////////////////////////////////////////////////
TraitPtr Trait_Color::reproduce(SharedContext& t_context)const {
	float hueFactor{ t_context.m_rng->normalDisttribution(1.f,s_traitsStdDev) }; // For colors, the normal distribution changes the hue value
	auto hsl{ HSL::TurnToHSL(m_color) };
	hsl.Hue *= hueFactor;
	return std::make_unique<Trait_Color>(m_id, m_isActive, m_inheritChance, hsl.TurnToRGB());
}



////////////////////////////////////////////////////////////
const sf::Color& Trait_Color::getColor() const { return m_color; }

////////////////////////////////////////////////////////////
void Trait_Color::setColor(const sf::Color& t_color) { m_color = t_color; }

// ------------------------------------------------------- trait color ------------------------------------------------------- 






// ------------------------------------------------------------- TRAITS' IMPLEMENTATION ------------------------------------------------------------------


////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_MaxEnergy(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	float e{ dynamic_cast<Trait_Float*>(t_trait)->getValue() };
	t_organism->m_trait_maxEnergy = e;
	t_organism->m_energy = e;
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_DigestiveEfficiency(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_digestiveEfficiency = dynamic_cast<Trait_Float*>(t_trait)->getValue();
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_RestingMetabolicRate(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_restingMetabolicRate = dynamic_cast<Trait_Float*>(t_trait)->getValue();
	t_organism->m_rmr = t_organism->m_mass * t_organism->m_trait_restingMetabolicRate; // Also included in size trait to not enforce a loading order
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_MovementSpeed(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_movementSpeed = dynamic_cast<Trait_Float*>(t_trait)->getValue();
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_TurningSpeed(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_turningSpeed = dynamic_cast<Trait_Float*>(t_trait)->getValue();
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Lifespan(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_lifespan = dynamic_cast<Trait_Float*>(t_trait)->getValue();
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Size(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->m_trait_size = dynamic_cast<Trait_Float*>(t_trait)->getValue();
	t_organism->m_mass = 4.1887902f * std::powf(t_organism->m_trait_size * 0.5f,3.f); // mass : volume = (4/3)pi * (diameter/2)^3
	t_organism->m_rmr = t_organism->m_mass * t_organism->m_trait_restingMetabolicRate;
	t_organism->m_sprite.setScale(t_organism->m_trait_size, t_organism->m_trait_size);
}

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Color(Trait_Base* t_trait, Organism* t_organism, const float& t_elapsed) {
	t_organism->setColor(dynamic_cast<Trait_Color*>(t_trait)->getColor());
}

// ------------------------------------------------------------- traits' implementation ------------------------------------------------------------------