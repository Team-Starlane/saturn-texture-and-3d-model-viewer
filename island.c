#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "vectors.h"
#include "collision.h"
#include "sort.h"
#include "island.h"

void IslandInitialize(void * currentAddress, int * lastEntityId, BlueSkiesIsland * island)
{
    // Basic initialization
    island->EntityId = *lastEntityId;
    island->Center.x = 0;
    island->Center.y = 0;
    island->Center.z = 0;
    currentAddress = ztLoad3Dmodel((Sint8*)"IS00DS_G.ZTP", currentAddress, (*lastEntityId)++, false);
    currentAddress = ztLoad3Dmodel((Sint8*)"IS00DS_B.ZTP", currentAddress, (*lastEntityId)++, false);
    island->EntityCount = (*lastEntityId) - island->EntityId;

    // Prepare array for bounding boxes
    int totalBoxes = 0;

    for (int entity = island->EntityId; entity < island->EntityId + island->EntityCount; entity++)
    {
        for(int mesh = 0; mesh < entities[mesh].nbMeshes; mesh++)
	    {
            //	Rectify vertex directions
            //	Number of quads in model
            int num_quads = entities[entity].pol[mesh]->nbPolygon;

            //	Start loop
            for (int i = 0; i < num_quads; i++)
            {
                //	Disable lighting
                entities[mesh].pol[mesh]->attbl[i].sort &= ~UseLight;
                
                //	Change Z sort
                entities[entity].pol[mesh]->attbl[i].sort &= ~3;
                if(entity == island->EntityId)
                {
                    //	Island base, skybox sorted at max distance
                    entities[entity].pol[mesh]->attbl[i].sort |= SORT_MAX;
                }
                else
                {
                    //	Buildings sorted at min distance
                    entities[entity].pol[mesh]->attbl[i].sort |= SORT_MIN;
                }
            }

            // Count bounding boxes
            totalBoxes += entities[entity].pol[mesh]->nbPolygon;
        }
    }

    island->Colliders = (BoundingBox*)jo_malloc(sizeof(BoundingBox) * totalBoxes);
    island->ColliderCount = totalBoxes;
    
    // Generate boxes
    int currentBox = 0;

    for (int entity = island->EntityId; entity < island->EntityId + island->EntityCount; entity++)
    {
        for(int mesh = 0; mesh < entities[entity].nbMeshes; mesh++)
        {
            int polygons = entities[entity].pol[mesh]->nbPolygon;

            for (int polygon = 0; polygon < polygons; polygon++)
            {
                jo_pos3D_fixed min = { JO_FIXED_MAX, JO_FIXED_MAX, JO_FIXED_MAX };
                jo_pos3D_fixed max = { JO_FIXED_MIN, JO_FIXED_MIN, JO_FIXED_MIN };

                for (int point = X; point < XYZS; point++)
                {
                    jo_fixed x = entities[entity].pol[mesh]->pntbl[entities[entity].pol[mesh]->pltbl[polygon].Vertices[point]][X];
                    jo_fixed y = entities[entity].pol[mesh]->pntbl[entities[entity].pol[mesh]->pltbl[polygon].Vertices[point]][Y];
                    jo_fixed z = entities[entity].pol[mesh]->pntbl[entities[entity].pol[mesh]->pltbl[polygon].Vertices[point]][Z];

                    min.x = JO_MIN(x, min.x) - JO_FIXED_4;
                    min.y = JO_MIN(y, min.y) - JO_FIXED_4;
                    min.z = JO_MIN(z, min.z) - JO_FIXED_4;

                    max.x = JO_MAX(x, max.x) + JO_FIXED_4;
                    max.y = JO_MAX(y, max.y) + JO_FIXED_4;
                    max.z = JO_MAX(z, max.z) + JO_FIXED_4;
                }

                island->Colliders[currentBox].Min.x = min.x;
                island->Colliders[currentBox].Min.y = min.y;
                island->Colliders[currentBox].Min.z = min.z;

                island->Colliders[currentBox].Max.x = max.x;
                island->Colliders[currentBox].Max.y = max.y;
                island->Colliders[currentBox].Max.z = max.z;

                jo_vector_fixed_add(&island->Colliders[currentBox].Min, &island->Colliders[currentBox].Max, &island->Colliders[currentBox].Center);
                jo_vector_fixed_div(&island->Colliders[currentBox].Center, JO_FIXED_2, &island->Colliders[currentBox].Center);

                currentBox++;
            }
        }
    }
}

void IslandHandleCollision(const BlueSkiesIsland * const island, int (*comparer)(void*, void*), const jo_vector_fixed * const camera, jo_vector_fixed * velocity)
{
    // Collide with island
    bool hadCollision;
    BoundingBox * collisionBuffer[ISLAND_COLLISION_BUFFER];

    do 
    {
        // Resset possible collisions
        int blockCount = 0;
        hadCollision = false;

        // Get possible colliding blocks
        for(int box = 0; box < island->ColliderCount && blockCount < ISLAND_COLLISION_BUFFER; box++)
        {
            if (CollisionBroadPointToBox(&(island->Colliders[box]), camera, velocity))
            {
                collisionBuffer[blockCount++] = &(island->Colliders[box]);
            }
        }

        // Sort boxes by distance
        QuickSort((void**)collisionBuffer, comparer, blockCount);

        // Check collisions
        for(int box = 0; box < blockCount; box++)
	    {
            jo_vector_fixed slideVector;

            if (CollisionResponse(collisionBuffer[box], camera, velocity, &slideVector))
            {
                jo_vector_fixed_copy(&slideVector, velocity);
                hadCollision = true;
            }
	    }
    }
    while (hadCollision);
}

void IslandDraw(const BlueSkiesIsland * const island)
{
    /*
        Plenty of drawing optimizations are needed here
    */
    jo_3d_translate_matrix_fixed(island->Center.x, island->Center.y, island->Center.z);

    for (int entity = island->EntityId; entity < island->EntityId + island->EntityCount; entity++)
    {
        for(int mesh = 0; mesh < entities[entity].nbMeshes; mesh++)
	    {
	        slPutPolygon((XPDATA*)entities[entity].pol[mesh]);
	    }
    }
}