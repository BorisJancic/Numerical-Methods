#ifndef CA_UWATERLOO_DWHARDER_DATA_ANAYSIS
#define CA_UWATERLOO_DWHARDER_DATA_ANAYSIS

#define CAPACITY 8

// Structure declarations
typedef struct data data_t;

// Function declarations
void data_init( data_t *p_this, double step_size );
// You may not need this
void data_destroy( data_t *p_this );

void data_append( data_t *p_this, double new_value );

double data_current( data_t *p_this );
double data_next( data_t *p_this );

// Structure definitions
struct data {
  double entries_[CAPACITY];
  int front;
};
#endif