#ifndef VECTOR_H
#define VECTOR_H


#include <cmath>
#include <sstream>
#include <array>

#include <initializer_list>

#include "FDMathCore.h"




namespace FDMathCore
{
	template <typename T, size_t S>
	class Segment;

	template <typename T, size_t S>
	class Line;

	template <typename T, size_t S>
	class Point;


	template<typename T, size_t S>
	class Vector
	{
		private:
			std::array<T, S> m_arr;

		public:

			Vector()
			{
				m_arr.fill(0);
			}


			explicit Vector(const std::array<T, S> &v) : m_arr(v) {}


			explicit Vector(const std::initializer_list<T> &l) : Vector<T, S>()
			{
				size_t i(0);
				for (auto it(l.begin());
				it != l.end() && i < S; ++it, ++i)
					m_arr[i] = *it;

				for (; i < S; ++i)
					m_arr[i] = 0;
			}


			Vector(const std::string &str)
			{
				this->fromString(str);
			}

			Vector(const std::wstring &str)
			{
				this->fromWString(str);
			}


			explicit Vector(const Point<T, S> &p)
			{
				for (size_t i(0); i < S; ++i)
				{
					this->at(i) = p.at(i);
				}
			}

			Vector(const Point<T, S> &p1, const Point<T, S> &p2)
			{
				for (size_t i(0); i < S; ++i)
				{
					this->at(i) = p2.at(i) - p1.at(i);
				}
			}


			Vector(const Line<T, S> &l) : Vector(l.getPoint(0), l.getPoint(1))
			{
				T n = this->euclidianNorm();
				if (n != 0)
				{
					for (size_t i(0); i < S; ++i)
					{
						this->at(i) /= n;
					}
				}
			}



			Vector(const Segment<T, S> &l) : Vector(l.getPoint(0), l.getPoint(1)) {}




			inline bool isNull() const
			{
				return FDMathCore::isNull(m_arr);
			}



			inline static bool isColinear(const T det)
			{
				return isSignedZero(det);
			}


			inline bool isColinear(const Vector<T, S> &v) const
			{
				return isColinear(v, nullptr);
			}



			bool isColinear(const Vector<T, S> &v, T* factor) const
			{
				if (this->isNull() || v.isNull())
				{
					if (factor != nullptr)
						*factor = 0;

					return true;
				}


				size_t i(0);
				T k(0);
				while (i < S)
				{
					T absU(abs(this->at(i))), absV(abs(v.at(i)));
					if ((isZero(absU) && !isZero(absV))
						|| (!isZero(absU) && isZero(absV)))
						return false;

					if (!isZero(absU))
					{
						k = v.at(i) / this->at(i);
						++i;
						break;
					}

					++i;
				}

				while (i < S)
				{
					T absU(abs(this->at(i))), absV(abs(v.at(i)));
					if ((isZero(absU) && !isZero(absV))
						|| (!isZero(absU) && isZero(absV)))
						return false;

					if (!isZero(absU))
					{
						if (!isEqual(k, (v.at(i) / this->at(i))))
							return false;
					}

					++i;
				}

				if (factor != nullptr)
					*factor = k;

				return k != 0;
			}

			inline static bool isPositive(const T det)
			{
				return det > 0;
			}

			inline static bool isNegative(const T det)
			{
				return det < 0;
			}

			Vector<T, S> crossProduct(const Vector<T, S> &v) const
			{
				Vector<T, S> w;
				for (size_t i(0); i < S; ++i)
				{
					size_t x((i + 1) % S), y((i + 2) % S);
					w.at(i) += this->at(x) * v.at(y) - this->at(y) * v.at(x);
				}

				return w;
			}


			inline static T sinVectors(const T normW, const T normU, const T normV)
			{
				return normW / (normU * normV);
			}


			T sinVectors(const Vector<T, S> &v) const
			{
				Vector<T, S> w(this->crossProduct(v));
				return sinVectors(w.euclidianNorm(), euclidianNorm(), v.euclidianNorm());
			}



			T dotProduct(const Vector<T, S> &v) const
			{
				T ret(0);
				for (size_t i(0); i < S; ++i)
				{
					ret += this->at(i) * v.at(i);
				}

				return ret;
			}




			inline static T cosVectors(const T scal, const T normU, const T normV)
			{
				return scal / (normU * normV);
			}




			inline T cosVectors(const Vector<T, S> &v) const
			{
				return cosVectors(dotProduct(v), euclidianNorm(), v.euclidianNorm());
			}



			inline static T angleVectors(const T cos)
			{
				return acos(cos);
			}


