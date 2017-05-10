#ifndef POINT_H
#define POINT_H


#include <initializer_list>
#include <sstream>
#include <array>

#include "FDMathCore.h"

namespace FDMathCore
{
	template<typename T, size_t S>
	class Vector;

	template<typename T, size_t S>
    class Point	{
        private:
            std::array<T, S> m_arr;

		public:

        Point()
		{
            m_arr.fill(0);
		}


        explicit Point(const std::array<T, S> &v) : m_arr(v){}




        explicit Point(const std::initializer_list<T> &l) : Point<T, S>()
        {
            size_t i(0);
            for(auto it(l.begin());
                it != l.end() && i < S; ++it, ++i)
            m_arr[i] = *it;

            for(; i < S; ++i)
                m_arr[i] = 0;
        }



		Point(const std::string &str)
		{
			this->fromString(str);
		}


		Point(const std::wstring &str)
		{
			this->fromWString(str);
		}
		

		explicit Point(const Vector<T, S> &v)
		{
			for (size_t i(0); i < S; ++i)
			{
				this->at(i) = v.at(i);
			}
		}


		inline bool isNull() const
		{
            return FDMathCore::isNull(this->m_arr);
		}



		inline bool isOrigine() const
		{
            return FDMathCore::isNull(this->m_arr);
        }


        T distance(const Point<T, S> &p) const
		{
			T val = 0;
			for (size_t i(0); i < S; ++i)
				val += pow(this->at(i) - p.at(i), 2);
			return sqrt(val);
		}




        std::string doToString() const
		{
			std::ostringstream ret;

			ret << '(' << this->at(0);

			for (size_t i(1); i < S; ++i)
				ret << ", " << this->at(i);

			ret << ')';

			return ret.str();
		}



        std::wstring doToWString() const
		{
			std::wostringstream ret;

			ret << '(' << this->at(0);

			for (size_t i(1); i < S; ++i)
				ret << ", " << this->at(i);

			ret << ')';

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

            return stream >> c;
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

            return stream >> c;
		}



        Point<T, S> &operator=(const std::initializer_list<T> &l)
		{
            size_t i(0);
            for(auto it(l.begin());
                it != l.end() && i < S; ++it, ++i)
            m_arr[i] = *it;

            for(; i < S; ++i)
                m_arr[i] = 0;

			return *this;
        }

        Point<T, S> &operator=(const std::array<T, S> &arr)
        {
            m_arr = arr;
            return *this;
        }

        bool operator==(const Point<T, S> &p) const
		{
			size_t i(0);

			while (i < S)
			{
				if (!isEqual(this->at(i), p.at(i)))
					return false;

				i++;
			}

			return true;
		}


        bool operator==(const std::array<T, S> &arr) const
        {
            size_t i(0);

            while (i < S)
            {
                if (!isEqual(this->at(i), arr.at(i)))
                    return false;

                i++;
            }

            return true;
        }


        inline bool operator==(const T t) const
		{
            if (t == 0 && this->isNull())
				return true;

			return false;
		}


        inline bool operator!=(const T t) const
        {
            return !(*this == t);
        }


		inline bool operator!=(const Point<T, S> &p) const
		{
			return !((*this) == p);
		}


        inline bool operator!=(const std::array<T, S> &arr) const
        {
            return !((*this) == arr);
        }


        Point<T, S> operator-() const
		{
			Point<T, S> p;
			for (size_t i(0); i < S; ++i)
				p.at(i) = -this->at(i);
			return p;
		}



        Point<T, S> &operator+=(const Point<T, S> &p)
		{
			for (size_t i(0); i < S; ++i)
				this->at(i) += p.at(i);

			return *this;
		}



        Point<T, S> &operator-=(const Point<T, S> &p)
		{
			for (size_t i(0); i < S; ++i)
				this->at(i) -= p.at(i);

			return *this;
		}




		template<typename U>
        Point<T, S> &operator+=(const U k)
		{
            for (size_t i(0); i < S; ++i)
				this->at(i) += k;

			return *this;
		}


		template<typename U>
        inline Point<T, S> &operator-=(const U k)
		{
			return (*this) += -k;
		}



		inline Point<T, S> operator+(Point<T, S> p) const
		{
			return p += (*this);
		}


        inline Point<T, S> operator-(const Point<T, S> &p) const
		{
            Point<T, S> ret(*this);
            ret -= p;
            return ret;
		}


        Point<T, S> operator+(const T k) const
		{
			Point<T, S> p(*this);
			return p += k;
		}


        inline Point<T, S> operator-(const T k) const
		{
			return (*this) + (-k);
		}


        Point<T, S> &operator*=(const T k)
        {
            for (size_t i(0); i < S; ++i)
                this->at(i) *= k;

            return *this;
        }


        Point<T, S> &operator/=(const T k)
        {
            for (size_t i(0); i < S; ++i)
                this->at(i) /= k;

            return *this;
        }


        Point<T, S> &operator^=(const T k)
        {
            if(k != static_cast<T>(1))
            {
                for (size_t i(0); i < S; ++i)
                    std::pow(this->at(i), k);
            }

            return *this;
        }


        Point<T, S> operator*(const T k) const
        {
            Point<T, S> p(*this);
            return p *= k;
        }


        Point<T, S> operator/(const T k) const
        {
            Point<T, S> p(*this);
            return p /= k;
        }


        Point<T, S> operator^(const T k) const
        {
            Point<T, S> p(*this);
            return p ^= k;
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
    class Point<T, 0>
    {
        public:
            Point(){throw std::invalid_argument("Error : cannot instantiate Point with size 0");}
    };



    template<typename T>
    class Point<T, 1>
    {
        public:
            Point(){throw std::invalid_argument("Error : cannot instantiate Point with size 1");}
    };




	typedef Point<float, 2> Point2F;
	typedef Point<float, 3> Point3F;

	template<size_t S>
	using PointF = Point<float, S>;

    template<typename T, size_t S, typename Container>
    Point<T, S> barycenter(const Container &c)
    {
        Point<T, S> ret;
        size_t s = c.size();
        for(size_t i = 0; i < s; ++i)
        {
            const Point<T, S> &p = c[i];
            ret += p;
        }

        ret /= static_cast<T>(s);

        return ret;
    }
}



template<typename T, size_t S>
inline FDMathCore::Point<T, S> operator*(T k, FDMathCore::Point<T, S> v)
{
    return v *= k;
}




template<typename T, size_t S>
inline bool operator==(T t, FDMathCore::Point<T, S> v)
{
	return v == t;
}


template<typename T, size_t S>
inline bool operator!=(T t, FDMathCore::Point<T, S> v)
{
    return v != t;
}


#endif // POINT_H
