#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct Settings {
    int resWidth;
    int resHeight;
    int fullscreen;
    int use_native_pc_speaker;
} Settings;

extern Settings settings;
extern int settings_changed;

void read_settings(void);
void write_settings(void);

#endif // SETTINGS_H