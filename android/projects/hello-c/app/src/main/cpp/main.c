#include <android/log.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LOG_TAG "HelloC"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef struct {
    ANativeActivity *activity;
    ANativeWindow *window;
    pthread_mutex_t lock;
    int running;
    int sensor_available;
    float pulse_bpm;
} AppState;

static pthread_mutex_t pulse_lock = PTHREAD_MUTEX_INITIALIZER;
static int shared_sensor_available = 0;
static float shared_pulse_bpm = 0.0f;
static char shared_status[64] = "waiting";
static AppState *foreground_state = NULL;

static const char *glyph_for(char ch) {
    switch (ch) {
        case 'H': return "10001"
                         "10001"
                         "10001"
                         "11111"
                         "10001"
                         "10001"
                         "10001";
        case 'E': return "11111"
                         "10000"
                         "10000"
                         "11110"
                         "10000"
                         "10000"
                         "11111";
        case 'L': return "10000"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "11111";
        case 'O': return "01110"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "01110";
        case 'C': return "01111"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "01111";
        case 'P': return "11110"
                         "10001"
                         "10001"
                         "11110"
                         "10000"
                         "10000"
                         "10000";
        case 'U': return "10001"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "01110";
        case 'S': return "01111"
                         "10000"
                         "10000"
                         "01110"
                         "00001"
                         "00001"
                         "11110";
        case 'B': return "11110"
                         "10001"
                         "10001"
                         "11110"
                         "10001"
                         "10001"
                         "11110";
        case 'M': return "10001"
                         "11011"
                         "10101"
                         "10101"
                         "10001"
                         "10001"
                         "10001";
        case '-': return "00000"
                         "00000"
                         "00000"
                         "11111"
                         "00000"
                         "00000"
                         "00000";
        case '0': return "01110"
                         "10001"
                         "10011"
                         "10101"
                         "11001"
                         "10001"
                         "01110";
        case '1': return "00100"
                         "01100"
                         "00100"
                         "00100"
                         "00100"
                         "00100"
                         "01110";
        case '2': return "01110"
                         "10001"
                         "00001"
                         "00010"
                         "00100"
                         "01000"
                         "11111";
        case '3': return "11110"
                         "00001"
                         "00001"
                         "01110"
                         "00001"
                         "00001"
                         "11110";
        case '4': return "00010"
                         "00110"
                         "01010"
                         "10010"
                         "11111"
                         "00010"
                         "00010";
        case '5': return "11111"
                         "10000"
                         "10000"
                         "11110"
                         "00001"
                         "00001"
                         "11110";
        case '6': return "01110"
                         "10000"
                         "10000"
                         "11110"
                         "10001"
                         "10001"
                         "01110";
        case '7': return "11111"
                         "00001"
                         "00010"
                         "00100"
                         "01000"
                         "01000"
                         "01000";
        case '8': return "01110"
                         "10001"
                         "10001"
                         "01110"
                         "10001"
                         "10001"
                         "01110";
        case '9': return "01110"
                         "10001"
                         "10001"
                         "01111"
                         "00001"
                         "00001"
                         "01110";
        default: return "00000"
                        "00000"
                        "00000"
                        "00000"
                        "00000"
                        "00000"
                        "00000";
    }
}

static void put_rect(uint32_t *pixels, int stride, int width, int height, int x, int y, int w, int h, uint32_t color) {
    for (int yy = y; yy < y + h; yy++) {
        if (yy < 0 || yy >= height) continue;
        uint32_t *row = pixels + yy * stride;
        for (int xx = x; xx < x + w; xx++) {
            if (xx >= 0 && xx < width) row[xx] = color;
        }
    }
}

static void draw_char(uint32_t *pixels, int stride, int width, int height, int x, int y, int scale, char ch, uint32_t color) {
    const char *glyph = glyph_for(ch);
    for (int gy = 0; gy < 7; gy++) {
        for (int gx = 0; gx < 5; gx++) {
            if (glyph[gy * 5 + gx] == '1') {
                put_rect(pixels, stride, width, height, x + gx * scale, y + gy * scale, scale, scale, color);
            }
        }
    }
}

static void draw_text(uint32_t *pixels, int stride, int width, int height, int x, int y, int scale, const char *text, uint32_t color) {
    for (const char *p = text; *p; p++) {
        if (*p != ' ') {
            draw_char(pixels, stride, width, height, x, y, scale, *p, color);
        }
        x += 6 * scale;
    }
}

