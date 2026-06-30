#include "kernel/types.h"
#include "user/user.h"

const int ARR_SIZE = 1000;
int *arr;

int
main(void)
{
  int rudeness[3] = {5, 15, 30};

  arr = malloc(sizeof(int) * ARR_SIZE);
  arr[0] = 1;
  arr[1] = 2;
  for (int i = 2; i < ARR_SIZE; i++)
    arr[i] = 4 * arr[i - 1] - arr[i - 2];

  for (int i = 0; i < 3; i++) {
    if (fork() == 0) {
      int start, end;

      rude(rudeness[i]);
      start = uptime();

      if (rudeness[i] == 5) {
        long long sum = 0;

        for (int j = 0; j < ARR_SIZE; j++)
          sum += arr[j];
        printf("rude 5 sum: %lld\n", sum);
      } else if (rudeness[i] == 15) {
        int min = arr[0];
        int max = arr[0];
        long long range;

        for (int j = 1; j < ARR_SIZE; j++) {
          if (arr[j] < min)
            min = arr[j];
          if (arr[j] > max)
            max = arr[j];
        }

        printf("rude 15 normalized values x1000:\n");
        range = (long long)max - min;
        for (int j = 0; j < ARR_SIZE; j++) {
          int normalized = 0;

          if (range != 0)
            normalized = (((long long)arr[j] - min) * 1000) / range;
          printf("%d ", normalized);
        }
        printf("\n");
      } else {
        for (int j = 0; j < ARR_SIZE - 1; j++) {
          for (int k = 0; k < ARR_SIZE - j - 1; k++) {
            if (arr[k] > arr[k + 1]) {
              int temp = arr[k];
              arr[k] = arr[k + 1];
              arr[k + 1] = temp;
            }
          }
        }

        printf("rude 30 sorted values:\n");
        for (int j = 0; j < ARR_SIZE; j++)
          printf("%d ", arr[j]);
        printf("\n");
      }

      end = uptime();
      printf("metric: rude=%d elapsed=%d ticks\n", rudeness[i], end - start);
      free(arr);
      exit(0);
    }
  }

  for (int i = 0; i < 3; i++)
    wait(0);

  free(arr);
  exit(0);
}
