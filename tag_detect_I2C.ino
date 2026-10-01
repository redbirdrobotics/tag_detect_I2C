/*
 * ESP32-CAM AprilTag Sensor
 *
 * Detects AprilTags from the modified Standard 41h12 family.
 *
 * Tag IDs:
 *   0 = No relevant tag detected
 *   1 = Tag 1
 *   2 = Tag 2
 *   3 = Tag 3
 *
 * Reporting modes:
 *   REPORT_SERIAL -> print detected tag over Serial
 *   REPORT_I2C    -> make detected tag available over I2C
 */

// Uncomment ONE of these:
#define REPORT_SERIAL
// #define REPORT_I2C


// Camera Model
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

// ============================================================
// I2C configuration
// ============================================================
#ifdef REPORT_I2C
#include <Wire.h>

// I2C address of this ESP32-CAM.
// The other device will use this address when requesting data.
#define I2C_ADDRESS 0x69

// ESP32-CAM AI Thinker pins.
// Change these if your hardware uses different pins.
#define I2C_SDA 14
#define I2C_SCL 15

#endif

// Camera
#include "esp_camera.h"
#include "camera_pins.h"

// AprilTag
#include "apriltag.h"
#include "tagStandard41h12.h"
#include "common/image_u8.h"
#include "common/zarray.h"

// Global state
// 0 = no relevant tag
// 1 = Tag 1
// 2 = Tag 2
// 3 = Tag 3
volatile uint8_t detectedTag = 0;

// AprilTag detector
apriltag_detector_t *detector = nullptr;
apriltag_family_t *family = nullptr;

// I2C request handler
#ifdef REPORT_I2C

void onI2CRequest() {
    // Send the currently detected tag.
    uint8_t tag = detectedTag;
    Wire.write(tag);
}

#endif

// Camera initialization
void initCamera() {

    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;

    // Grayscale is exactly what AprilTag wants.
    config.pixel_format = PIXFORMAT_GRAYSCALE;

    // VGA = 640x480.
    config.frame_size = FRAMESIZE_QVGA;

    // Only one framebuffer is necessary.
    config.fb_count = 1;

    // Put framebuffer in PSRAM.
    config.fb_location = CAMERA_FB_IN_PSRAM;

    // Grab the newest frame.
    config.grab_mode = CAMERA_GRAB_LATEST;

#ifdef REPORT_SERIAL
    Serial.print("Initializing camera... ");
#endif

    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {
#ifdef REPORT_SERIAL
        Serial.printf("FAILED (0x%x)\n", err);
#endif
        ESP.restart();
    }

#ifdef REPORT_SERIAL
    Serial.println("OK");
#endif


    // Camera-specific configuration.
    sensor_t *sensor = esp_camera_sensor_get();

    sensor->set_brightness(sensor, 0);
    sensor->set_contrast(sensor, 0);
    sensor->set_saturation(sensor, 0);

    sensor->set_whitebal(sensor, 1);
    sensor->set_awb_gain(sensor, 1);
    sensor->set_wb_mode(sensor, 0);

    sensor->set_exposure_ctrl(sensor, 1);
    sensor->set_aec2(sensor, 1);
    sensor->set_ae_level(sensor, 0);
    sensor->set_aec_value(sensor, 168);

    sensor->set_gain_ctrl(sensor, 1);
    sensor->set_agc_gain(sensor, 0);
    sensor->set_gainceiling(sensor, (gainceiling_t)0);

    sensor->set_bpc(sensor, 0);
    sensor->set_wpc(sensor, 1);
    sensor->set_raw_gma(sensor, 1);
    sensor->set_lenc(sensor, 1);

    // AI Thinker camera orientation.
    sensor->set_hmirror(sensor, 1);
    sensor->set_vflip(sensor, 1);
}

// AprilTag initialization
void initAprilTag() {

#ifdef REPORT_SERIAL
    Serial.print("Initializing AprilTag detector... ");
#endif

    //Modified family contains only tags 0-4.
    family = tagStandard41h12_create();

    detector = apriltag_detector_create();

    apriltag_detector_add_family(detector, family);

    // Detection settings.
    detector->quad_sigma = 0.0;

    // Higher = faster, but makes small/far-away tags harder to detect.
    detector->quad_decimate = 4.0;

    detector->refine_edges = 0;
    detector->decode_sharpening = 0;

    detector->nthreads = 1;
    detector->debug = 0;

#ifdef REPORT_SERIAL
    Serial.println("OK");
#endif
}

// AprilTag detection
void detectAprilTag() {

    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
        return;
    }


    // Convert camera framebuffer into AprilTag image format.
    image_u8_t image = {
        .width = fb->width,
        .height = fb->height,
        .stride = fb->width,
        .buf = fb->buf
    };


    // Run detector.
    zarray_t *detections =
        apriltag_detector_detect(detector, &image);


    // Default to "nothing detected."
    detectedTag = 0;


    // Look for tags 1, 2, or 3.
    for (int i = 0; i < zarray_size(detections); i++) {

        apriltag_detection_t *det;

        zarray_get(detections, i, &det);


        if (det->id >= 1 && det->id <= 3) {

            detectedTag = det->id;

            break;
        }
    }


    // Free detection results.
    apriltag_detections_destroy(detections);


    // Return framebuffer to camera driver.
    esp_camera_fb_return(fb);
}

// Setup
void setup() {

#ifdef REPORT_SERIAL
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println("ESP32-CAM AprilTag Sensor");
    Serial.println("================================");
#endif


    // Initialize PSRAM.
    psramInit();

#ifdef REPORT_SERIAL
    Serial.print("Free PSRAM: ");
    Serial.println(ESP.getFreePsram());
#endif


    // Initialize camera.
    initCamera();


    // Initialize AprilTag.
    initAprilTag();


    // Initialize I2C if selected.
#ifdef REPORT_I2C

    // I2C address of this ESP32-CAM.
    Wire.begin(
        I2C_ADDRESS,
        I2C_SDA,
        I2C_SCL
    );

    Wire.onRequest(onI2CRequest);

#ifdef REPORT_SERIAL
    Serial.print("I2C slave address: 0x");
    Serial.println(I2C_ADDRESS, HEX);
    Serial.println("I2C ready.");
#endif

#endif


#ifdef REPORT_SERIAL
    Serial.println("Starting detection.");
    Serial.println();
#endif
}

// Main loop
void loop() {

    unsigned long startTime = millis();

    detectAprilTag();

#ifdef REPORT_SERIAL

    Serial.print("Tag: ");
    Serial.print(detectedTag);

    Serial.print(" | Detection time: ");
    Serial.print(millis() - startTime);
    Serial.println(" ms");

#endif

    // Ensure each loop takes at least 250 ms.
    unsigned long elapsed = millis() - startTime;

    if (elapsed < 250) {
        delay(250 - elapsed);
    }
}