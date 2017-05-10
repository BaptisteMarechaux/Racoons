#ifndef FDMATHCORE_H
#define FDMATHCORE_H


/**
 * @def ERRORMARGIN represents the epsilon accepted by the library
 * @details ERRORMARGIN represents the epsilon accepted by the library. Every number into the interval
 * [-ERRORMARGIN, +ERRORMARGIN] is interpreted as 0 and if two number a and b, with @code abs(a - b) <= ERRORMARGIN @endcode
 * a is considered equal to b.
 * @remark the default epsilon is 0.000001
 */
#ifndef ERRORMARGIN
    /**
      * @def STDERRORMARGIN represents the system's defined epsilon for a given type
      * @details STDERRORMARGIN represents the system's defined epsilon for a given type,
      * the value of the epsilon is @code std::numeric_limits<T>::epsilon() @endcode.
      *
      * @remark STDERRORMARGIN is used only if ERRORMARGIN is not defined
      */
    #ifndef STDERRORMARGIN
        /**
         * @def NOERRORMARGIN means that the library accepts no epsilon.
         * @details NOERRORMARGIN means that the library accepts no epsilon. So every check expect a strict equality
         * between the two numbers.
         * @remark STDERRORMARGIN and NOERRORMARGIN is used only if ERRORMARGIN is not defined
         * @remark NOERRORMARGIN will set ERRORMARGIN to 0
         */
        #ifndef NOERRORMARGIN
            #define ERRORMARGIN 0.000001
        #else
            #define ERRORMARGIN 0
        #endif
    #endif
#endif


#include <limits>
#include <array>
#include <cmath>

/**
 * @namespace FDMathCore all the math utils functions and objects used in most of the FD framework's libraries
 */
namespace FDMathCore
{
    /**
     * @brief getPi returns the value representing pi for the type T
     * @return the value representing pi for the type T
     */
    template<typename T>
    T getPi()
    {
        return acos(static_cast<T>(-1));
    }


    template<typename T, typename std::enable_if<std::is_signed<T>::value, signed>::type = 0>
    T internal_abs(const T &val)
    {
        return val < 0 ? -val : val;
    }

	template<typename T, typename std::enable_if<!std::is_signed<T>::value, unsigned>::type = 0>
	T internal_abs(const T &val)
	{
		return val;
	}

    /**
     * @brief abs return the absolute value of @param val
     * @param val The value to transform into positive a value
     * @return if @param val >= 0 val else -val
     */
    template<typename T>
    T abs(const T &val)
    {
        return internal_abs(val);
    }

    /**
     * @brief getErrorMargin returns the epsilon of the library
     * @return ERRORMARGIN or std::numeric_limits<T>::epsilon()
     */
    template<typename T>
    const T getErrorMargin()
    {
        #ifdef ERRORMARGIN
            return static_cast<T>(ERRORMARGIN);
        #else
            return std::numeric_limits<T>::epsilon();
        #endif
    }


    /**
     * @brief getInfinity returns the representation of the infinity value for the given type
     * @return the representation of the infinity value for the given type
     */
    template<typename T>
    const T& getInfinity()
    {
        static const T inf = std::numeric_limits<T>::infinity();
        return inf;
    }


    template<typename T, typename std::enable_if<std::is_signed<T>::value, signed>::type = 0>
    inline bool internal_isEqual(const T& a, const T& b)
    {
        return abs(a - b) <= getErrorMargin<T>();
    }


	template<typename T, typename std::enable_if<!std::is_signed<T>::value, unsigned>::type = 0>
	inline bool internal_isEqual(const T& a, const T& b)
	{
		return (a > b) ? (a - b) <= getErrorMargin<T>() : (b - a) <= getErrorMargin<T>();
	}

    /**
     * @brief isEqual checks if @code abs(a - b) <= ERRORMARGIN @endcode
     * @param a the first value to check
     * @param b the second value to check
     * @return abs(a - b) <= ERRORMARGIN
     */
    template<typename T>
    inline bool isEqual(const T& a, const T& b)
    {
        return internal_isEqual(a, b);
    }


