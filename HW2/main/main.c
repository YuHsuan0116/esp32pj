#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define N 4

static int M1[N][N] = {{2, 0, 0, 6}, {0, 1, 1, 6}, {1, 3, 9, 0}, {1, 0, 3, 6}};
static int M2[N][N] = {{6, 0, 0, 2}, {6, 1, 1, 0}, {0, 9, 3, 1}, {6, 3, 0, 1}};
static int M3[N][N];

static int idx = 0, sum = 0;
static SemaphoreHandle_t idx_mutex, sum_mutex, finished_tasks_sem;

void task(void* arg) {
    int core_id = esp_cpu_get_core_id();
    printf("%s is created on Core%d\n", pcTaskGetName(NULL), core_id);
    while(1) {
        if(xSemaphoreTake(idx_mutex, portMAX_DELAY) == pdTRUE) {
            if(idx > N * N - 1) {
                xSemaphoreGive(idx_mutex);
                break;
            }
            int row = idx / N, col = idx % N;
            idx++;
            xSemaphoreGive(idx_mutex);
            int val = 0;
            printf("%s is processing M3[%d][%d] on Core%d\n", pcTaskGetName(NULL), row, col, core_id);
            for(int i = 0; i < N; i++) {
                val += M1[row][i] * M2[i][col];
            }
            M3[row][col] = val;
            if(xSemaphoreTake(sum_mutex, portMAX_DELAY) == pdTRUE) {
                sum += val;
                xSemaphoreGive(sum_mutex);
            }
        }
    }
    xSemaphoreGive(finished_tasks_sem);
    vTaskDelete(NULL);
}

void app_main(void) {
    // setup
    idx_mutex = xSemaphoreCreateMutex();
    sum_mutex = xSemaphoreCreateMutex();
    finished_tasks_sem = xSemaphoreCreateCounting(2, 0);

    // create Multiplication tasks
    xTaskCreate(task, "task0", 2048, NULL, 1, NULL);
    xTaskCreate(task, "task1", 2048, NULL, 1, NULL);

    for(int i = 0; i < 2; i++) {
        xSemaphoreTake(finished_tasks_sem, portMAX_DELAY);
    }

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