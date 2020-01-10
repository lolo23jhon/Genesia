#ifndef TRAITS_H
#define TRAITS_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SharedContext.h"

class Organism;
class Traits;
struct Trait;
enum class TraitId;
enum class TraitEffectTime;

using VitalTraits = std::unordered_set<TraitId>;
using TraitStrings = std::unordered_map<std::string, TraitId>;

using TraitsMap = std::unordered_map<TraitId, Trait>; // Group by id
using TraitsContainer = std::unordered_map<TraitEffectTime, TraitsMap>; // Subdivide by effect time

using TraitsPtr = std::unique_ptr<Traits>;
using TraitFunctor = void(Traits::*)(Trait*, Organism*, const float&);
using TraitCallback = std::function<void(Trait*, Organism*, const float&)>;

using TraitTuple = std::tuple< std::string, float, bool, bool, float, TraitEffectTime, TraitFunctor>;
using DefaultTraits = std::unordered_map<TraitId, TraitTuple>;

enum class TraitId {
	INVALID_TRAID_ID = -1,

	// ----------------------  Vital Traits ----------------------
	MaxEnergy,				// (eu)		Energy storage capacity
	DigestiveEfficiency,	// ([0 1))	How much energy is extracted from food
	RestingMetabolicRate,	// (eu/s)   How much energt is expended for maintaining vital functions per second
	MovementSpeed,			// (u/s)    How fast the organism moves noramally
	TurningSpeed,			// (deg/s)  How fast the actor can change of direction
	FoodDetectionRange,		// (u)	    Distance at which the actor can detect food
	Lifespan,				// (s)		Natural living time of the strain
};


enum class TraitEffectTime {
	OnConstruction,		// Called at the organism's birth
	Continuously		// Effect called every frame
};

struct Trait {

	TraitId m_id;					// Trait id in the universal trait pool of all the potentially existant traits
	float m_value;					// Generic value used for anything the trait needs to affect the host
	bool m_isActive;				// Whether or not the trait affects the current instance of the strain (could be a dormant or epigenetic trait)
	bool m_isVital;					// True for basic, low level genes that are present on all creatures. Their effects can be overriden by other genes though.
	float m_inheritChance;			// Pct chance for the offspring to inherit the trait. All vital traits have 100% chance.
	TraitEffectTime m_effectTime;	// Identifier for the moment at which the trait's effect is called
	TraitCallback m_effect;			// The effect of the trait, which is exerted throug a callback to keep their potential broad.

	Trait(Traits* t_traitsObj,
		const TraitId& t_id,
		const float& t_value,
		bool t_isActive,
		const float& t_inheritChance,
		const TraitEffectTime& t_effectTime,
		TraitFunctor t_effect);
	Trait clone()const; // Creates perfect copy
	void update(Organism* t_owner, const float& t_elapsed);

};




class Traits {


	static const VitalTraits s_vitalTraits;
	static const DefaultTraits s_defaultTraits;

	TraitsContainer m_traits;

public:
	static const TraitStrings s_traitStrings;
	static const std::string& traitIdToStr(const TraitId& t_id);
	static TraitId traitStrToId(const std::string& t_str);
	static bool isTraitVital(const TraitId& t_id);
	static const VitalTraits& getVitalTraits();

	Traits();

	bool hasTrait(const TraitId& t_id)const;
	void removeTrait(const TraitId& t_id); // Fails if trait is vital (does nothing if trait is not present)
	void addTrait(const Trait& t_trait); // Overwrites any existent trait in the same slot

	bool setTraitValue(const TraitId& t_id, const float& t_value);
	bool getTraitValue(const TraitId& t_id, float& t_out_value)const;

	bool isTraitActive(const TraitId& t_id, bool& t_out_isActive)const;
	bool activateTrait(const TraitId& t_id); // Returns the finding result of the trait
	bool deactivateTrait(const TraitId& t_id); // Returns the finding result of the trait

	bool getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const;
	bool setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance);

	const Trait* getTrait(const TraitId& t_id)const;
	Trait* getTrait(const TraitId& t_id);

	void onOrganismConstruction(Organism* t_owner, const float& t_elapsed); // Calls the update methods of construction based traits (mainly value setters)
	void update(Organism* t_owner, const float& t_elapsed); // Calls the update function every trait
	TraitsPtr clone(); // Retrns perfect copy of the set of traits
	TraitsPtr reproduce(SharedContext& t_context); // Produces an offspring of the set of traits affected by their chance to be passed


	// --------------------------------------------- TRAITS ---------------------------------------------------------
	static void Trait_MaxEnergy(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	static void Trait_DigestiveEfficiency(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	static void Trait_RestingMetabolicRate(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	static void Trait_MovementSpeed(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	static void Trait_TurningSpeed(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	static void Trait_Lifespan(Trait* t_trait, Organism* t_organism, const float& t_elapsed);
	// --------------------------------------------- traits ---------------------------------------------------------

};

#endif // !TRAIT_H