    template<typename T, typename std::enable_if<!std::is_signed<T>::value, unsigned>::type = 0>
    inline bool internal_isZero(const T& x)
    {
        return (x <= getErrorMargin<T>());
    }

	template<typename T, typename std::enable_if<std::is_signed<T>::value, signed>::type = 0>
	inline bool internal_isZero(const T& x)
	{
		const T e = getErrorMargin<T>();
		return x <= e && x >= -e;
	}

    /**
     * @brief isZero checks if @code x <= ERRORMARGIN @endcode
     * @param x the value to check
     * @return x <= ERRORMARGIN
     *
     */
    template<typename T>
    inline bool isZero(const T& x)
    {
        return internal_isZero(x);
    }


    /**
     * @brief isSignedZero checks if @code abs(x) <= ERRORMARGIN @endcode
     * @param x the value to check
     * @return abs(x) <= ERRORMARGIN
     */
    template<typename T>
    bool isSignedZero(const T& x)
    {
        return isZero(x);
    }

	
	template<typename T, typename std::enable_if<std::is_signed<T>::value, unsigned>::type = 0>
    inline bool internal_isInfinity(const T& x)
    {
        return x == getInfinity<T>();
    }

	template<typename T, typename std::enable_if<!std::is_signed<T>::value, unsigned>::type = 0>
	inline bool internal_isInfinity(const T& x)
	{
		return x == getInfinity<T>() || -x == getInfinity<T>();
	}

    /**
     * @brief isInfinity checks if @code x == getInfinity<T>() @endcode
     * @param x the value to check
     * @return x == getInfinity<T>()
     *
     * @remark use isSignedInfinity for signed types
     */
    template<typename T>
    inline bool isInfinity(const T& x)
    {
        return internal_isInfinity(x);
    }


    /**
     * @brief isSignedInfinity checks if @code abs(x) == getInfinity<T>() @endcode
     * @param x the value to check
     * @return x == getInfinity<T>()
     */
    template<typename T>
    inline bool isSignedInfinity(const T& x)
    {
        return isInfinity(abs(x));
    }


    /**
     * @brief isInRange checks if the parameter @code val @endcode is in range [min, max]
     * @param val the value to check
     * @param min the minimum value of the interval
     * @param max the maximum value of the interval
     * @return true if val is in range [min, max], false otherwwise
     */
    template<typename T>
    inline bool isInRange(const T& val, const T& min, const T& max)
    {
        return (val >= min || isEqual(val, min)) && (val <= max || isEqual(val, max));
    }


    /**
     * @brief factorial returns the mathematics factorial value of n (ie: !n)
     * @param n the value to turn in factorial. n Must be a positive or null integer
     * @return n * (n - 1) * ... * 1
     */
    template<typename T>
    T factorial(T n)
    {
        if(n < 0)
            throw std::invalid_argument("!n is defined only for positive or null integers");


        {
            long double d = static_cast<long double>(n);
            if(!isEqual(d, floorl(d)))
                throw std::invalid_argument("!n is defined only for positive or null integers");
        }

        if(n < 2)
            return static_cast<T>(1);

        T ret = n;
        n -= static_cast<T>(1);
        while(n > 1)
        {
            ret *= n;
            n -= static_cast<T>(1);
        }

        return ret;
    }


    /**
     * @brief intervalProduct returns the mathematics product of every integer in the range [a, b]
     * @param a the minimum value of the interval. a Must be a positive integer
     * @param b the minimum value of the interval. b Must be a positive integer
     * @return n * (n - 1) * ... * 1
     */
    template<typename T>
    T intervalProduct(T a, T b)
    {
        if(a > b)
            return intervalProduct(b, a);

        if(a <= 0 || b <= 0)
            throw std::invalid_argument("interval product is defined only for positive integers");


        {
            long double da = static_cast<long double>(a);
            long double db = static_cast<long double>(b);
            if(!isEqual(da, floorl(da)) || !isEqual(db, floorl(db)))
                throw std::invalid_argument("interval product is defined only for positive integers");
        }

        if(a == b)
            return a;

        T ret = a;
        a += static_cast<T>(1);
        while(a <= b)
        {
            ret *= a;
            a += static_cast<T>(1);
        }

        return ret;
    }


