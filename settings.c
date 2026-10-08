#include "settings.h"

#include <stdio.h>

Settings settings = {
    .resWidth = 640,
    .resHeight = 480,
    .fullscreen = 0,
    .use_native_pc_speaker = 0
};

int settings_changed = 0; /* Flag to indicate if settings have changed and need to be saved */

void read_settings(void)
{
    FILE *file = fopen("settings.ini", "r");
    if (file) {
        fscanf(file, "resWidth=%d\n", &settings.resWidth);
        fscanf(file, "resHeight=%d\n", &settings.resHeight);
        fscanf(file, "fullscreen=%d\n", &settings.fullscreen);
        fscanf(file, "use_native_pc_speaker=%d\n", &settings.use_native_pc_speaker);
        fclose(file);
    } else {
        /* If the settings file doesn't exist, write default settings */
        write_settings();
    }
}

void write_settings(void)
{
    FILE *file = fopen("settings.ini", "w");
    if (file) {
        fprintf(file, "resWidth=%d\n", settings.resWidth);
        fprintf(file, "resHeight=%d\n", settings.resHeight);
        fprintf(file, "fullscreen=%d\n", settings.fullscreen);
        fprintf(file, "use_native_pc_speaker=%d\n", settings.use_native_pc_speaker);
        fclose(file);
    }
}