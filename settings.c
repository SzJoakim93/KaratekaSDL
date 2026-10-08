#include "settings.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

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

int write_settings(void)
{
    FILE *file = fopen("settings.ini", "w");
    if (!file) {
        fprintf(stderr, "Could not open settings.ini for writing: %s\n", strerror(errno));
        return 0;
    }

    if (fprintf(file, "resWidth=%d\n", settings.resWidth) < 0 ||
        fprintf(file, "resHeight=%d\n", settings.resHeight) < 0 ||
        fprintf(file, "fullscreen=%d\n", settings.fullscreen) < 0 ||
        fprintf(file, "use_native_pc_speaker=%d\n", settings.use_native_pc_speaker) < 0) {
        fprintf(stderr, "Could not write settings.ini\n");
        fclose(file);
        return 0;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Could not finish writing settings.ini: %s\n", strerror(errno));
        return 0;
    }
    return 1;
}