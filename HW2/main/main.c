#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define N 4

static int M1[N][N] = {{2, 0, 0, 6}, {0, 1, 1, 6}, {1, 3, 9, 0}, {1, 0, 3, 6}};
static int M2[N][N] = {{6, 0, 0, 2}, {6, 1, 1, 0}, {0, 9, 3, 1}, {6, 3, 0, 1}};
static int M3[N][N] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

static int pos = 0, sum = 0;
static SemaphoreHandle_t pos_mutex;

void Multiplication(void* arg) {
    while(1) {
        if(xSemaphoreTake(pos_mutex, 1000) == pdTRUE) {
            if(pos >= N * N) {
                xSemaphoreGive(pos_mutex);
                break;
            }
            int row = pos / N;
            int col = pos % N;
            int core_id = esp_cpu_get_core_id();
            printf("Task %s is calculating M3[%d][%d] on Core%d\n", pcTaskGetName(NULL), row, col, core_id);
            for(int i = 0; i < N; i++) {
                M3[row][col] += M1[row][i] * M2[i][col];
            }
            pos++;
            xSemaphoreGive(pos_mutex);
        }
    }
    vTaskDelete(NULL);
}

void Summation(void* arg) {
    while(1) {
        if(xSemaphoreTake(pos_mutex, 1000) == pdTRUE) {
            if(pos >= N * N) {
                xSemaphoreGive(pos_mutex);
                break;
            }
            int row = pos / N;
            int col = pos % N;
            int core_id = esp_cpu_get_core_id();
            printf("Task %s is summing M3[%d][%d] on Core%d\n", pcTaskGetName(NULL), row, col, core_id);
            sum += M3[row][col];
            pos++;
            xSemaphoreGive(pos_mutex);
        }
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    pos_mutex = xSemaphoreCreateMutex();
    // create Multiplication tasks
    xTaskCreate(Multiplication, "multiplication_task0", 2048, NULL, 1, NULL);
    xTaskCreate(Multiplication, "multiplication_task1", 2048, NULL, 1, NULL);

    // delay
    vTaskDelay(pdMS_TO_TICKS(500));
    pos = 0;

    // create Summation tasks
    xTaskCreate(Summation, "summation_task0", 2048, NULL, 1, NULL);
    xTaskCreate(Summation, "summation_task1", 2048, NULL, 1, NULL);

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