static void draw(AppState *state) {
    if (state == NULL || state->window == NULL) return;

    ANativeWindow_setBuffersGeometry(state->window, 0, 0, WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer buffer;
    if (ANativeWindow_lock(state->window, &buffer, NULL) != 0) {
        LOGI("Could not lock native window");
        return;
    }

    uint32_t *pixels = (uint32_t *)buffer.bits;
    int width = buffer.width;
    int height = buffer.height;
    int stride = buffer.stride;
    uint32_t bg = 0xff101112;
    uint32_t fg = 0xffeeeeee;
    uint32_t accent = 0xff56b6c2;
    float pulse_bpm = 0.0f;
    int sensor_available = 0;

    pthread_mutex_lock(&pulse_lock);
    pulse_bpm = shared_pulse_bpm;
    sensor_available = shared_sensor_available;
    pthread_mutex_unlock(&pulse_lock);

    for (int y = 0; y < height; y++) {
        uint32_t *row = pixels + y * stride;
        for (int x = 0; x < width; x++) {
            row[x] = bg;
        }
    }

    int min_dim = width < height ? width : height;
    int scale_by_width = (width * 7) / (10 * 41);
    int scale_by_height = (height * 7) / (10 * 7);
    int scale_by_round_safe_area = min_dim / 64;
    int scale = scale_by_width;
    if (scale_by_height < scale) scale = scale_by_height;
    if (scale_by_round_safe_area < scale) scale = scale_by_round_safe_area;
    if (scale < 4) scale = 4;
    char pulse_text[24];
    if (!sensor_available) {
        strcpy(pulse_text, "PULSE --");
    } else if (pulse_bpm > 0.1f) {
        int bpm = (int)(pulse_bpm + 0.5f);
        snprintf(pulse_text, sizeof(pulse_text), "PULSE %d", bpm);
    } else {
        strcpy(pulse_text, "PULSE --");
    }

    int title_len = 7;
    int pulse_len = (int)strlen(pulse_text);
    int title_width = title_len * 6 * scale - scale;
    int pulse_width = pulse_len * 6 * scale - scale;
    int text_width = title_width > pulse_width ? title_width : pulse_width;
    int text_height = 16 * scale;
    int x = (width - text_width) / 2;
    int y = (height - text_height) / 2;
    int title_x = (width - title_width) / 2;
    int pulse_x = (width - pulse_width) / 2;
    int bar_height = scale / 2 + 1;
    int bar_y = y + text_height + scale * 2;

    put_rect(pixels, stride, width, height, x, bar_y, text_width, bar_height, accent);
    draw_text(pixels, stride, width, height, title_x, y, scale, "HELLO C", fg);
    draw_text(pixels, stride, width, height, pulse_x, y + 9 * scale, scale, pulse_text, sensor_available ? accent : fg);

    ANativeWindow_unlockAndPost(state->window);
}

JNIEXPORT void JNICALL Java_dev_nybo_helloc_HealthSdkBinder_nativeOnPulse(JNIEnv *env, jclass clazz, jdouble bpm, jlong timestamp_ms) {
    (void)env;
    (void)clazz;
    (void)timestamp_ms;
    pthread_mutex_lock(&pulse_lock);
    shared_pulse_bpm = (float)bpm;
    shared_sensor_available = 1;
    snprintf(shared_status, sizeof(shared_status), "health sdk binder");
    pthread_mutex_unlock(&pulse_lock);
    LOGI("Pulse %.1f bpm", (double)bpm);
    draw(foreground_state);
}

JNIEXPORT void JNICALL Java_dev_nybo_helloc_HealthSdkBinder_nativeOnHealthStatus(JNIEnv *env, jclass clazz, jstring status) {
    (void)clazz;
    const char *chars = status ? (*env)->GetStringUTFChars(env, status, NULL) : NULL;
    pthread_mutex_lock(&pulse_lock);
    if (chars != NULL) {
        snprintf(shared_status, sizeof(shared_status), "%s", chars);
    }
    if (chars != NULL && strstr(chars, "missing") != NULL) {
        shared_sensor_available = 0;
    }
    pthread_mutex_unlock(&pulse_lock);
    if (chars != NULL) {
        LOGI("Health status: %s", chars);
        (*env)->ReleaseStringUTFChars(env, status, chars);
    }
    draw(foreground_state);
}

static void on_native_window_created(ANativeActivity *activity, ANativeWindow *window) {
    AppState *state = (AppState *)activity->instance;
    state->window = window;
    foreground_state = state;
    LOGI("Native window created");
    draw(state);
}

static void on_native_window_redraw_needed(ANativeActivity *activity, ANativeWindow *window) {
    AppState *state = (AppState *)activity->instance;
    state->window = window;
    draw(state);
}

static void on_native_window_destroyed(ANativeActivity *activity, ANativeWindow *window) {
    (void)window;
    AppState *state = (AppState *)activity->instance;
    state->window = NULL;
    if (foreground_state == state) {
        foreground_state = NULL;
    }
}

static void on_destroy(ANativeActivity *activity) {
    AppState *state = (AppState *)activity->instance;
    if (state != NULL) {
        pthread_mutex_destroy(&state->lock);
        if (foreground_state == state) {
            foreground_state = NULL;
        }
    }
    free(state);
    activity->instance = NULL;
}

void ANativeActivity_onCreate(ANativeActivity *activity, void *saved_state, size_t saved_state_size) {
    (void)saved_state;
    (void)saved_state_size;

    AppState *state = calloc(1, sizeof(AppState));
    state->activity = activity;
    state->running = 1;
    pthread_mutex_init(&state->lock, NULL);
    activity->instance = state;

    activity->callbacks->onNativeWindowCreated = on_native_window_created;
    activity->callbacks->onNativeWindowRedrawNeeded = on_native_window_redraw_needed;
    activity->callbacks->onNativeWindowDestroyed = on_native_window_destroyed;
    activity->callbacks->onDestroy = on_destroy;
}
