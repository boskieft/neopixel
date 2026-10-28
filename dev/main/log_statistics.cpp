/*
*******************************************************************************
    Check and log the statistics of the Neopixel Driver

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "esp_log.h"
#include "config_neopixels.h"

static const char *TAG = "STAT";

/*
*******************************************************************************
    Check and log the Neopixel statistics
*******************************************************************************
*/
void logStatistics(void) {

    //---------------------------------------------------------------------------------------------
    // Maximum time taken to send data to the Neopixels
    //
    // Initially short (approx 200 us)
    // Will increase when you try to send faster than the Neopixel transmission rate,
    // because the Driver needs to wait for the previous transmission to complete
    //---------------------------------------------------------------------------------------------
    if (esp_log_level_get("I2S_") < ESP_LOG_DEBUG) {
        // Driver is not logging it, do it here
        static int64_t loggedMaxSendMicros = 0;
        if (npx.stats.maxSendMicros > loggedMaxSendMicros) {
            loggedMaxSendMicros = npx.stats.maxSendMicros;
            ESP_LOGI(TAG, "maxSendMicros=%" PRId64, loggedMaxSendMicros);
        }
    } // else: don't log twice

    //---------------------------------------------------------------------------------------------
    // Used chunks
    //
    // Usually same as the configured number of chunks (2 or more)
    // Will increase on certain ESP32xx variants when sending fast to a small number of Neopixels,
    // because the Driver may need to send multiple dummy flush chunks until it is done
    //---------------------------------------------------------------------------------------------
    static uint32_t loggedMaxChunksSent = 0;
    if (npx.stats.maxNrChunksSent > loggedMaxChunksSent) {
        loggedMaxChunksSent = npx.stats.maxNrChunksSent;
        ESP_LOGI(TAG, "maxNrChunksSent=%" PRIu32, loggedMaxChunksSent);
    }

    //---------------------------------------------------------------------------------------------
    // Preload data errors
    //
    // Never seen this happen in practice
    //---------------------------------------------------------------------------------------------
    static uint32_t loggedNrPreloadDataErrors = 0;
    if (npx.stats.nrPreloadDataErrors > loggedNrPreloadDataErrors) {
        loggedNrPreloadDataErrors = npx.stats.nrPreloadDataErrors;
        ESP_LOGI(TAG, "nrPreloadDataErrors=%" PRIu32, loggedNrPreloadDataErrors);
    }
    //---------------------------------------------------------------------------------------------
    // Channel errors
    //
    // Never seen this happen in practice
    //---------------------------------------------------------------------------------------------
    static uint32_t loggedNrChannelErrors = 0;
    if (npx.stats.nrChannelErrors > loggedNrChannelErrors) {
        loggedNrChannelErrors = npx.stats.nrChannelErrors;
        ESP_LOGI(TAG, "nrChannelErrors=%" PRIu32, loggedNrChannelErrors);
    }

    //---------------------------------------------------------------------------------------------
    // Timeouts
    //
    // Only seen this one happen in case the dataPin was not configured correctly
    //---------------------------------------------------------------------------------------------
    static uint32_t loggedNrTimeouts = 0;
    if (npx.stats.nrTimeouts > loggedNrTimeouts) {
        loggedNrTimeouts = npx.stats.nrTimeouts;
        ESP_LOGI(TAG, "nrTimeouts=%" PRIu32, loggedNrTimeouts);
    }

#if (ENABLE_I2S_TASK_VERSION)
    //---------------------------------------------------------------------------------------------
    // Free room in the Task stack
    //
    // Should remain at least 100 bytes, ESP32xx will reboot when the stack is exhausted
    // Depends a lot on the ESP32xx variant, Task on ESP32-S3 uses the most stack
    //---------------------------------------------------------------------------------------------
    static UBaseType_t loggedMinFreeTaskStack = UINT_MAX;
    if (npx.stats.minimumFreeStack < loggedMinFreeTaskStack) {
        loggedMinFreeTaskStack = npx.stats.minimumFreeStack;
        ESP_LOGI(TAG, "minimumFreeStack=%u", loggedMinFreeTaskStack);
    }
#endif
}
