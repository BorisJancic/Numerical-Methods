#include <iostream>
#include <functional>
#include <cmath>

int main();
double range( std::function<double( double )> f,
              double a,
              double b,
              double max_freq );
double interpolate(double x1, double x0, double x2,
	double f1, double f0, double f2);
// These will change for the tests
double f1( double x );
double f2( double x );
double f3( double x );

int main() {
  std::cout.precision( 16 );
  for (int i = 0; i < 5; i++) {
	  std::cout << "****************" << std::endl;
  }

  // These will change  for the tests
  double max_freq1{ 8.3/2.0/M_PI };
  
  for ( double a{ 0.0 }; a <= 5.0; ++a ) {
    for ( double b{ 1.0 }; b <= 14.0; ++b ) {
      std::cout << "[" << a << ", " << a + b << "]" << std::endl;
      f1( INFINITY  );
      std::cout << range( f1, a, a + b, max_freq1 ) << std::endl;
      std::cout << "Function calls: " << f1( INFINITY  ) << std::endl;
      std::cout << range( f1, a, a + b, max_freq1 ) << std::endl;
      f1( INFINITY  );
      std::cout << " >> " << range( f1, a, a + b, max_freq1 )
                 - range( f1, a, a + b, max_freq1 ) << std::endl;
      std::cout << std::endl;
    }
  }

  double max_freq2{ 1.0/M_PI };

  for ( double a{ 0.0 }; a <= 5.0; ++a ) {
    for ( double b{ 1.0 }; b <= 14.0; ++b ) {
      std::cout << "[" << a << ", " << a + b << "]" << std::endl;
      f2( INFINITY  );
      std::cout << range( f2, a, a + b, max_freq2 ) << std::endl;
      std::cout << "Function calls: " << f2( INFINITY  ) << std::endl;
      std::cout << range( f2, a, a + b, max_freq2 ) << std::endl;
      f2( INFINITY  );
      std::cout << " >> " << range( f2, a, a + b, max_freq2 )
                 - range( f2, a, a + b, max_freq2 ) << std::endl;
      std::cout << std::endl;
    }
  }

  double max_freq3{ 2.0/M_PI };

    for ( double a{ 0.0 }; a <= 0.0; ++a ) {
    for ( double b{ 10.0 }; b <= 10.0; ++b ) {
      std::cout << "[" << a << ", " << a + b << "]" << std::endl;
      f3( INFINITY  );
      std::cout << range( f3, a, a + b, max_freq3 ) << std::endl;
      std::cout << "Function calls: " << f3( INFINITY  ) << std::endl;
      std::cout << range( f3, a, a + b, max_freq3 ) << std::endl;
      f3( INFINITY  );
      std::cout << " >> " << range( f3, a, a + b, max_freq3 )
                 - range( f3, a, a + b, max_freq3 ) << std::endl;
      std::cout << std::endl;
    }
  }

  return 0;
}

  ////////////////////////
 // The function 'f(x) //
////////////////////////

double f1( double x ) {
  static unsigned long count{ 0 };

  if ( x == INFINITY ) {
    unsigned long value{ count };
    count = 0;
    return value;
  } else {
    ++count;
    return 
            std::sin( 2.4*x + 0.8 )
      + 3.5*std::sin( 8.3*x + 1.5 )
      + 3.7*std::sin( 0.5*x + 1.2 );
  }
}

double f2( double x ) {
  static unsigned long count{ 0 };

  if ( x == INFINITY ) {
    unsigned long value{ count };
    count = 0;
    return value;
  } else {
    ++count;
    return std::sin( x ) + std::sin( 2.0*x + 1.0 );
  }
}

double f3( double x ) {
  static unsigned long count{ 0 };

  if ( x == INFINITY ) {
    unsigned long value{ count };
    count = 0;
    return value;
  } else {
    ++count;
    return std::sin(   x ) + std::sin( 2*x )
         + std::sin( 3*x ) + std::sin( 4*x );
  }
}

double range( std::function<double (double)> f,
              double a, double b,
              double max_freq ) {
    double max_value {-MAXFLOAT};
    double min_value {MAXFLOAT};
    double h = 1/max_freq/4;
	double current = a;
	int i = 0;
	double x1;
	double x0 = a;
	double x2 = a+h;
	double f1;
	double f0 = f(x0);
	double f2 = f(x2);
	double delta = 1e-22;
	double epsilon = 1e-12;
	if (f0 > max_value) { max_value = f1; }
	if (f0 < min_value) { min_value = f1; }
	if (f2 > max_value) { max_value = f2; }
	if (f2 < min_value) { min_value = f2; }

	while (current <= b + h) {
		x1 = x0; x0 = x2; x2 = current; 
		f1 = f0; f0 = f2; f2 = f(current);
		if (f2 > max_value) { max_value = f2; }
		if (f2 < min_value) { min_value = f2; }

		double x_1 = x1; double x_0 = x0; double x_2 = x2;
		double f_1 = f1; double f_0 = f0; double f_2 = f2;
		double x_next; double f_next;
		if (f0 > f1 && f0 > f2) {
			do {
				if ( (f_1 > f_0) && (f_1 >= f_2) ) {
					std::swap( x_0, x_1 );
					std::swap( f_0, f_1 );
				} else if ( f_2 > f_0 ) {
					std::swap( x_0, x_2 );
					std::swap( f_0, f_2 );
				}
				if ( f_2 > f_1 ) {
					std::swap( x_1, x_2 );
					std::swap( f_1, f_2 );
				}

				x_next = interpolate(x_1, x_0, x_2, f_1, f_0, f_2);
				f_next = f(x_next);
				if (f_next > max_value && x_next <= b) { max_value = f_next; }

				x_2 = x_next;
    			f_2 = f_next;
			} while (abs(f_next-f_0) > epsilon && abs(x_next-x_0) > delta);
		} else if (f0 < f1 && f0 < f2) {
			do {
				if ( (f_1 < f_0) && (f_1 <= f_2) ) {
					std::swap( x_0, x_1 );
					std::swap( f_0, f_1 );
				} else if ( f_2 < f_0 ) {
					std::swap( x_0, x_2 );
					std::swap( f_0, f_2 );
				}
				if ( f_2 < f_1 ) {
					std::swap( x_1, x_2 );
					std::swap( f_1, f_2 );
				}

				x_next = interpolate(x_1, x_0, x_2, f_1, f_0, f_2);
				f_next = f(x_next);
				if (f_next < min_value && x_next <= b) { min_value = f_next; }

				x_2 = x_next;
    			f_2 = f_next;
			} while (abs(f_next-f_0) > epsilon && abs(x_next-x_0) > delta);
		} 
		i++;
		current = a + i/max_freq/4;
	}
	f0 = f(b);
	if (f0 > max_value) { max_value = f0; }
	if (f0 < min_value) { min_value = f0; }
	// std::cout << "Max: " << max_value << " " << "Min: " << min_value << std::endl;
    return abs(max_value-min_value);
}

double interpolate(double x1, double x0, double x2,
				   double f1, double f0, double f2) {
	double a = (x0+x1)/2;
	double b = (f0-f1)*(x1-x2)*(x2-x0);
	double c = 2*((f0 - f1)*(x1-x2) + (f2-f1)*(x0-x1));
	return a + b/c;
}