# Genesia

## Genetic evolution simulation
Organisms find food, repoduce and pass their genes to their offspring.

## Controls
- `ARROW KEYS`: Move camera
- `P`: Pause/unpause
- `R`: Reset view
- `W`: Zoom in
- `S`: Zoom out

***

## Energy
There is a limited amound of energy present in the environment at any 
given time. This limits the amount of food and organisms that can be
spawned. Bigger organisms increase the energy they hold geometrically
with their dimensions.

## Reproduction
The critters reproduce asexually. They need a specific energy threshold
calculated on their specific traits and charactersitics and produce an
offspring.

## Inheritance
Traits inherited experiment slight changes to their numeric influece 
based on normal distribution. Over time this steers the gene pool and
more and more appropriate organisms evolve.

## Miscelaneous
- Built own actor/entity system from scratch.
- The idle movement of organisms uses Perlin noise to appear natural.
- Collision is implemented with a quadtree.
