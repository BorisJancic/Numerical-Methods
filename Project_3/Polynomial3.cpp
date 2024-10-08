#include <cassert>
#include <stdexcept>
#include "Polynomial.hpp"

// Create a polynomial of degree 'N - 1' where 'N' is
// the number of objects in the initializer list.
//
// The 'N' coefficients are stored in an array of that
// capacity
//  - The capacity of the array must be at least one
Polynomial::Polynomial(
    std::initializer_list<std::complex<double>> init) : degree_{(init.size() == 0) ? 0 : init.size() - 1},
                                                        coeffs_{new std::complex<double>[degree_ + 1]}
{
  // If the user creates a polynomial with no arguments,
  // we will create the zero polynomial.
  if (init.size() == 0)
  {
    coeffs_[0] = 0.0;

    // Otherwise, the first argument is the constant
    // coefficient, the second is the coefficient of
    // 'x', and so on, until the last is the
    //                 N-1
    // coefficient of x   .
  }
  else
  {
    std::size_t k{0};

    for (auto itr{init.begin()};
         itr != init.end();
         ++k, ++itr)
    {
      coeffs_[k] = *itr;
      std::cout << coeffs_[k] << std::endl;
    }

    if ((degree_ >= 1) && (coeffs_[degree_] == 0.0))
    {
      delete[] coeffs_;
      coeffs_ = nullptr;
      throw std::invalid_argument{"The leading coefficient must be non-zero"};
    }
  }
}

Polynomial::~Polynomial()
{
  delete[] coeffs_;
  coeffs_ = nullptr;
  degree_ = 0;
}

std::complex<double> Polynomial::operator()(std::complex<double> z) const
{
  return evaluate(coeffs_, degree_, z);
}

// This function is writen for you, but you can change it if
// you wish. It is only necessary that it retern a std::vector
// instance with the roots. It must be 'const'.
std::vector<std::complex<double>> Polynomial::roots() const
{
  // The vector of roots to be returned by this function
  //   - The argument is the number of entries in the std::vector
  std::vector<std::complex<double>> roots_vec(degree_);

  // An array of the coefficients
  //  - This will be manipulated by you as you find the roots
  std::complex<double> coeffs[degree_ + 1];

  // The polynomial may have a leading coefficient that is not
  // equal to '1'. It will be easier if the leading coefficient
  // is '1', so the coeffs array will store the coefficients
  // divided by the leadning coefficient, which makes the
  // leading coefficient '1'.

  assert(std::abs(coeffs_[degree_]) > 1e-16);
  coeffs[degree_] = 1.0;

  for (unsigned int k{0}; k < degree_; ++k)
  {
    coeffs[k] = coeffs_[k] / coeffs_[degree_];
  }

  // In general, methods like Newton's method and the secant
  // may require a complex initial value. If you implement
  // Muller's method, you do not need this special case and
  // your algorithm can use either real or complex initial
  // values. You could, if you wanted, always use a complex
  // initial value; but this has the nasty side-effect that
  // if you have a polynomial with real coefficients, then the
  // roots, if they are real, will be approximated with values
  // like 1.414213562373095 + 0.3957188605e-53j

  for (unsigned int k{0}; k < degree_; ++k)
  {
    roots_vec[k] = find_root(coeffs, degree_ - k);
    divide(coeffs, degree_ - k, roots_vec[k]);
  }

  return roots_vec;
}

// Evaluate the polynomial at the point 'x' using Horner's Rule

std::complex<double> Polynomial::evaluate(
    std::complex<double> const *const coeffs,
    unsigned int const degree,
    std::complex<double> const z)
{
  // a0 + x(a1 + x(a2 + x*value))
  std::complex<double> value = coeffs[degree];

  for (int n = degree - 1; n >= 0; n--)
  {
    value = coeffs[n] + z * value;
  }

  return value;
}

// Given the polynomial of degree 'degree', the coefficients
// of which are given in the array 'coeffs', find an approximation
// of a root.
// The root finding function uses Newton's Method to find a root, until it reaches a value
// within some epsilon. In the case that our initial point z0 does not converge to a root
// we run newtons method again, on another random point

