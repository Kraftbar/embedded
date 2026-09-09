#include <android/looper.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/sensor.h>
#include <jni.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LOG_TAG "HelloCPure"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define SENSOR_IDENT 42
#define HEART_RATE_DELAY_US 60000000

typedef struct {
    ANativeActivity *activity;
    ANativeWindow *window;
    pthread_mutex_t lock;
    pthread_t sensor_thread;
    int running;
    int sensor_available;
    int sample_count;
    float pulse_bpm;
    int64_t last_sample_ms;
} AppState;

static const char *glyph_for(char ch) {
    switch (ch) {
        case 'C': return "01111"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "10000"
                         "01111";
        case 'H': return "10001"
                         "10001"
                         "10001"
                         "11111"
                         "10001"
                         "10001"
                         "10001";
        case 'P': return "11110"
                         "10001"
                         "10001"
                         "11110"
                         "10000"
                         "10000"
                         "10000";
        case 'R': return "11110"
                         "10001"
                         "10001"
                         "11110"
                         "10100"
                         "10010"
                         "10001";
        case 'U': return "10001"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "10001"
                         "01110";
        case 'E': return "11111"
                         "10000"
                         "10000"
                         "11110"
                         "10000"
                         "10000"
                         "11111";
        case 'S': return "01111"
                         "10000"
                         "10000"
                         "01110"
                         "00001"
                         "00001"
                         "11110";
        case 'M': return "10001"
                         "11011"
                         "10101"
                         "10101"
                         "10001"
                         "10001"
                         "10001";
        case 'N': return "10001"
                         "11001"
                         "10101"
                         "10011"
                         "10001"
                         "10001"
                         "10001";
        case 'T': return "11111"
                         "00100"
                         "00100"
                         "00100"
                         "00100"
                         "00100"
                         "00100";
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

static int64_t now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
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
    if (ANativeWindow_lock(state->window, &buffer, NULL) != 0) return;

    uint32_t *pixels = (uint32_t *)buffer.bits;
    int width = buffer.width;
    int height = buffer.height;
    int stride = buffer.stride;
    uint32_t bg = 0xff101112;
    uint32_t fg = 0xffeeeeee;
    uint32_t accent = 0xff98c379;

    float bpm;
    int available;
    int count;
    pthread_mutex_lock(&state->lock);
    bpm = state->pulse_bpm;
    available = state->sensor_available;
    count = state->sample_count;
    pthread_mutex_unlock(&state->lock);

    for (int y = 0; y < height; y++) {
        uint32_t *row = pixels + y * stride;
        for (int x = 0; x < width; x++) row[x] = bg;
    }

    int min_dim = width < height ? width : height;
    int scale = min_dim / 68;
    if (scale < 4) scale = 4;

    char pulse_text[24];
    char count_text[24];
    if (available && bpm > 0.1f) {
        snprintf(pulse_text, sizeof(pulse_text), "PULSE %d", (int)(bpm + 0.5f));
    } else {
        strcpy(pulse_text, "PULSE --");
    }
    snprintf(count_text, sizeof(count_text), "CNT %d", count);

    const char *title = "PURE C";
    int title_w = (int)strlen(title) * 6 * scale - scale;
    int pulse_w = (int)strlen(pulse_text) * 6 * scale - scale;
    int count_w = (int)strlen(count_text) * 6 * scale - scale;
    int max_w = title_w;
    if (pulse_w > max_w) max_w = pulse_w;
    if (count_w > max_w) max_w = count_w;
    int block_h = 25 * scale;
    int y = (height - block_h) / 2;

    draw_text(pixels, stride, width, height, (width - title_w) / 2, y, scale, title, fg);
    draw_text(pixels, stride, width, height, (width - pulse_w) / 2, y + 9 * scale, scale, pulse_text, available ? accent : fg);
    draw_text(pixels, stride, width, height, (width - count_w) / 2, y + 18 * scale, scale, count_text, fg);
    put_rect(pixels, stride, width, height, (width - max_w) / 2, y + 26 * scale, max_w, scale / 2 + 1, accent);

    ANativeWindow_unlockAndPost(state->window);
}

static void *sensor_thread_main(void *arg) {
    AppState *state = (AppState *)arg;
    JavaVM *vm = state->activity->vm;
    JNIEnv *env = NULL;
    jclass activity_class = NULL;
    jmethodID pulse_method = NULL;
    int jni_attached = 0;
    if (vm != NULL && (*vm)->AttachCurrentThread(vm, &env, NULL) == JNI_OK) {
        jni_attached = 1;
        jclass local_class = (*env)->FindClass(env, "dev/nybo/hellocpure/MainActivity");
        if (local_class != NULL) {
            activity_class = (*env)->NewGlobalRef(env, local_class);
            (*env)->DeleteLocalRef(env, local_class);
            pulse_method = (*env)->GetStaticMethodID(env, activity_class, "nativeOnPulse", "(D)V");
        }
        if ((*env)->ExceptionCheck(env)) {
            (*env)->ExceptionClear(env);
            activity_class = NULL;
            pulse_method = NULL;
        }
    }
    ALooper *looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ASensorManager *manager = ASensorManager_getInstance();
    const ASensor *heart = manager ? ASensorManager_getDefaultSensor(manager, ASENSOR_TYPE_HEART_RATE) : NULL;
    if (manager == NULL || heart == NULL) {
        pthread_mutex_lock(&state->lock);
        state->sensor_available = 0;
        pthread_mutex_unlock(&state->lock);
        LOGI("No C heart-rate sensor");
        if (activity_class != NULL && env != NULL) {
            (*env)->DeleteGlobalRef(env, activity_class);
        }
        if (jni_attached) {
            (*vm)->DetachCurrentThread(vm);
        }
        return NULL;
    }

    ASensorEventQueue *queue = ASensorManager_createEventQueue(manager, looper, SENSOR_IDENT, NULL, NULL);
    if (queue == NULL) {
        LOGI("Could not create C sensor queue");
        if (activity_class != NULL && env != NULL) {
            (*env)->DeleteGlobalRef(env, activity_class);
        }
        if (jni_attached) {
            (*vm)->DetachCurrentThread(vm);
        }
        return NULL;
    }

    int enabled = ASensorEventQueue_enableSensor(queue, heart);
    int delay = ASensorEventQueue_setEventRate(queue, heart, HEART_RATE_DELAY_US);
    pthread_mutex_lock(&state->lock);
    state->sensor_available = enabled == 0;
    pthread_mutex_unlock(&state->lock);
    LOGI("C heart-rate sensor started enabled=%d delay=%d", enabled, delay);

    while (state->running) {
        int ident = ALooper_pollOnce(1000, NULL, NULL, NULL);
        if (ident != SENSOR_IDENT && ident != ALOOPER_POLL_WAKE) {
            continue;
        }
        ASensorEvent event;
        while (ASensorEventQueue_getEvents(queue, &event, 1) > 0) {
            if (event.type != ASENSOR_TYPE_HEART_RATE) {
                continue;
            }
            pthread_mutex_lock(&state->lock);
            state->pulse_bpm = event.heart_rate.bpm;
            state->sample_count++;
            state->last_sample_ms = now_ms();
            state->sensor_available = 1;
            pthread_mutex_unlock(&state->lock);
            LOGI("C pulse %.1f bpm accuracy=%d samples=%d", (double)event.heart_rate.bpm, event.heart_rate.status, state->sample_count);
            if (env != NULL && activity_class != NULL && pulse_method != NULL) {
                (*env)->CallStaticVoidMethod(env, activity_class, pulse_method, (jdouble)event.heart_rate.bpm);
                if ((*env)->ExceptionCheck(env)) {
                    (*env)->ExceptionClear(env);
                }
            }
            draw(state);
        }
    }

    ASensorEventQueue_disableSensor(queue, heart);
    ASensorManager_destroyEventQueue(manager, queue);
    if (activity_class != NULL && env != NULL) {
        (*env)->DeleteGlobalRef(env, activity_class);
    }
    if (jni_attached) {
        (*vm)->DetachCurrentThread(vm);
    }
    LOGI("C heart-rate sensor stopped");
    return NULL;
}

static void on_native_window_created(ANativeActivity *activity, ANativeWindow *window) {
    AppState *state = (AppState *)activity->instance;
    state->window = window;
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
}

static void on_destroy(ANativeActivity *activity) {
    AppState *state = (AppState *)activity->instance;
    if (state != NULL) {
        state->running = 0;
        pthread_join(state->sensor_thread, NULL);
        pthread_mutex_destroy(&state->lock);
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

    int err = pthread_create(&state->sensor_thread, NULL, sensor_thread_main, state);
    if (err != 0) {
        LOGI("Could not start C sensor thread: %d", err);
    }
}
