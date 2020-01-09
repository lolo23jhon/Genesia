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
class Trait;
enum class TraitId;

using VitalTraits = std::unordered_set<TraitId>;
using TraitStrings = std::unordered_map<std::string, TraitId>;


using TraitsMap = std::unordered_map<TraitId, Trait>;


using TraitsPtr = std::unique_ptr<Traits>;
using TraitCallback = std::function<void>(Trait*, Organism*, const float&);

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



struct Trait {
	TraitId m_id;			// Trait id in the universal trait pool of all the potentially existant traits
	float m_value;			// Generic value used for anything the trait needs to affect the host
	bool m_isActive;		// Whether or not the trait affects the current instance of the strain (could be a dormant or epigenetic trait)
	bool m_isVital;			// True for basic, low level genes that are present on all creatures. Their effects can be overriden by other genes though.
	float m_inheritChance;  // Pct chance for the offspring to inherit the trait. All vital traits have 100% chance.
private:
	TraitCallback m_effect; // The effect of the trait, which is exerted throug a callback to keep their potential broad.
public:
	Trait(const TraitId& t_id,
		const float& t_value,
		bool t_isActive,
		const float& t_inheritChance,
		TraitCallback t_effect);

	Trait clone()const; // Creates perfect copy
	void update(Organism* t_owner, const float& t_elapsed);
};




class Traits {


	static const TraitStrings s_traitStrings;
	static const VitalTraits s_vitalTraits;
	static const TraitsMap s_defaultTraits;

	TraitsMap m_traits;

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

	void setTraitValue(const TraitId& t_id, const float& t_value);
	bool getTraitValue(const TraitId& t_id, float& t_out_value)const;

	bool isTraitActive(const TraitId& t_id, bool& t_out_isActive)const;
	bool activateTrait(const TraitId& t_id); // Returns the finding result of the trait
	bool deactivateTrait(const TraitId& t_id); // Returns the finding result of the trait

	bool getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const;
	bool setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance);

	const Trait* getTrait(const TraitId& t_id)const;
	Trait* getTrait(const TraitId& t_id);

	void update(Organism* t_owner, const float& t_elapsed); // Calls the update function every trait
	TraitsPtr clone(); // Retrns perfect copy of the set of traits
	TraitsPtr reproduce(SharedContext& t_context); // Produces an offspring of the set of traits affected by their chance to be passed

};

#endif // !TRAIT_H