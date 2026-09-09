#include <android/bitmap.h>
#include <jni.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const char *glyph_for(char ch) {
    switch (ch) {
        case 'A': return "01110""10001""10001""11111""10001""10001""10001";
        case 'B': return "11110""10001""10001""11110""10001""10001""11110";
        case 'C': return "01111""10000""10000""10000""10000""10000""01111";
        case 'H': return "10001""10001""10001""11111""10001""10001""10001";
        case 'P': return "11110""10001""10001""11110""10000""10000""10000";
        case 'R': return "11110""10001""10001""11110""10100""10010""10001";
        case 'T': return "11111""00100""00100""00100""00100""00100""00100";
        case 'W': return "10001""10001""10001""10101""10101""11011""10001";
        case ':': return "00000""00100""00100""00000""00100""00100""00000";
        case '%': return "10001""00010""00100""01000""10000""00000""10001";
        case '-': return "00000""00000""00000""11111""00000""00000""00000";
        case '0': return "01110""10001""10011""10101""11001""10001""01110";
        case '1': return "00100""01100""00100""00100""00100""00100""01110";
        case '2': return "01110""10001""00001""00010""00100""01000""11111";
        case '3': return "11110""00001""00001""01110""00001""00001""11110";
        case '4': return "00010""00110""01010""10010""11111""00010""00010";
        case '5': return "11111""10000""10000""11110""00001""00001""11110";
        case '6': return "01110""10000""10000""11110""10001""10001""01110";
        case '7': return "11111""00001""00010""00100""01000""01000""01000";
        case '8': return "01110""10001""10001""01110""10001""10001""01110";
        case '9': return "01110""10001""10001""01111""00001""00001""01110";
        default: return "00000""00000""00000""00000""00000""00000""00000";
    }
}

static void rect(uint32_t *pixels, int width, int height, int x, int y, int w, int h, uint32_t color) {
    for (int yy = y; yy < y + h; yy++) {
        if (yy < 0 || yy >= height) continue;
        uint32_t *row = pixels + yy * width;
        for (int xx = x; xx < x + w; xx++) {
            if (xx >= 0 && xx < width) row[xx] = color;
        }
    }
}

static void slanted_bar(uint32_t *pixels, int width, int height, int cx, int cy, int length, int thick, int rise, uint32_t color) {
    for (int i = -length / 2; i <= length / 2; i++) {
        int y = cy - (i * rise) / length;
        rect(pixels, width, height, cx + i - thick / 2, y - thick / 2, thick, thick, color);
    }
}

static void fill_poly(uint32_t *pixels, int width, int height, const int *xy, int count, uint32_t color) {
    int min_y = xy[1];
    int max_y = xy[1];
    for (int i = 1; i < count; i++) {
        int y = xy[i * 2 + 1];
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    for (int y = min_y; y <= max_y; y++) {
        int nodes[16];
        int node_count = 0;
        int j = count - 1;
        for (int i = 0; i < count; i++) {
            int xi = xy[i * 2];
            int yi = xy[i * 2 + 1];
            int xj = xy[j * 2];
            int yj = xy[j * 2 + 1];
            if ((yi < y && yj >= y) || (yj < y && yi >= y)) {
                if (node_count < 16) {
                    nodes[node_count++] = xi + (y - yi) * (xj - xi) / (yj - yi);
                }
            }
            j = i;
        }
        for (int i = 0; i < node_count - 1; i++) {
            for (int k = i + 1; k < node_count; k++) {
                if (nodes[i] > nodes[k]) {
                    int tmp = nodes[i];
                    nodes[i] = nodes[k];
                    nodes[k] = tmp;
                }
            }
        }
        for (int i = 0; i < node_count; i += 2) {
            if (i + 1 >= node_count) break;
            rect(pixels, width, height, nodes[i], y, nodes[i + 1] - nodes[i] + 1, 1, color);
        }
    }
}

static void circle(uint32_t *pixels, int width, int height, int cx, int cy, int radius, uint32_t color) {
    int r2 = radius * radius;
    for (int y = cy - radius; y <= cy + radius; y++) {
        if (y < 0 || y >= height) continue;
        uint32_t *row = pixels + y * width;
        for (int x = cx - radius; x <= cx + radius; x++) {
            if (x < 0 || x >= width) continue;
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy <= r2) row[x] = color;
        }
    }
}

static void ring(uint32_t *pixels, int width, int height, int cx, int cy, int outer, int inner, uint32_t color) {
    int outer2 = outer * outer;
    int inner2 = inner * inner;
    for (int y = cy - outer; y <= cy + outer; y++) {
        if (y < 0 || y >= height) continue;
        uint32_t *row = pixels + y * width;
        for (int x = cx - outer; x <= cx + outer; x++) {
            if (x < 0 || x >= width) continue;
            int dx = x - cx;
            int dy = y - cy;
            int d2 = dx * dx + dy * dy;
            if (d2 <= outer2 && d2 >= inner2) row[x] = color;
        }
    }
}

