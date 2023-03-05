#include <jo/jo.h>
// Include everything this .h file lists, which is XL2's entire ZTP library.
// Take it from this subfolder.
// IMPORTANT: Do not write file extensions in capital letters in Linux! Case sensitive OS.
#include "ZT/ZT_COMMON.h"
#include "island.h"

/*  Image variables.
    
    XN: I believe that in Jo the maximum number of 8 bit palettes is 4 (4 x 256 = 1024).
    Palette colors are not shared across multiple palettes.
    
    Consider using a single palette for multiple images and coordinating the indices of colors
    across multiple images.

    E.g. Image 1 takes color indices 0-7 and images 2 takes indices 8-15 and so forth.

*/
static jo_palette       palette1;
static jo_palette       palette2;
static jo_palette       palette3;
static jo_palette       palette4;
static jo_palette       palette5;
static jo_palette       palette6;
static jo_palette       palette7;
static jo_palette       palette8;

/*  XN:

    A.tga = Display mesh.
    B through E.tga = LOD levels 0 to 3.

*/ 
#define IMG_MAX 6
enum images{a_dot_tga, b_dot_tga, c_dot_tga, d_dot_tga, e_dot_tga, f_dot_tga};
int img_loaded = a_dot_tga;
int spriteIndex = 0;

// Values for rotation. Base matrix rotation value and rotation frustrum.
int rot_multiplier = 1;
int rot_x = 0;
int rot_y = 0;
int rot_z = 0;
int rot_neg_frust = -90;
int rot_pos_frust = 90;
int neg_frust_ext = -180;
int pos_frust_ext = 180;
bool toggle_auto_rot;

// Values for z-depth.
#define Z_BASE 75
int z = Z_BASE;
int far = 1000;
int near = 200;
int frustrum = 20;

/* Main island */
static BlueSkiesIsland Island;

// Reyes stuff.
/*
void            my_draw(void)
{
    jo_sprite_set_palette(spriteIndex);
    jo_sprite_draw3D(spriteIndex, 0, 0, 500);
}
*/

void			my_input(void)
{
    // IMG_MAX is the number of images you put into your cd root.
    // Cycle through all compiled images.
    if  (jo_is_pad1_key_down(JO_KEY_A) && spriteIndex < IMG_MAX)
    {
        spriteIndex++;
    }
    else if (jo_is_pad1_key_down(JO_KEY_A) && spriteIndex > IMG_MAX)
    {
        spriteIndex == 0;
    }
    else if (jo_is_pad1_key_down(JO_KEY_B) && spriteIndex >= 0)
    {
        spriteIndex--;
    }
    else if (jo_is_pad1_key_down(JO_KEY_B) && spriteIndex < 0)
    {
        spriteIndex == IMG_MAX;
    }

    if  (jo_is_pad1_key_pressed(JO_KEY_C))
    {
        
        z+=(z>>2) / 25 + 1;
        
        /*
		if(z > far)
		{
			z = z+12;
		}
		else if (z > near)
        {
            z = z+3;
        }
        else
		{
			z++;
		}
        */
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_Z))
    {
        if (z>frustrum){
            z-=(z>>2) / 25 + 1;
        }
        // Readding angry smiley:
        else{z++;} 

        /*
		if(z > far)
		{
			z = z-12;
		}
		else if (z > near)
        {
            z = z-3;
        }
        else if (z > frustrum)
		{
			z--;
		}
        else
        {
            z++;
        }
        */
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

    // Reset all values.
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
    rot_x = 0;
    rot_y = 0;
    rot_z = 0;
    z = Z_BASE;
    }
}

