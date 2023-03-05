#ifndef __ISLAND_H__
#define __ISLAND_H__

/* How many collisions can we respond to at once*/
#define ISLAND_COLLISION_BUFFER (20)

/* Island model definition */
typedef struct
{
    int EntityId;
    int EntityCount;
    //BoundingBox * Colliders;
    //int ColliderCount;
    jo_vector_fixed Center;
} BlueSkiesIsland;

void IslandInitialize(void * currentAddress, int * lastEntityId, BlueSkiesIsland * island);

void IslandHandleCollision(const BlueSkiesIsland * const island, int (*comparer)(void*, void*), const jo_vector_fixed * const camera, jo_vector_fixed * velocity);

void IslandDraw(const BlueSkiesIsland * const island);

#endif
