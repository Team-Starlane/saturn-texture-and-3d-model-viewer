/*
Concepts in C to read up upon for this model viewer as a beginner
(use any search engine of choice and type in the topics listed here for further reading).

#include statements > referencing code from different .c files
by taking them from .h lists.
The meaning of bracketing in #include statements.
	
Example: 
https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170
*/

#include <jo/jo.h>
// Include everything this .h file lists, which is XL2's entire ZTP library.
// Take it from this subfolder.
// IMPORTANT: Do not write file extensions in capital letters in Linux! Case sensitive OS.
#include "ZT/ZT_COMMON.h"

// Current model and total number of models
static int CurrentEntity = 0;
static int TotalEntities = 0;

// Viewport angle
#define FOV_MIN (DEGtoANG(10))
#define FOV_MAX (DEGtoANG(160))
#define FOV_STEP (DEGtoANG(1))
static ANGLE Fov = (DEGtoANG(70));

// Values for rotation. Base matrix rotation value and rotation frustrum.
int rot_multiplier = 1;
int rot_x = 0;
int rot_y = 0;
int rot_z = 0;
int rot_neg_frust = -90;
int rot_pos_frust = 90;
int neg_frust_ext = -180;
int pos_frust_ext = 180;

// Values for z-depth.
#define Z_BASE 500
int z = Z_BASE;
int far = 1000;
int near = 200;
int frustrum = 20;

jo_img bg;

void			my_input(void)
{
    // Cycle through all models
    if  (jo_is_pad1_key_down(JO_KEY_A) && CurrentEntity < TotalEntities)
    {
        CurrentEntity++;
    }
    else if (jo_is_pad1_key_down(JO_KEY_X) && CurrentEntity >= 0)
    {
        CurrentEntity--;
    }

    // Handle wrapping around
    if (CurrentEntity >= TotalEntities)
    {
        CurrentEntity = 0;
    }
    else if (CurrentEntity < 0)
    {
        CurrentEntity = TotalEntities - 1;
    }

    if  (jo_is_pad1_key_pressed(JO_KEY_C))
    {
        z+=(z>>2) / 25 + 1;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_Z))
    {
        if (z>frustrum){
            z-=(z>>2) / 25 + 1;
        }
        // Readding angry smiley:
        else{z++;} 
    }
    
    // Horizontal axis.
    if (jo_is_pad1_key_pressed(JO_KEY_UP) && rot_x > rot_neg_frust)
    {
        rot_x--;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_UP) && rot_x < rot_neg_frust)
    {
        rot_x++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && rot_x < rot_pos_frust)
    {
        rot_x++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && rot_x > rot_pos_frust)
    {
        rot_x--;
    }

    // Vertical axis.

    if (jo_is_pad1_key_pressed(JO_KEY_LEFT) && rot_y > rot_neg_frust)
    {
        rot_y-=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_LEFT) && rot_y < rot_neg_frust)
    {
        rot_y+=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && rot_y < rot_pos_frust)
    {
        rot_y+=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && rot_y > rot_pos_frust)
    {
        rot_y-=rot_multiplier;
    }

    // Axis on the line of the camera / depth. Tilts image left and right.

    if (jo_is_pad1_key_pressed(JO_KEY_L) && rot_z > neg_frust_ext)
    {
        rot_z-=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_L) && rot_z < neg_frust_ext)
    {
        rot_z+=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_R) && rot_z < pos_frust_ext)
    {
        rot_z+=rot_multiplier;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_R) && rot_z > pos_frust_ext)
    {
        rot_z-=rot_multiplier;
    }

	// FOV > Field of view.
    if (jo_is_pad1_key_pressed(JO_KEY_B) && Fov > FOV_MIN)
    {
        Fov -= FOV_STEP;
        slPerspective(Fov);
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_Y) && Fov < FOV_MAX)
    {
        Fov += FOV_STEP;
        slPerspective(Fov);
    }

    // Reset all values.
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        rot_x = 0;
        rot_y = 0;
        rot_z = 0;
        z = Z_BASE;
		Fov = (DEGtoANG(70));
    }
}

void        my_draw(void)
{
    jo_3d_push_matrix();
    {
        jo_3d_translate_matrix_z(z);
        jo_3d_rotate_matrix_x(rot_x);
        jo_3d_rotate_matrix_y(rot_y);
        jo_3d_rotate_matrix_z(rot_z);

        if (CurrentEntity < TotalEntities && CurrentEntity >= 0)
        {
            for(int mesh = 0; mesh < entities[CurrentEntity].nbMeshes; mesh++)
	        {
	            slPutPolygon((XPDATA*)entities[CurrentEntity].pol[mesh]);
	        }
        }
    }
    jo_3d_pop_matrix();
    
    // Clear screen line 2 and following, because the numbers freak otherwise, when z value is updated.
    // Don't clear line 0, because clearing is broke on line 0.
    jo_printf(0, 0, "Press Start to reset all values.         ");
    jo_printf(0, 2, "Z-depth (press C or Z): %d               ", z);
    jo_printf(0, 3, "Model (cycle with A and X): %d/%d        ", CurrentEntity + 1, TotalEntities);
    jo_printf(0, 4, "Rotat. on x (press Up or Down): %d       ", rot_x);
    jo_printf(0, 5, "Rotat. on y (press Left or Right): %d    ", rot_y);
    jo_printf(0, 6, "Rotat. on z (press L or R trigger): %d   ", rot_z);
    jo_printf(0, 7, "Field of View (press B or Y): %d         ", jo_fixed2int(slAng2FX(Fov)));
}

/** @brief Load assets
 */
void LoadAssets()
{
    // Load background.
    bg.data = NULL;
    jo_tga_loader(&bg, JO_ROOT_DIR, "GRID.TGA", JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 96, 56);
    jo_free_img(&bg);

    // Set default perspective
    slPerspective(Fov);

	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
    int entityIds = 0;

    // Load models.
    // To add new ones, just duplicate this line.
    currentAddress = ztLoad3Dmodel((Sint8*)"IS00DS_G.ZTP", currentAddress, entityIds++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"testdsp1.ZTP", currentAddress, entityIds++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"testdsp2.ZTP", currentAddress, entityIds++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"testlod1.ZTP", currentAddress, entityIds++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"testlod2.ZTP", currentAddress, entityIds++, false);

    // Set our total model count
    TotalEntities = entityIds;
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);

    LoadAssets();
    
    jo_core_add_callback(my_input);
    jo_core_add_callback(my_draw);

	jo_core_run();
}