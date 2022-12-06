#include <jo/jo.h>

// Image variables.
static jo_palette      palette1;

// Values for rotation. Base matrix rotation value and rotation frustrum.
int rot_x = 0;
int rot_y = 0;
int rot_z = 0;
int rot_neg_frust = -90;
int rot_pos_frust = 90;
int neg_frust_ext = -180;
int pos_frust_ext = 180;

// Values for z-depth.
#define Z_BASE 75
int z = Z_BASE;
int far = 1000;
int near = 200;
int frustrum = 20;

void			my_draw(void)
{
    if (jo_is_pad1_key_pressed(JO_KEY_C))
    {
        
        z+=z/25 +1;
        
        
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
        if (z>20){
        z-=z/25 +1;
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
        rot_y--;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_LEFT) && rot_y < rot_neg_frust)
    {
        rot_y++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && rot_y < rot_pos_frust)
    {
        rot_y++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && rot_y > rot_pos_frust)
    {
        rot_y--;
    }

    // Axis on the line of the camera / depth. Tilts image left and right.
    if (jo_is_pad1_key_pressed(JO_KEY_L) && rot_z > neg_frust_ext)
    {
        rot_z--;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_L) && rot_z < neg_frust_ext)
    {
        rot_z++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_R) && rot_z < pos_frust_ext)
    {
        rot_z++;
    }
    else if (jo_is_pad1_key_pressed(JO_KEY_R) && rot_z > pos_frust_ext)
    {
        rot_z--;
    }

    // Reset all values.
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
    rot_x = 0;
    rot_y = 0;
    rot_z = 0;
    z = Z_BASE;
    }
    
    // Setting palette every frame seems not to matter.
    // Push matrix, apply transformations from button inputs, draw sprite, pop matrix.
    jo_sprite_set_palette(palette1.id);
    jo_3d_push_matrix();
    jo_3d_translate_matrix_z(z);
    jo_3d_rotate_matrix_x(rot_x);
    jo_3d_rotate_matrix_y(rot_y);
    jo_3d_rotate_matrix_z(rot_z);
    jo_3d_draw_sprite(0);
    jo_3d_pop_matrix();

    // Clear screen line 2-5, because the numbers freak otherwise, when z value is updated.
    // Don't clear line 0, because clearing is broke on line 0.
    jo_clear_screen_line(2);
    jo_clear_screen_line(3);
    jo_clear_screen_line(4);
    jo_clear_screen_line(5);
    jo_printf(0, 0, "Press Start to reset all values.");
    jo_printf(0, 2, "Z-depth (press C or Z): %d", z);
    jo_printf(0, 3, "Rotat. on x (press Up or Down): %d", rot_x);
    jo_printf(0, 4, "Rotat. on y (press Left or Right): %d", rot_y);
    jo_printf(0, 5, "Rotat. on z (press L or R trigger): %d", rot_z);
}

jo_palette      *my_tga_palette_handling(void)
{
    jo_create_palette(&palette1);
    return (&palette1);
}

void            add_image_files(void)
{
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "A.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "B.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);
/*
    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "C.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "D.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);

    jo_set_tga_palette_handling(my_tga_palette_handling);
    jo_sprite_add_tga(JO_ROOT_DIR, "E.TGA", 1);
    jo_set_tga_palette_handling(JO_NULL);
*/
}
void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);

    add_image_files();

    jo_core_add_callback(my_draw);
	jo_core_run();
}