static void draw_char(uint32_t *pixels, int width, int height, int x, int y, int scale, char ch, uint32_t color) {
    const char *glyph = glyph_for(ch);
    for (int gy = 0; gy < 7; gy++) {
        for (int gx = 0; gx < 5; gx++) {
            if (glyph[gy * 5 + gx] == '1') {
                rect(pixels, width, height, x + gx * scale, y + gy * scale, scale, scale, color);
            }
        }
    }
}

static int text_width(const char *text, int scale) {
    int len = (int)strlen(text);
    return len > 0 ? len * 6 * scale - scale : 0;
}

static void draw_text(uint32_t *pixels, int width, int height, int x, int y, int scale, const char *text, uint32_t color) {
    for (const char *p = text; *p; p++) {
        if (*p != ' ') draw_char(pixels, width, height, x, y, scale, *p, color);
        x += 6 * scale;
    }
}

JNIEXPORT void JNICALL
Java_dev_nybo_purewatchface_PureCWatchFaceService_render(
        JNIEnv *env,
        jclass clazz,
        jobject bitmap,
        jint width,
        jint height,
        jint hour,
        jint minute,
        jint second,
        jint battery_pct,
        jdouble bpm,
        jboolean ambient) {
    (void)env;
    (void)clazz;

    uint32_t *pixels = NULL;
    if (AndroidBitmap_lockPixels(env, bitmap, (void **)&pixels) != 0 || pixels == NULL) return;

    uint32_t bg = ambient ? 0xff000000 : 0xff101112;
    uint32_t fg = ambient ? 0xff9a9a9a : 0xffeeeeee;
    uint32_t dim = ambient ? 0xff5d5d5d : 0xff8ea3a6;
    uint32_t accent = ambient ? 0xff767676 : 0xff98c379;

    for (int y = 0; y < height; y++) {
        uint32_t *row = pixels + y * width;
        for (int x = 0; x < width; x++) row[x] = bg;
    }

    int min_dim = width < height ? width : height;
    int cx = width / 2;
    int cy = height / 2;
    uint32_t cross = ambient ? 0xff171717 : 0xff202522;
    uint32_t cross_core = ambient ? 0xff1e1e1e : 0xff2a312c;
    int stem_h = min_dim * 78 / 100;
    int stem_w = min_dim * 15 / 100;
    int arm_w = min_dim * 72 / 100;
    int arm_h = min_dim * 15 / 100;
    int arm_y = cy - min_dim * 10 / 100;
    rect(pixels, width, height, cx - stem_w / 2, cy - stem_h / 2, stem_w, stem_h, cross);
    rect(pixels, width, height, cx - arm_w / 2, arm_y - arm_h / 2, arm_w, arm_h, cross);
    rect(pixels, width, height, cx - stem_w / 2 + 3, cy - stem_h / 2 + 3, stem_w - 6, stem_h - 6, cross_core);
    rect(pixels, width, height, cx - arm_w / 2 + 3, arm_y - arm_h / 2 + 3, arm_w - 6, arm_h - 6, cross_core);

    int time_scale = min_dim / 78;
    if (time_scale < 4) time_scale = 4;
    int small_scale = min_dim / 118;
    if (small_scale < 3) small_scale = 3;

    char time_text[16];
    snprintf(time_text, sizeof(time_text), "%02d:%02d", hour, minute);

    char pulse_text[24];
    if (bpm > 0.1) {
        snprintf(pulse_text, sizeof(pulse_text), "HR %d", (int)(bpm + 0.5));
    } else {
        snprintf(pulse_text, sizeof(pulse_text), "HR --");
    }

    char battery_text[24];
    if (battery_pct >= 0) {
        snprintf(battery_text, sizeof(battery_text), "BAT %d%%", battery_pct);
    } else {
        snprintf(battery_text, sizeof(battery_text), "BAT --");
    }

    int time_w = text_width(time_text, time_scale);
    int pulse_w = text_width(pulse_text, small_scale);
    int battery_w = text_width(battery_text, small_scale);
    int time_y = height - min_dim * 22 / 100;
    int pulse_y = height - min_dim * 13 / 100;
    int battery_y = height - min_dim * 7 / 100;

    draw_text(pixels, width, height, (width - time_w) / 2, time_y, time_scale, time_text, fg);
    draw_text(pixels, width, height, (width - pulse_w) / 2, pulse_y, small_scale, pulse_text, accent);
    draw_text(pixels, width, height, (width - battery_w) / 2, battery_y, small_scale, battery_text, dim);

    AndroidBitmap_unlockPixels(env, bitmap);
}
