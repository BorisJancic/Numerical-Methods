#include <assert.h>
#include <math.h>
#include "analyze.h"

#include <stdio.h>


// You can add other functions if you want to perform
// other calculations indepenent of these functions.

void data_init( data_t *p_this, double step_size ) {
    p_this->front = 0;
    p_this->back = 0;
}

// You may not need this if you do not have a dynamically
// allocated array.
void data_destroy( data_t *p_this ) {
  // Your implementation here...
}

void data_append( data_t *p_this, double new_value ) {
  // Your implementation here...
  p_this->entries_[p_this->front] = new_value;
  (++p_this->front) %= 8;
}

double data_current( data_t *p_this ) {
    // Your implementation here...
    double matrix[3][3] = {   { 17.0/24, 3.0/8, 1.0/24 },
                            { 3.0/8, 53.0/168, 1.0/24 },
                            { 1.0/24, 1.0/24, 1.0/168 } };
    double coefficients[3] = { 0, 0, 0 };
    double resultant[3] = { 0, 0, 0 };

    for (int i = 0; i < 3; i++) {
        int current = p_this->front;
        for (int j = 0; j > -8; j--) {
            resultant[i] += p_this->entries_[current]*pow(j, i);
            current--;
            if (current < 0) { current = 7; }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            coefficients[i] += resultant[j]*matrix[i][j];
        }
    }
    printf("%f,%f,%f", coefficients[0], coefficients[0], coefficients[0]);
    return 0.0;
}

double data_next( data_t *p_this ) {
  // Your implementation here...
  return 0.0;
}

