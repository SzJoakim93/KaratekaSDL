#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#pragma pack(push, 1)
typedef struct {
	unsigned char fig_id;
	unsigned short x_pos;
	unsigned char y_pos;
} RenderEntry;
#pragma pack(pop)

void render(void);
void renderBG(int bg);

#endif
