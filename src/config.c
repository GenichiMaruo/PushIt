#include "../include/config.h"

int port;
int max_clients;
int max_fps;
double gravity;
int max_offscreen_width;
int max_offscreen_height;

void config_init() {
    port = 50000;              // default port
    max_clients = 2;           // default max clients
    max_fps = 60;              // default max fps
    gravity = 300;             // default gravity
    max_offscreen_width = 2;   // default max offscreen width
    max_offscreen_height = 4;  // default max offscreen height
    get_config();
}

// get config from config file
void get_config() {
    // もしconfigファイルがなければ./confit/config.jsonを作成し、デフォルト値を書き込む
    // もしconfigファイルがあれば、デフォルト値を読み込む
    // この関数は、main関数の最初に呼び出される

    // configフォルダがあるかどうか確認
    if (access("./config", F_OK) == -1) {
        // configフォルダがない場合
        // configフォルダを作成する
        mkdir("./config", 0777);
    }
    // configファイルがあるかどうか確認
    if (access("./config/config.json", F_OK) == -1) {
        // configファイルがない場合
        // デフォルト値を書き込む
        FILE *fp;
        fp = fopen("./config/config.json", "w");
        fprintf(fp, "{\n");
        fprintf(fp, "    \"port\": %d,\n", port);
        fprintf(fp, "    \"max_clients\": %d,\n", max_clients);
        fprintf(fp, "    \"max_fps\": %d,\n", max_fps);
        fprintf(fp, "    \"gravity\": %.1f,\n", gravity);
        fprintf(fp, "    \"max_offscreen_width\": %d,\n", max_offscreen_width);
        fprintf(fp, "    \"max_offscreen_height\": %d\n", max_offscreen_height);
        fprintf(fp, "}\n");
        fclose(fp);
    } else {
        // configファイルがある場合
        // デフォルト値を読み込む
        FILE *fp;
        fp = fopen("./config/config.json", "r");
        char buf[256];
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            if (strstr(buf, "port") != NULL) {
                sscanf(buf, "    \"port\": %d,\n", &port);
            } else if (strstr(buf, "max_clients") != NULL) {
                sscanf(buf, "    \"max_clients\": %d,\n", &max_clients);
            } else if (strstr(buf, "max_fps") != NULL) {
                sscanf(buf, "    \"max_fps\": %d,\n", &max_fps);
            } else if (strstr(buf, "gravity") != NULL) {
                sscanf(buf, "    \"gravity\": %lf,\n", &gravity);
            } else if (strstr(buf, "max_offscreen_width") != NULL) {
                sscanf(buf, "    \"max_offscreen_width\": %d,\n",
                       &max_offscreen_width);
            } else if (strstr(buf, "max_offscreen_height") != NULL) {
                sscanf(buf, "    \"max_offscreen_height\": %d\n",
                       &max_offscreen_height);
            }
        }
        fclose(fp);
    }
}