			inline static T angleVectors(const T cos, bool positive)
			{
				return (positive ? angleVectors(cos) : -angleVectors(cos));
			}


			inline static T angleVectors(const T cos, const T det)
			{
				return angleVectors(cos, isPositive(det));
			}


			inline T angleVectors(const Vector<T, S> &v) const
			{
				return Vector<T, S>::angleVectors(cosVectors(v));
			}

			inline T angleVectors(const Vector<T, S> &v, bool positive) const
			{
				return Vector<T, S>::angleVectors(cosVectors(v), positive);
			}


			inline static bool isOrthogonal(const T scal)
			{
				return isSignedZero(scal);
			}




			inline bool isOrthogonal(const Vector<T, S> &v) const
			{
				return isOrthogonal(dotProduct(v));
			}



			inline static bool isAcute(const T scal)
			{
				return scal > 0;
			}




			inline bool isAcute(const Vector<T, S> &v) const
			{
				return isAcute(dotProduct(v));
			}


			inline static T isObtuse(const T scal)
			{
				return scal < 0;
			}



			T isObtuse(const Vector<T, S> &v) const
			{
				return isObtuse(dotProduct(v));
			}


			T euclidianNorm() const
			{
				T val = 0;
				for (size_t i(0); i < S; ++i)
					val += pow(this->at(i), 2);
				return sqrt(val);
			}


			const Vector<T, S> &normal(Vector<T, S> &n) const
			{
				for (size_t i(0), c(S / 2); i < c; ++i)
				{
					size_t j = i * 2;
					if (this->at(j) != 0)
					{
						n.at(j) = this->at(j + 1);
						n.at(j + 1) = -this->at(j);
					}
					else
					{
						n.at(j) = -this->at(j + 1);
						n.at(j + 1) = this->at(j);
					}
				}

				if (S % 2 != 0)
					n.at(S - 1) = 0;

				return n;
			}




			Vector<T, S> normal() const
			{
				Vector<T, S> n;

				normal(n);

				return n;
			}



			inline bool isNormal(const Vector<T, S> &n) const
			{
				return isOrtogonal(n);
			}



			inline bool isInternNormal(const Vector<T, S> &n, const Vector<T, S> &v) const
			{
				return isOrtogonal(n) && v.isAcute(n);
			}




			inline bool isExternNormal(const Vector<T, S> &n, const Vector<T, S> &v) const
			{
				return isOrtogonal(n) && v.isObtuse(n);
			}

			inline Vector<T, S> normalize() const
			{
				return *this / this->euclidianNorm();
			}



			std::string doToString() const
			{
				std::ostringstream ret;

				ret << '[' << this->at(0);

				for (size_t i(1); i < S; ++i)
					ret << ", " << this->at(i);

				ret << ']';

				return ret.str();
			}



			std::wstring doToWString() const
			{
				std::wostringstream ret;

				ret << '[' << this->at(0);

				for (size_t i(1); i < S; ++i)
					ret << ", " << this->at(i);

				ret << ']';

				return ret.str();
			}




			std::istream &doFromStream(std::istream &stream)
			{
				char c;
				T val;
				stream >> c;

				for (size_t i(0); i < S; ++i)
				{
					stream >> val >> c;
					this->at(i) = val;
				}

				return stream;
			}



			std::wistream &doFromWStream(std::wistream &stream)
			{
				wchar_t c;
				T val;
				stream >> c;

				for (size_t i(0); i < S; ++i)
				{
					stream >> val >> c;
					this->at(i) = val;
				}

				return stream;
			}



			Vector<T, S> &operator=(const std::initializer_list<T> &l)
			{
				size_t i(0);
				for (auto it(l.begin());
				it != l.end() && i < S; ++it, ++i)
					m_arr[i] = *it;

				for (; i < S; ++i)
					m_arr[i] = 0;

				return *this;
			}

			Vector<T, S> &operator=(const std::array<T, S> &arr)
			{
				m_arr = arr;
				return *this;
			}




			bool operator==(const Vector<T, S> &v) const
			{
				size_t i(0);

				while (i < S)
				{
					if (!isEqual(this->at(i), v.at(i)))
						return false;

					i++;
				}

				return true;
			}




			bool operator==(const T t) const
			{
				if (t == 0 && isNull())
					return true;

				return false;
			}




			inline bool operator!=(const Vector<T, S> &v) const
			{
				return !(*this == v);
			}


			inline bool operator!=(const T t) const
			{
				return !(*this == t);
			}



