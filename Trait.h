#ifndef TRAIT_H
#define TRAIT_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <SFML/Graphics/Color.hpp>

class Organism;
class TraitCollection;
class Trait_Base;
class Trait_Float;
class Trait_Color;
enum class TraitId;
enum class TraitEffectTime;

using VitalTraits = std::unordered_set<TraitId>;
using TraitStrings = std::unordered_map<std::string, TraitId>;
using TraitPtr = std::unique_ptr<Trait_Base>;
using TraitFunctor = void(TraitCollection::*)(Trait_Base*, Organism*, const float&);
using TraitCallback = std::function<void(Trait_Base*, Organism*, const float&)>;
using TraitTuple = std::tuple<std::string, TraitFunctor, TraitPtr>;
using TraitTable = std::unordered_map<TraitId, TraitTuple>;
using TraitSet = std::unordered_set<TraitId>;


enum class TraitId {
	INVALID_TRAID_ID = -1,

	// ----------------------  Vital Traits ----------------------
	MaxEnergy,				// (eu)		Energy storage capacity.
	DigestiveEfficiency,	// ([0 1))	How much energy is extracted from food.
	RestingMetabolicRate,	// (eu/s)   How much energt is expended for maintaining vital functions per second.
	MovementSpeed,			// (u/s)    How fast the organism moves noramally.
	TurningSpeed,			// (deg/s)  How fast the actor can change of direction.
	FoodDetectionRange,		// (u)	    Distance at which the actor can detect food.
	Lifespan,				// (s)		Natural living time of the strain.
	Size,					// (u)		Size of the organism. Affects energy consumption.
	Color
};
enum class TraitEffectTime {
	OnConstruction,		// Called at the organism's birth
	Continuously		// Effect called every frame
};



class Trait_Base {
protected:
	static const TraitTable s_defaultTraits;
	static const TraitSet s_vitalTraits;
	static const TraitSet s_floatTraits;
	static const TraitSet s_colorTraits;
	static const float s_traitsStdDev; // Describes the height if the bell curve of percentual change when traits are inherited
	static const float s_minTraitFactor;

public:

	static const std::string& getTraitName(const TraitId& t_id);
	static TraitFunctor getTraitFunctor(const TraitId& t_id);
	static bool isTraitVital(const TraitId& t_id);
	static bool isTraitFloat(const TraitId& t_id);
	static bool isTraitColor(const TraitId& t_id);
	static const TraitSet& getVitalTraits();
	static const float& getTraitStdDev();
	static TraitPtr cloneDefaultTrait(const TraitId& t_id);		// Returns perfect copy of a default trait
	static TraitPtr reproduceDefaultTrait(SharedContext& t_context,const TraitId& t_id); // Returns a slightly altered copy of a default trait, as to simulate reproduction

	Trait_Base(const TraitId& t_id, bool t_isActive, const float& t_inheritChance);

	const TraitId& getId()const;
	bool getIsActive()const;
	void setIsActive(bool t_isActive);
	bool getIsVital()const;
	const float& getInheritChance()const;
	void setInheritChance(const float& t_inheritChance);
	const TraitEffectTime& getEffectTime()const;
	void update(Organism* t_owner, const float& t_elapsed);

	virtual TraitPtr clone()const = 0; // Creates perfect copy
	virtual TraitPtr reproduce(SharedContext& t_context)const = 0; // Creates a copy based on reproduction system

protected:
	TraitId m_id;					// Trait id in the universal trait pool of all the potentially existant traits
	bool m_isActive;				// Whether or not the trait affects the current instance of the strain (could be a dormant or epigenetic trait)
	float m_inheritChance;			// Pct chance for the offspring to inherit the trait. All vital traits have 100% chance.
	TraitEffectTime m_effectTime;	// Identifier for the moment at which the trait's effect is called
	TraitCallback m_effect;			// The effect of the trait, which is exerted throug a callback to keep their potential broad.




private:
	// --------------------------------------------- TRAITS ---------------------------------------------------------
	static void TraitFn_MaxEnergy(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_DigestiveEfficiency(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_RestingMetabolicRate(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_MovementSpeed(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_TurningSpeed(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_Lifespan(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);
	static void TraitFn_Size(Trait_Float* t_trait, Organism* t_organism, const float& t_elapsed);

	static void TraitFn_Color(Trait_Color* t_trait, Organism* t_organism, const float& t_elapsed);
	// --------------------------------------------- traits ---------------------------------------------------------
};

class Trait_Float : public Trait_Base {

	float m_value;					// Generic value used for anything the trait needs to affect the host
public:
	Trait_Float(const TraitId& t_id,
		bool t_isActive,
		const float& t_inheritChance,
		const float& t_value);

	const float& getValue()const;
	void setValue(const float& t_value);

	TraitPtr clone()const; 
	TraitPtr reproduce(SharedContext& t_context)const;
};

class Trait_Color : public Trait_Base {

	sf::Color m_color;
public:
	Trait_Color(const TraitId& t_id,
		bool t_isActive,
		const float& t_inheritChance,
		const sf::Color& t_color);

	const sf::Color& getColor()const;
	void setColor(const sf::Color& t_color);

	TraitPtr clone()const;
	TraitPtr reproduce(SharedContext& t_context)const;
};

#endif // !TRAIT_H