#ifndef TRAITS_H
#define TRAITS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Trait.h"
#include "SharedContext.h"


using TraitCollectionPtr = std::unique_ptr<TraitCollection>;
using TraitsMap = std::unordered_map<TraitId, TraitPtr>; // Group by id
using TraitsContainer = std::unordered_map<TraitEffectTime, TraitsMap>; // Subdivide by effect time

class TraitCollection {

	TraitsContainer m_traits;

public:
	static const std::string& traitIdToStr(const TraitId& t_id);
	static TraitId traitStrToId(const std::string& t_str);
	static bool isTraitVital(const TraitId& t_id);
	static const VitalTraits& getVitalTraits();

	TraitCollection();

	bool hasTrait(const TraitId& t_id)const;
	void removeTrait(const TraitId& t_id); // Fails if trait is vital (does nothing if trait is not present)
	void addTrait(TraitPtr t_trait); // Overwrites any existent trait in the same slot

	bool setTraitValue(const TraitId& t_id, const float& t_value);
	bool getTraitValue(const TraitId& t_id, float& t_out_value)const;
	bool setTraitColor(const TraitId& t_id, const sf::Color& t_color);
	bool getTraitColor(const TraitId& t_id, sf::Color& t_out_color)const;

	bool isTraitActive(const TraitId& t_id, bool& t_out_isActive)const;
	bool activateTrait(const TraitId& t_id); // Returns the finding result of the trait
	bool deactivateTrait(const TraitId& t_id); // Returns the finding result of the trait

	bool getTraitInheritChance(const TraitId& t_id, float& t_out_inheritChance)const;
	bool setTraitInheritChance(const TraitId& t_id, const float& t_inheritChance);

	const Trait_Base* getTrait(const TraitId& t_id)const;
	Trait_Base* getTrait(const TraitId& t_id);

	void onOrganismConstruction(Organism* t_owner, const float& t_elapsed); // Calls the update methods of construction based traits (mainly value setters)
	void update(Organism* t_owner, const float& t_elapsed); // Calls the update function every trait
	TraitCollectionPtr clone(); // Retrns perfect copy of the set of traits
	TraitCollectionPtr reproduce(SharedContext& t_context); // Produces an offspring of the set of traits affected by their chance to be passed




};

#endif // !TRAIT_H