    /**
     * @brief triangularNumber returns the sum of every integer from 1 to n
     * @param n the max number of the sum
     * @return the sum of every integer from 1 to n
     */
    template<typename T>
    T triangularNumber(T n)
    {
        if(n <= 0)
            throw std::invalid_argument("tn(n) is defined only for positive integers");


        {
            long double d = static_cast<long double>(n);
            if(d != floorl(d))
                throw std::invalid_argument("tn(n) is defined only for positive integers");
        }

        T ret = n;
        n -= static_cast<T>(1);
        while(n > 0)
        {
            ret += n;
            n -= static_cast<T>(1);
        }

        return ret;
    }



    /**
     * @brief intervalSum returns the sum of every integer between a and b
     * @param a the minimum number of the interval
     * @param b the maximum number of the interval
     * @return the sum of every integer between a and b
     */
    template<typename T>
    T intervalSum(T a, T b)
    {
        if(a > b)
            return intervalSum(b, a);

        if(a <= 0 || b <= 0)
            throw std::invalid_argument("interval sum is defined only for positive integers");


        {
            long double da = static_cast<long double>(a);
            long double db = static_cast<long double>(b);
            if(!isEqual(da, floorl(da)) || !isEqual(db, floorl(db)))
                throw std::invalid_argument("interval sum is defined only for positive integers");
        }

        if(a == b)
            return a;

        T ret = a;
        a += static_cast<T>(1);
        while(a <= b)
        {
            ret += a;
            a += static_cast<T>(1);
        }

        return ret;
    }



    /**
     * @brief isNull checks if all elements of an array are equals to zero
     * @param A the array to check
     * @return false if any element of the array is not null
     */
    template<typename T, size_t S>
    bool isNull(const std::array<T, S> &A)
    {
        size_t i(0);

        while (i < S)
        {
            if (!isSignedZero(A.at(i)))
                return false;

            i++;
        }

        return true;
    }

    /**
     * @brief permutations returns the number of possible permutations of nbElement elemetns into a set of elementSetCounts
     * @param nbElement the number of possible elements
     * @param elementSetCount the number of desired elements
     * @return the number of possible permutations of nbElement elemetns into a set of elementSetCounts
     */
    template<typename T>
    T permutations(T nbElement, T elementSetCount)
    {
        if(nbElement <= 0 || elementSetCount <= 0)
            return 0;
        if(nbElement > elementSetCount)
            return 0;
        else if(nbElement == elementSetCount)
           return 1;

        T ret = elementSetCount;
        elementSetCount -= 1;
        while (elementSetCount > nbElement)
            ret *= nbElement;

        return ret;
    }

    /**
     * @brief combinaisons returns the number of possible combinaisons of nbElement elemetns into a set of elementSetCounts
     * @param nbElement the number of possible elements
     * @param elementSetCount the number of desired elements
     * @return the number of possible combinaisons of nbElement elemetns into a set of elementSetCounts
     */
    template<typename T>
    T combinaisons(T nbElement, T elementSetCount)
    {
        if(elementSetCount - nbElement < elementSetCount)
            return combinaisons(elementSetCount - nbElement, elementSetCount);

        if(nbElement <= 0 || elementSetCount <= 0)
            return 0;
        if(nbElement > elementSetCount)
            return 0;
        else if(nbElement == elementSetCount)
           return 1;

        T ret = elementSetCount;
        elementSetCount -= 1;
        while (elementSetCount > nbElement)
            ret *= nbElement;

        return ret;
    }
}



#endif //FDMATHCORE_H