void        my_draw(void)
{
    // New ZTP draw code.
    MATRIX matrix;
    
    jo_3d_push_matrix();
    {
        // Set 3D matrix
        
        // Don't wanna use Reyes coordinate system.
        // ExGetMatrixToCoordinateSystem(&Camera, matrix);
        slLoadMatrix(matrix);

        jo_3d_translate_matrix_z(z);
        jo_3d_rotate_matrix_x(rot_x);
        jo_3d_rotate_matrix_y(rot_y);
        jo_3d_rotate_matrix_z(rot_z);

        // Draw island
        IslandDraw(&Island);
        
        // Draw water
        //jo_background_3d_plane_a_draw(true);
        //jo_background_3d_plane_b_draw(true);
    }
    jo_3d_pop_matrix();
    
    /* Old draw code

    // Setting palette every frame seems not to matter.
    // Push matrix, apply transformations from button inputs, draw sprite, pop matrix.
    jo_sprite_set_palette(palette1.id);
    jo_3d_push_matrix();
    jo_3d_translate_matrix_z(z);
    jo_3d_rotate_matrix_x(rot_x);
    jo_3d_rotate_matrix_y(rot_y);
    jo_3d_rotate_matrix_z(rot_z);
    jo_3d_create_sprite_quad (spriteIndex);
    jo_3d_pop_matrix();

    */

    // Clear screen line 2 and following, because the numbers freak otherwise, when z value is updated.
    // Don't clear line 0, because clearing is broke on line 0.
    jo_clear_screen_line(2);
    jo_clear_screen_line(3);
    jo_clear_screen_line(4);
    jo_clear_screen_line(5);
    jo_clear_screen_line(6);
    jo_printf(0, 0, "Press Start to reset all values.");
    jo_printf(0, 2, "Z-depth (press C or Z): %d", z);
    jo_printf(0, 3, "Cycle images with A & B: Img. Nr.: %d", spriteIndex);
    jo_printf(0, 4, "Rotat. on x (press Up or Down): %d", rot_x);
    jo_printf(0, 5, "Rotat. on y (press Left or Right): %d", rot_y);
    jo_printf(0, 6, "Rotat. on z (press L or R trigger): %d", rot_z);
}

jo_palette      *my_tga_palette_handling(void)
{
    if(img_loaded == a_dot_tga)
        {
            jo_create_palette(&palette1);
            return (&palette1);
        }
    else if(img_loaded == b_dot_tga)
        {
            jo_create_palette(&palette2);
            return (&palette2);
        }
    else if(img_loaded == c_dot_tga)
        {
            jo_create_palette(&palette3);
            return (&palette3);
        }
    else if(img_loaded == d_dot_tga)
        {
            jo_create_palette(&palette4);
            return (&palette4);
        }
    else if(img_loaded == e_dot_tga)
        {
            jo_create_palette(&palette5);
            return (&palette5);
        }
    else if(img_loaded == f_dot_tga)
        {
            jo_create_palette(&palette6);
            return (&palette6);
        }
}

/** @brief Load assets
 */
void LoadAssets()
{
    jo_core_tv_off();

    /* Not using RGB planes.
    // set palette loading
    jo_set_tga_palette_handling(Rgb0PaletteHandling);

    // Enable RGB0
    jo_enable_background_3d_plane(JO_COLOR_RGB(71,243,255));
    
    // Load water layer texture
    jo_img_8bits img;
    img.data = JO_NULL;
    jo_tga_8bits_loader(&img, JO_ROOT_DIR, "WATER.TGA", 0);
    jo_background_3d_plane_a_img(&img, Rgb0Palette.id, true, true);
    jo_background_3d_plane_b_img(&img, Rgb0Palette.id, true, true);
    jo_free_img(&img);
    */

	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
    int entityIds = 0;

    // Load island model
    IslandInitialize(currentAddress, &entityIds, &Island);

    jo_core_tv_on();
}

void            add_image_files(void)
{
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "A.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    img_loaded = b_dot_tga;
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "B.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    img_loaded = c_dot_tga;
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "C.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    img_loaded = d_dot_tga;
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "D.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    img_loaded = e_dot_tga;
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "E.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    img_loaded = f_dot_tga;
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "F.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);

    LoadAssets();
    // add_image_files();
    jo_core_add_callback(my_input);
    jo_core_add_callback(my_draw);

	jo_core_run();
}