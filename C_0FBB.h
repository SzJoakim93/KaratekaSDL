#ifndef C_0FBB_H
#define C_0FBB_H

int load_background_graphics(char *fname, int bp14);
void load_castle_bg(void);
void load_sprite_assets(int idx);
int load_animation_script(char *path, char *bp6c);
void parse_script_line(char *bp16, char *bp18, int *bp1a, int *bp1c);
int open_file_safe(char *fname, int attr);

#endif