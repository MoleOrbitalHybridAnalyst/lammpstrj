#ifndef __MY_VECTOR_H__
#define __MY_VECTOR_H__

#include <stdexcept>
#include <cmath>

namespace VECTOR_NS {

template<class T>
class Vector {
   T d[3];

public:
   Vector() = default;
/// create a vector with same value
   Vector(T d0) {d[0] = d[1] = d[2] = d0;}
   Vector(T d0, T d1, T d2) {
      d[0] = d0; d[1] = d1; d[2] = d2;
   }
   Vector(T *x) {d[0] = x[0]; d[1] = x[1]; d[2] = x[2]; }
   T& operator[] (int i);
   T  operator[] (int i) const;
   Vector& operator+= (const Vector& v);
   Vector& operator-= (const Vector& v);
   Vector& operator*= (const T& s);
   Vector& operator/= (const T& s);
   Vector operator- () const;
   T modulo2() const {return dotProduct(*this, *this); }
   T modulo() const {return std::sqrt(modulo2()); }
   int size() const {return 3;}

   template<class U>
   friend Vector<U> operator+ (const Vector<U>&, const Vector<U>&);
   template<class U>
   friend Vector<U> operator- (const Vector<U>&, const Vector<U>&);
   template<class U>
   friend Vector<U> operator* (T, const Vector<U>&);
   template<class U>
   friend Vector<U> operator* (const Vector<U>&, T);
   template<class U>
   friend Vector<U> operator/ (const Vector<U>&, T);
   template<class U>
   friend Vector<U> operator* (const Vector<U>&, const Vector<U>&);
   template<class U>
   friend Vector<U> operator/ (const Vector<U>&, const Vector<U>&);
   template<class U>
   friend U dotProduct(const Vector<U>&, const Vector<U>&);
   template<class U>
   friend Vector<U> closest_int(const Vector<U>&);
};

template<class T>
T& Vector<T>::operator[] (int i) {
   if(i < -size() || i >= size())
      throw std::runtime_error("index out of range");
   if(i >= 0) return d[i];
   else      return d[size() + i];
}

template<class T>
T Vector<T>::operator[] (int i) const {
   if(i < -size() || i >= size())
      throw std::runtime_error("index out of range");
   if(i >= 0) return d[i];
   else      return d[size() + i];
}

template <class T>
Vector<T>& Vector<T>::operator+= (const Vector<T>& v)
{
   if(size() != v.size())
      throw std::runtime_error("incompatible sizes");
   for(int k = 0; k < size(); ++k) d[k] += v.d[k];
   return *this;
}

template <class T>
Vector<T>& Vector<T>::operator-= (const Vector<T>& v)
{
   if(size() != v.size())
      throw std::runtime_error("incompatible sizes");
   for(int k = 0; k < size(); ++k) d[k] -= v.d[k];
   return *this;
}

template <class T>
Vector<T>& Vector<T>::operator*= (const T& s)
{
   for(int k = 0; k < size(); ++k) d[k] *= s;
   return *this;
}

template <class T>
Vector<T>& Vector<T>::operator/= (const T& s)
{
   for(int k = 0; k < size(); ++k) d[k] /= s;
   return *this;
}

template <class T>
Vector<T> Vector<T>::operator- () const
{
   return Vector<T>(-d[0], -d[1], -d[2]);
}

//friends of Vector
//
template <class T>
Vector<T> operator+ (const Vector<T>& v1, const Vector<T>& v2)
{
   Vector<T> v(v1); return v += v2;
}

template <class T>
Vector<T> operator- (const Vector<T>&v1, const Vector<T>& v2)
{
   Vector<T> v(v1); return v -= v2;
}

template <class T>
Vector<T> operator* (T s, const Vector<T>& v)
{
   Vector<T> _v(v);
   return _v *= s;
}

template <class T>
Vector<T> operator* (const Vector<T>& v, T s)
{
   Vector<T> _v(v);
   return _v *= s;
}

template <class T>
Vector<T> operator/ (const Vector<T>& v, T s)
{
   Vector<T> _v(v);
   return _v /= s;
}

template <class T>
Vector<T> operator* (const Vector<T>& v1, const Vector<T>& v2)
{
   if(v1.size() != v2.size())
      throw std::runtime_error("incompatible sizes");
   Vector<T> v;
   for(int k = 0; k < v1.size(); ++k) v.d[k] = v1.d[k] * v2.d[k];
   return v;
}

template <class T>
Vector<T> operator/ (const Vector<T>& v1, const Vector<T>& v2)
{
   if(v1.size() != v2.size())
      throw std::runtime_error("incompatible sizes");
   Vector<T> v;
   for(int k = 0; k < v1.size(); ++k) v.d[k] = v1.d[k] / v2.d[k];
   return v;
}

template <class T>
T dotProduct(const Vector<T>& v1, const Vector<T>& v2)
{
   if(v1.size() != v2.size())
      throw std::runtime_error("incompatible sizes");
   T x = T(0.0);
   for(int k = 0; k < v1.size(); ++k)
      x += (v1.d[k] * v2.d[k]);
   return x;
}

template <class T>
Vector<T> closest_int(const Vector<T>& v)
{
   Vector<T> result;
   for(int k = 0; k < v.size(); ++k) {
      result[k] = (T)(static_cast<int>(std::floor(v.d[k] + 0.5)));
   }
   return result;
}

}

#endif
