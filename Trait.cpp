#include "Trait.h"
#include "Organism.h"
#include "SharedContext.h"
#include "RandomGenerator.h"



// Used some macros to cut down the space needed for the table initializer
#define TB Trait_Base						// T.B	 = Trait Base
#define MT std::make_tuple					// M.T.	 = Make Tuple
#define TID TraitId							// T.I.D = Trait ID
#define	MF std::make_unique<Trait_Float>	// M.K.  = Make Float
#define MC std::make_unique<Trait_Color>	// M.C.  = Make Color
#define TEF TraitEffectTime					// T.E.T = Trait Effect Time
#define SFC sf::Color						// S.F.C = Simple Fast Color (SFML)

static const bool NON_VITAL_TRAIT{ false };
static const bool VITAL_TRAIT{ true };
static const bool INACTIVE_TRAIT{ false };
static const bool ACTIVE_TRAIT{ true };
static const float ALWAYS_INHERITED{ 1.f };


// ------------------------------------------------------- TRAIT BASE ------------------------------------------------------- 
enum TraitTupleElements { TRAIT_NAME, TRAIT_FUNCTOR, TRAIT_PTR };

////////////////////////////////////////////////////////////
const std::string& Trait_Base::getTraitName(const TraitId& t_id) {
	static const std::string emptyStr{ "" };
	auto it{ s_defaultTraits.find(t_id) };
	return (it != s_defaultTraits.cend() ? std::get<TRAIT_NAME>(it->second) : emptyStr);
}

////////////////////////////////////////////////////////////
TraitFunctor Trait_Base::getTraitFunctor(const TraitId& t_id) {
	auto it{ s_defaultTraits.find(t_id) };
	return (it != s_defaultTraits.cend() ? std::get<TRAIT_FUNCTOR>(it->second) : nullptr);
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
	m_effect{ std::bind(getTraitFunctor(t_id), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) }{}

////////////////////////////////////////////////////////////
void Trait_Base::update(Organism* t_owner, const float& t_elapsed) {
	m_effect(this, t_owner, t_elapsed);
}

////////////////////////////////////////////////////////////
TraitPtr Trait_Base::cloneDefaultTrait(const TraitId& t_id) {
	auto it{ s_defaultTraits.find(t_id) };
	if (it == s_defaultTraits.cend()) { return nullptr; }
	return std::move(std::get<TRAIT_PTR>(it->second)->clone());
}

////////////////////////////////////////////////////////////
TraitPtr Trait_Base::reproduceDefaultTrait(SharedContext& t_context, const TraitId& t_id) {
	auto it{ s_defaultTraits.find(t_id) };
	if (it == s_defaultTraits.cend()) { return nullptr; }
	return std::move(std::get<TRAIT_PTR>(it->second)->reproduce(t_context));
}

////////////////////////////////////////////////////////////
const VitalTraits& Trait_Base::getVitalTraits() { return s_vitalTraits; }

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
	TraitId::FoodDetectionRange,
	TraitId::Lifespan,
	TraitId::Size
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
const TraitTable Trait_Base::s_defaultTraits{
	//--- ------------------------------------------------------------------------------------------------ Trait_Float --------------------------------------------------------------------------------------------------------------
	// Key							string								functor										value		isActive		inheritChance	   effectTime			 functor
	{TID::MaxEnergy,				MT("Trait_MaxEnergy",				&TB::TraitFn_MaxEnergy,					MF(100.f,		ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_MaxEnergy))},
	{TID::DigestiveEfficiency,		MT("Trait_DigestiveEfficiency",		&TB::TraitFn_DigestiveEfficiency,		MF(0.5f,		ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_DigestiveEfficiency))},
	{TID::RestingMetabolicRate,		MT("Trait_RestingMetabolicRate",	&TB::TraitFn_RestingMetabolicRate,		MF(1.f,			ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_RestingMetabolicRate))},
	{TID::MovementSpeed,			MT("Trait_MovementSpeed",			&TB::TraitFn_MovementSpeed ,			MF(20.f,		ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_MovementSpeed))},
	{TID::TurningSpeed,				MT("Trait_TurningSpeed",			&TB::TraitFn_TurningSpeed ,				MF(5.f,			ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_TurningSpeed))},
	{TID::Lifespan,					MT("Trait_Lifespan",				&TB::TraitFn_Lifespan,					MF(100.f,		ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_Lifespan))},
	{TID::Size,						MT("Trait_Size",					&TB::TraitFn_Size,						MF(1.f,			ACTIVE_TRAIT,	ALWAYS_INHERITED,  TEF::OnConstruction,  &TB::TraitFn_Size))},
	// --------------------------------------------------------------------------------------------------- Trait_Color --------------------------------------------------------------------------------------------------------------
	// Key								string						    functor									color			isActive		inheritChance		effectTime			functor
	{TID::Color,					MT("Trait_Color",					&TB::TraitFn_Color,						MC(SFC::White,	ACTIVE_TRAIT,	ALWAYS_INHERITED,	TEF::OnConstruction, &TB::TraitFn_Color))}
};
// ---------- static members ---------- 

// ------------------------------------------------------- trait base --------------------------------------------------------
// ------------------------------------------------------- TRAIT FLOAT ------------------------------------------------------- 

////////////////////////////////////////////////////////////
TraitPtr Trait_Float::clone() const { return std::make_unique<Trait_Float>(m_id, m_isActive, m_inheritChance, m_value); }

////////////////////////////////////////////////////////////
TraitPtr Trait_Float::reproduce(SharedContext& t_context)const {
	float factor{ t_context.m_rng->normalDisttribution(1.f,s_traitsStdDev) };
	if (factor < s_minTraitFactor) { factor = s_minTraitFactor; }
	return std::make_unique<Trait_Float>(m_id, m_isActive, m_value * factor);
}
////////////////////////////////////////////////////////////
const float& Trait_Float::getValue() const { return m_value; }

////////////////////////////////////////////////////////////
void Trait_Float::setValue(const float& t_value) { m_value = t_value; }

// ------------------------------------------------------- trait float ------------------------------------------------------- 
// ------------------------------------------------------- TRAIT COLOR ------------------------------------------------------- 

////////////////////////////////////////////////////////////
Trait_Color::Trait_Color(const TraitId& t_id, bool t_isActive, const float& t_inheritChance, const sf::Color& t_color) :
	Trait_Base(t_id, t_isActive, m_inheritChance),
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
void Trait_Base::TraitFn_MaxEnergy(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_maxEnergy = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_DigestiveEfficiency(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_digestiveEfficiency = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_RestingMetabolicRate(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_restingMetabolicRate = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_MovementSpeed(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_movementSpeed = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_TurningSpeed(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_turningSpeed = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Lifespan(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_lifespan = t_trait->getValue(); }

////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Size(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_trait_size = t_trait->getValue(); }



////////////////////////////////////////////////////////////
void Trait_Base::TraitFn_Color(Trait_Color* t_trait, Organism* t_organism, const float& t_elapsed) { t_organism->m_color = t_trait->getColor(); }

// ------------------------------------------------------------- traits' implementation ------------------------------------------------------------------