std::complex<double> Polynomial::find_root(
    std::complex<double> const *const coeffs,
    unsigned int const degree)
{
  // break if there are no roots / inf roots
  if (degree == 0)
  {
    return 0.0;
  }
  // inf loop to keep trying new initial values if the random value we pick doesn't converge
  while (true)
  {
    int n = 0;
    // initialize random point in the complex plane
    std::complex<double> zn{
        (rand() + 1.0) / (RAND_MAX + 2.0) * 20.0 - 10.0,
        (rand() + 1.0) / (RAND_MAX + 2.0) * 20.0 - 10.0};
    // exit margin
    double epsilon = 1e-10;
    // std::vector<std::complex<double>> *const f_prime;
    // f_prime = new std::vector<std::complex<double>>;
    std::complex<double> *f_prime = new std::complex<double>[degree];
    // initialize coeffitiants for derivative
    for (int i = degree; i >= 1; i--)
    {
      f_prime[i - 1] = coeffs[i] * (double)i;
    }

    // exit if our initial point does not converge after 1e10 runs
    while (n < 1e10)
    {
      // evaluate next point using Newton's Method: zn+1 = zn - f(zn)/f'(zn)
      zn -= evaluate(coeffs, degree, zn) / evaluate(f_prime, degree - 1, zn);

      // return root when within sufficient margin
      if (abs(evaluate(coeffs, degree, zn)) < epsilon)
      {
        delete[] f_prime;
        return zn;
      }
      n++;
    }
  }
}

// Polynomial division
//
// In place, update the entries in the coefficient array to
// give the quotient when the polynomial defined by
//                                                    degree
//    coeffs[0] + coeffs[1] x + ... + coeffs[degree] x
// is divided by x - r

std::complex<double> Polynomial::divide(
    std::complex<double> *coeffs,
    unsigned int const degree,
    std::complex<double> const r)
{
  // exit if can't be divided
  if (degree == 0)
  {
    return 0;
  }

  // use synthetic division
  std::vector<std::complex<double>> new_coeffs(degree);
  new_coeffs[degree - 1] = coeffs[degree];
  for (int i = degree - 1; i > 0; i--)
  {
    new_coeffs[i - 1] = coeffs[i] + new_coeffs[i] * r;
  }
  // fill coefficients with new values after division
  coeffs[degree] = 0;

  std::complex<double> result = coeffs[0] + new_coeffs[0] * r;
  for (int i = degree; i > 0; i--)
  {
    coeffs[i - 1] = new_coeffs[i - 1];
  }

  // return remainder
  return result;
}

std::string to_string_term(unsigned int n)
{
  if (n == 0)
  {
    return "";
  }
  else if (n == 1)
  {
    return "z";
  }
  else
  {
    return "z^" + std::to_string(n);
  }
}

std::string to_string_complex(std::complex<double> z, bool is_leading, unsigned int n)
{
  if (z.imag() == 0.0)
  {
    // The coefficient is real (possibly zero)
    if (z.real() == 0.0)
    {
      if (is_leading && (n == 0))
      {
        return "0";
      }
      else
      {
        return "";
      }
    }
    else
    {
      if (is_leading)
      {
        return std::to_string(z.real()) + to_string_term(n);
      }
      else
      {
        if (z.real() < 0.0)
        {
          return " - " + std::to_string(-z.real()) + to_string_term(n);
        }
        else
        {
          return " + " + std::to_string(z.real()) + to_string_term(n);
        }
      }
    }
  }
  else if (z.real() == 0.0)
  {
    // The coefficient is a non-zero imaginary number
    if (is_leading)
    {
      return std::to_string(z.imag()) + "j" + to_string_term(n);
    }
    else
    {
      if (z.imag() < 0.0)
      {
        return " - " + std::to_string(-z.imag()) + "j" + to_string_term(n);
      }
      else
      {
        return " + " + std::to_string(z.imag()) + "j" + to_string_term(n);
      }
    }
  }
  else
  {
    // Both real and imaginary parts are non-zero
    if (is_leading && (n == 0))
    {
      if (z.imag() < 0.0)
      {
        return std::to_string(z.real()) + " - " + std::to_string(-z.imag()) + "j";
      }
      else
      {
        return std::to_string(z.real()) + " + " + std::to_string(z.imag()) + "j";
      }
    }
    else
    {
      bool is_negative{false};
      std::string out{""};

      if (z.real() < 0.0)
      {
        z = -z;
        is_negative = true;
      }

      if (z.imag() < 0.0)
      {
        out = out + std::to_string(z.real()) + " - " + std::to_string(-z.imag()) + "j";
      }
      else
      {
        out = out + std::to_string(z.real()) + " + " + std::to_string(z.imag()) + "j";
      }

      if (is_negative)
      {
        if (is_leading)
        {
          out = "-(" + out + ")";
        }
        else
        {
          out = " - (" + out + ")";
        }
      }
      else if (n > 0)
      {
        out = "(" + out + ")";

        if (!is_leading)
        {
          out = " + " + out;
        }
      }
      else
      {
        out = " + " + out;
      }

      return out + to_string_term(n);
    }
  }
}

std::string Polynomial::to_string() const
{
  std::string out{to_string_complex(coeffs_[degree_], true, degree_)};

  for (unsigned int n{degree_ - 1}; n < degree_; --n)
  {
    out += to_string_complex(coeffs_[n], false, n);
  }

  return out;
}

std::ostream &operator<<(std::ostream &out, Polynomial const &p)
{
  return out << p.to_string();
}