			Vector<T, S> operator-() const
			{
				Vector<T, S> v;
				for (size_t i(0); i < S; ++i)
					v.at(i) = -this->at(i);
				return v;
			}



			Vector<T, S> &operator+=(const Vector<T, S> &v)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) += v.at(i);

				return *this;
			}



			Vector<T, S> &operator+=(const T k)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) += k;

				return *this;
			}


			Vector<T, S> &operator-=(const Vector<T, S> &v)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) -= v.at(i);

				return *this;
			}


			Vector<T, S> &operator-=(const T k)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) -= k;

				return *this;
			}


			Vector<T, S> &operator*=(const T k)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) *= k;

				return *this;
			}


			Vector<T, S> &operator/=(const T k)
			{
				for (size_t i(0); i < S; ++i)
					this->at(i) /= k;

				return *this;
			}



			inline Vector<T, S> operator+(Vector<T, S> v) const
			{
				return v += (*this);
			}


			Vector<T, S> operator+(const T k) const
			{
				Vector<T, S> v(*this);
				return v += k;
			}


			Vector<T, S> operator-(Vector<T, S> v) const
			{
				Vector<T, S> w(*this);
				return w -= v;
			}


			Vector<T, S> operator-(const T k) const
			{
				Vector<T, S> v(*this);
				return v -= k;
			}


			Vector<T, S> operator*(const T k) const
			{
				Vector<T, S> v(*this);
				return v *= k;
			}



			inline T operator*(const Vector<T, S> &v) const
			{
				return this->dotProduct(v);
			}



			inline Vector<T, S> operator^(const Vector<T, S> &v) const
			{
				return this->crossProduct(v);
			}



			Vector<T, S> operator/(const T k) const
			{
				Vector<T, S> v(*this);
				return v /= k;
			}

			T &at(size_t i)
			{
				return m_arr.at(i);
			}





			const T &at(size_t i) const
			{
				return m_arr.at(i);
			}





			T &operator[](size_t i)
			{
				return m_arr.at(i);
			}





			const T &operator[](size_t i) const
			{
				return m_arr.at(i);
			}

			const T *data() const
			{
				return m_arr.data();
			}



			T *data()
			{
				return m_arr.data();
			}
	};



	template<typename T>
	class Vector<T, 1>
	{
		public:
			Vector() { throw std::invalid_argument("Error : cannot instantiate Vector with size 1"); }
	};

	template<typename T>
	class Vector<T, 0>
	{
		public:
			Vector() { throw std::invalid_argument("Error : cannot instantiate Vector with size 0"); }
	};


	template<typename T>
	class Vector<T, 2>
	{
		private:
			std::array<T, 2> m_arr;

		public:

			Vector()
			{
				m_arr.fill(0);
			}


			explicit Vector(const std::array<T, 2> &v) : m_arr(v) {}


			explicit Vector(const std::initializer_list<T> &l) : Vector<T, 2>()
			{
				size_t i(0);
				for (auto it(l.begin());
				it != l.end() && i < 2; ++it, ++i)
					m_arr[i] = *it;

				for (; i < 2; ++i)
					m_arr[i] = 0;
			}


			Vector(const std::string &str)
			{
				this->fromString(str);
			}

			Vector(const std::wstring &str)
			{
				this->fromWString(str);
			}

			explicit Vector(const Point<T, 2> &p)
			{
				for (size_t i(0); i < 2; ++i)
				{
					this->at(i) = p.at(i);
				}
			}

			Vector(const Point<T, 2> &p1, const Point<T, 2> &p2)
			{
				this->at(0) = p2.at(0) - p1.at(0);
				this->at(1) = p2.at(1) - p1.at(1);
			}


			Vector(const Line<T, 2> &l) : Vector(l.getPoint(0), l.getPoint(1))
			{
				T n = this->euclidianNorm();

				if (n != 0)
				{
					this->at(0) /= n;
					this->at(1) /= n;
				}
			}


			Vector(const Segment<T, 2> &l) : Vector(l.getPoint(0), l.getPoint(1)) {}

			inline T getDeterminant(const Vector<T, 2> &v) const
			{
				return this->at(0) * v.at(1) - this->at(1) * v.at(0);
			}


			inline static bool isColinear(const T det)
			{
				return isSignedZero(det);
			}


			inline bool isColinear(const Vector<T, 2> &v) const
			{
				return isColinear(getDeterminant(v));
			}


			inline static bool isPositive(const T det)
			{
				return det > 0;
			}


			inline bool isPositive(const Vector<T, 2> &v) const
			{
				return isPositive(getDeterminant(v));
			}


			inline static bool isNegative(const T det)
			{
				return det < 0;
			}


			inline bool isNegative(const Vector<T, 2> &v) const
			{
				return isNegative(getDeterminant(v));
			}


			inline T dotProduct(const Vector<T, 2> &v) const
			{
				return this->at(0) * v.at(0) + this->at(1) * v.at(1);
			}


			inline static T angleVectors(const T cos, bool positive)
			{
				return (positive ? acos(cos) : -acos(cos));
			}


			inline static T angleVectors(const T cos, const T det)
			{
				return angleVectors(cos, isPositive(det));
			}


			inline T angleVectors(const Vector<T, 2> &v) const
			{
				return angleVectors(cosVectors(v), isPositive(v));
			}

			inline T angleVectors(const Vector<T, 2> &v, bool positive) const
			{
				return Vector<T, 2>::angleVectors(cosVectors(v), positive);
			}


			inline T sinVectors(const Vector<T, 2> &v) const
			{
				return sin(angleVectors(v));
			}


			inline T euclidianNorm() const
			{
				return sqrt(pow(this->at(0), 2) + pow(this->at(1), 2));
			}


			inline const Vector<T, 2> &normal(Vector<T, 2> &n) const
			{
				if (this->at(0) != 0)
				{
					n.at(1) = -this->at(0);
					n.at(0) = this->at(1);
				}
				else
				{
					n.at(1) = this->at(0);
					n.at(0) = -this->at(1);
				}

				return n;
			}


			Vector<T, 2> normal() const
			{
				Vector<T, 2> n;

				normal(n);

				return n;
			}


			inline Vector<T, 2> normalize() const
			{
				return *this / this->euclidianNorm();
			}


			std::string doToString() const
			{
				std::ostringstream ret;

				ret << '[' << this->at(0) << ", " << this->at(1) << ']';

				return ret.str();
			}


			std::wstring doToWString() const
			{
				std::wostringstream ret;

				ret << '[' << this->at(0) << ", " << this->at(1) << ']';

				return ret.str();
			}


			std::istream &doFromStream(std::istream &stream)
			{
				char c;
				T val1, val2;
				stream >> c >> val1 >> c >> val2 >> c;
				this->at(0) = val1;
				this->at(1) = val2;

				return stream >> c;
			}



			std::wistream &doFromWStream(std::wistream &stream)
			{
				wchar_t c;
				T val1, val2;
				stream >> c >> val1 >> c >> val2 >> c;
				this->at(0) = val1;
				this->at(1) = val2;

				return stream >> c;
			}


			inline bool isNull() const
			{
				return FDMathCore::isNull(m_arr);
			}


			bool isColinear(const Vector<T, 2> &v, T* factor) const
			{
				if (this->isNull() || v.isNull())
				{
					if (factor != nullptr)
						factor = 0;

					return true;
				}


				size_t i(0);
				T k(0);
				while (i < 2)
				{
					T absU(abs(this->at(i))), absV(abs(v.at(i)));
					if ((isZero(absU) && !isZero(absV))
						|| !isZero(absU) && isZero(absV))
						return false;

					if (!isZero(absU))
					{
						k = v.at(i) / this->at(i);
						++i;
						break;
					}

					++i;
				}

				while (i < 2)
				{
					T absU(abs(this->at(i))), absV(abs(v.at(i)));
					if ((isZero(absU) && !isZero(absV))
						|| !isZero(absU) && isZero(absV))
						return false;

					if (!isZero(absU))
					{
						if (!isEqual(k, (v.at(i) / this->at(i))))
							return false;
					}

					++i;
				}

				if (factor != nullptr)
					*factor = k;

				return true;
			}


			inline static T cosVectors(const T scal, const T normU, const T normV)
			{
				return scal / (normU * normV);
			}


			inline T cosVectors(const Vector<T, 2> &v) const
			{
				return cosVectors(dotProduct(v), euclidianNorm(), v.euclidianNorm());
			}


			inline static bool isOrthogonal(const T scal)
			{
				return isSignedZero(scal);
			}


			inline bool isOrthogonal(const Vector<T, 2> &v) const
			{
				return isOrthogonal(dotProduct(v));
			}

			inline static bool isAcute(const T scal)
			{
				return scal > 0;
			}


			inline bool isAcute(const Vector<T, 2> &v) const
			{
				return isAcute(dotProduct(v));
			}

			inline static bool isObtuse(const T scal)
			{
				return scal < 0;
			}


			inline bool isObtuse(const Vector<T, 2> &v) const
			{
				return isObtuse(dotProduct(v));
			}


			inline bool isNormal(const Vector<T, 2> &n) const
			{
				return isOrtogonal(n);
			}


			inline bool isInternNormal(const Vector<T, 2> &n, const Vector<T, 2> &v) const
			{
				return isOrtogonal(n) && v.isAcute(n);
			}


			inline bool isExternNormal(const Vector<T, 2> &n, const Vector<T, 2> &v) const
			{
				return isOrtogonal(n) && v.isObtuse(n);
			}


			Vector<T, 2> &operator=(const std::initializer_list<T> &l)
			{
				size_t i(0);
				for (auto it(l.begin());
				it != l.end() && i < 2; ++it, ++i)
					m_arr[i] = *it;

				for (; i < 2; ++i)
					m_arr[i] = 0;

				return *this;
			}

			Vector<T, 2> &operator=(const std::array<T, 2> &arr)
			{
				m_arr = arr;
				return *this;
			}

			inline bool operator==(const Vector<T, 2> &v) const
			{
				if (!isEqual(this->at(0), v.at(0)) || !isEqual(this->at(1), v.at(1)))
					return false;

				return true;
			}




			inline bool operator==(const T t) const
			{
				if (t == 0 && isNull())
					return true;

				return false;
			}




			inline bool operator!=(const T t) const
			{
				return !(*this == t);
			}

			inline bool operator!=(const Vector<T, 2> &v) const
			{
				return !(*this == v);
			}



			Vector<T, 2> operator-() const
			{
				Vector<T, 2> v;
				v.at(0) = -this->at(0);
				v.at(1) = -this->at(1);
				return v;
			}



			inline Vector<T, 2> &operator+=(const Vector<T, 2> &v)
			{
				this->at(0) += v.at(0);
				this->at(1) += v.at(1);

				return *this;
			}


			inline Vector<T, 2> &operator+=(const T k)
			{
				this->at(0) += k;
				this->at(1) += k;

				return *this;
			}


			inline Vector<T, 2> &operator-=(const Vector<T, 2> &v)
			{
				this->at(0) -= v.at(0);
				this->at(1) -= v.at(1);

				return *this;
			}


			inline Vector<T, 2> &operator-=(const T k)
			{
				this->at(0) -= k;
				this->at(1) -= k;

				return *this;
			}


			inline Vector<T, 2> &operator*=(const T k)
			{
				this->at(0) *= k;
				this->at(1) *= k;

				return *this;
			}



			inline Vector<T, 2> &operator/=(const T k)
			{
				this->at(0) /= k;
				this->at(1) /= k;

				return *this;
			}



			inline Vector<T, 2> operator+(Vector<T, 2> v) const
			{
				return v += (*this);
			}


			Vector<T, 2> operator+(const T k) const
			{
				Vector<T, 2> v(*this);
				return v += k;
			}


			inline Vector<T, 2> operator-(Vector<T, 2> v) const
			{
				return v -= (*this);
			}


			Vector<T, 2> operator-(const T k) const
			{
				Vector<T, 2> v(*this);
				return v -= k;
			}


			Vector<T, 2> operator*(const T k) const
			{
				Vector<T, 2> v(*this);
				return v *= k;
			}



			inline T operator*(const Vector<T, 2> &v) const
			{
				return this->dotProduct(v);
			}



			Vector<T, 2> operator/(const T k) const
			{
				Vector<T, 2> v(*this);
				return v /= k;
			}

			T &at(size_t i)
			{
				return m_arr.at(i);
			}





			const T &at(size_t i) const
			{
				return m_arr.at(i);
			}





			T &operator[](size_t i)
			{
				return m_arr.at(i);
			}





			const T &operator[](size_t i) const
			{
				return m_arr.at(i);
			}

			const T *data() const
			{
				return m_arr.data();
			}



			T *data()
			{
				return m_arr.data();
			}
	};





	typedef Vector<float, 2> Vect2F;
	typedef Vector<float, 3> Vect3F;

	template<size_t S>
	using VectorF = Vector<float, S>;
}


template<typename T, size_t S>
inline FDMathCore::Vector<T, S> operator*(T k, FDMathCore::Vector<T, S> v)
{
	return v *= k;
}


template<typename T, size_t S>
inline bool operator==(T t, FDMathCore::Vector<T, S> v)
{
	return v == t;
}

template<typename T, size_t S>
inline bool operator!=(T t, FDMathCore::Vector<T, S> v)
{
	return !(v == t);
}

#endif //VECTOR_H