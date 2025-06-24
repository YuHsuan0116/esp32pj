#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define N 4

static int M1[N][N] = {{2, 0, 0, 6}, {0, 1, 1, 6}, {1, 3, 9, 0}, {1, 0, 3, 6}};
static int M2[N][N] = {{6, 0, 0, 2}, {6, 1, 1, 0}, {0, 9, 3, 1}, {6, 3, 0, 1}};
static int M3[N][N];

static int l_idx, r_idx, sum = 0;
static SemaphoreHandle_t idx_mutex, sum_mutex;

void ResetIndex() {
    l_idx = 0;
    r_idx = N * N - 1;
}

void Mult_task(void* arg) {
    int core_id = esp_cpu_get_core_id();
    while(1) {
        if(xSemaphoreTake(idx_mutex, portMAX_DELAY) == pdTRUE) {
            if(l_idx > r_idx) {
                xSemaphoreGive(idx_mutex);
                break;
            }
            int idx;
            if(core_id == 0) {
                idx = l_idx++;
            } else {
                idx = r_idx--;
            }
            xSemaphoreGive(idx_mutex);
            int row = idx / N, col = idx % N;
            int val = 0;
            printf("Task %s is processing M3[%d][%d] on Core%d\n", pcTaskGetName(NULL), row, col, core_id);
            for(int i = 0; i < N; i++) {
                val += M1[row][i] * M2[i][col];
            }
            M3[row][col] = val;
        }
    }
    vTaskDelete(NULL);
}

void Sum_task(void* arg) {
    int core_id = esp_cpu_get_core_id();
    int local_sum = 0;
    while(1) {
        if(xSemaphoreTake(idx_mutex, portMAX_DELAY) == pdTRUE) {
            if(l_idx > r_idx) {
                xSemaphoreGive(idx_mutex);
                break;
            }
            int idx;
            if(core_id == 0) {
                idx = l_idx++;
            } else {
                idx = r_idx--;
            }
            xSemaphoreGive(idx_mutex);
            int row = idx / N, col = idx % N;
            printf("Task %s is processing M3[%d][%d] on Core%d\n", pcTaskGetName(NULL), row, col, core_id);
            local_sum += M3[row][col];
        }
    }
    if(xSemaphoreTake(sum_mutex, portMAX_DELAY) == pdTRUE) {
        sum += local_sum;
        xSemaphoreGive(sum_mutex);
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    // setup
    idx_mutex = xSemaphoreCreateMutex();
    sum_mutex = xSemaphoreCreateMutex();

    // create Multiplication tasks
    ResetIndex();
    xTaskCreate(Mult_task, "mult_task0", 2048, NULL, 1, NULL);
    xTaskCreate(Mult_task, "mult_task1", 2048, NULL, 1, NULL);

    // delay
    vTaskDelay(pdMS_TO_TICKS(500));

    // create Summation tasks
    ResetIndex();
    xTaskCreate(Sum_task, "sum_task0", 2048, NULL, 1, NULL);
    xTaskCreate(Sum_task, "sum_task1", 2048, NULL, 1, NULL);

    // delay
    vTaskDelay(pdMS_TO_TICKS(500));

    // print M3 and sum
    printf("M3: \n");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%2d ", M3[i][j]);
        }
        printf("\n");
    }
    printf("sum of elements of M3: %d\n", sum);
}