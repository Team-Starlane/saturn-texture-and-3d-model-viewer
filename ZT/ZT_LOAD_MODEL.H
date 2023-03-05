#ifndef ZT_LOAD_MODEL_H
#define ZT_LOAD_MODEL_H

#define MAX_MESHES (40)
#define MAX_MODELS (40)

/**Gouraud shading*/
#define	GRtbl(n)	    (0x1c38+(n))
#define	GRaddr	        0x1c38
#define GOUR_REAL_MAX   768

#define LWRAM	(2097152)


typedef     Sint16	compVert[XYZ];
typedef     Uint8   compNorm;

typedef struct
{
 compVert * cVert;
 compNorm * cNorm;

} anim_struct;

typedef struct
{
    unsigned short  TOTAL_MESH; //total amount of PDATA
    short           TOT_TEXT;  //total amount of textures
    unsigned int    TEXT_SIZE;  //to quickly load from disk, that's the size of the textures in bytes
    unsigned int    PDATA_SIZE; //to quickly load from disk, total size of pdata in bytes
    FIXED           Origin[XYZ]; //Origin point used to "center" the culling/collision data and your model's position. Should be 0,0,0 unless you have an offset
    FIXED           Length[XYZ];  //Lenght along the x,y z axis. Together with the origin, that gives you the bounding box for quick broad collision testing
    unsigned short nbFrames;
    unsigned short framerate; //Interpolation factor of 2, means the number of frames between 2 key frames
    anim_struct *  animation;
} modelData_t;

typedef struct
{
    FIXED   pos[XYZ];
    FIXED   length[XYZ];
    Uint16 nbMeshes;
    Uint16 nbFrames;
    Uint16 AnimInterpolation;
    anim_struct * animation[240];
    XPDATA * pol[MAX_MESHES];
} entity_t;

typedef struct
{
	Bool	file_done;
	Bool	active;
	void *	dstAddress;
	entity_t * tmodel;
	Sint8 * filename;
}	request;

/**Store all your PDATA meshes here**/
extern entity_t entities[MAX_MODELS];
//	Stores sizes of pData for corresponding entities for malloc/freeing
//	(Added by Emerald Nova)
extern Uint32 pDataSizes[MAX_MODELS];
extern unsigned int gouraudCounter;

extern  request	requests[80];

void setTextures(Uint32 id, entity_t * model, unsigned int total_meshes, Bool UseRealtimeGouraud);

Uint16 loadTextures(void * startAddress, modelData_t * modelData);

void * loadPDATA(void * startAddress, entity_t * model, modelData_t * modelData);

void * loadAnimations(void * startAddress, entity_t * model, modelData_t * modelData);

/** This function loads a 3d polygonal model. Returns the last address in LWRAM (to allow loading multiple meshes)**/
void *      ztLoad3Dmodel(Sint8 * filename, void * startAddress, int entity_ID, Bool UseRealtimeGouraud);
//Notice: In loading order, models come first.
void * ztModelRequest(char * dirname, Sint8 * name, void * startAddress, entity_t * model, Uint16 rqNum, Bool dirChange);

void	master_file_system(void(*game_code)(void));

#endif // ZT_LOAD_MODEL_H
