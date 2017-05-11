#ifndef PARAMETRICCURVE_H
#define PARAMETRICCURVE_H

#include <vector>

#include "Point.h"
#include "Vector.h"

namespace FDMathCore
{
	enum CurveConnectionType
	{
		Auto = -1,
		C0 = 0,
		C1 = 1,
		C2 = 2
	};

	enum CurveType
	{
		Bezier,
		BSpline,
		NURBS
	};

	template<typename T, size_t S>
	class ParametricCurve
	{
		public:

#pragma region Member types
			
			typedef std::vector<Point<T, S>> container_type;
			typedef Point<T, S> point_type;
			typedef Point<T, S>& point_reference;
            typedef const Point<T, S>& const_point_reference;
			typedef Point<T, S>* point_pointer;
            typedef const Point<T, S>* const_point_pointer;

			typedef typename container_type::value_type value_type;
			typedef typename container_type::allocator_type allocator_type;
			typedef typename container_type::size_type size_type;
			typedef typename container_type::difference_type difference_type;
			typedef typename container_type::reference reference;
			typedef typename container_type::const_reference const_reference;
			typedef typename container_type::pointer pointer;
			typedef typename container_type::const_pointer const_pointer;


			typedef typename container_type::iterator iterator;
			typedef typename container_type::const_iterator const_iterator;
			typedef typename container_type::reverse_iterator reverse_iterator;
			typedef typename container_type::const_reverse_iterator const_reverse_iterator;

#pragma endregion

		private:

#pragma region Members

			container_type m_controlPoints;
			size_type m_degree;

#pragma endregion

		public:

#pragma region Constructors

			ParametricCurve() : m_degree(0) {}

			explicit ParametricCurve(const container_type &pts) : m_controlPoints(pts), m_degree(0) {}
			explicit ParametricCurve(container_type &&pts) : m_controlPoints(std::move(pts)), m_degree(0) {}
			explicit ParametricCurve(std::initializer_list<point_type> list) : m_degree(0) { m_controlPoints.insert(m_controlPoints.begin(), list); }

			template<typename InputIterator>
			explicit ParametricCurve(InputIterator first, InputIterator last) : m_controlPoints(first, last), m_degree(0) {}

#pragma endregion

#pragma region Assign operator

			ParametricCurve &operator=(const container_type& points) { m_controlPoints = points; return *this; }
			ParametricCurve &operator=(container_type&& points) { m_controlPoints = points; return *this; }
			ParametricCurve &operator=(std::initializer_list<point_type> points) { m_controlPoints = points;  return *this; }

            void assign(size_type count, const_point_reference value) { m_controlPoints.assign(count, value); }
			
			
			template< class InputIt >
			void assign(InputIt first, InputIt last) { m_controlPoints.assign(first, last); }
			
			
			void assign(std::initializer_list<point_type> ilist) { m_controlPoints.assign(ilist); }

			allocator_type get_allocator() const { return m_controlPoints.get_allocator(); }

#pragma endregion

#pragma region Element access

			const_point_reference operator[](size_type index) const { return m_controlPoints[index]; }

			const_point_reference at(size_type index) const { return m_controlPoints.at(index); }

			point_reference operator[](size_type index) { return m_controlPoints[index]; }

			point_reference at(size_type index) { return m_controlPoints.at(index); }

			const_point_reference front() const { return m_controlPoints.front(); }

			const_point_reference back() const { return m_controlPoints.back(); }

			point_reference front() { return m_controlPoints.front(); }

			point_reference back() { return m_controlPoints.back(); }

			const_point_pointer data() const { return m_controlPoints.data(); }

			point_pointer data() { return m_controlPoints.data(); }

#pragma endregion

#pragma region Iterators

			iterator begin() { return m_controlPoints.begin(); }
			const_iterator begin() const { return m_controlPoints.begin(); }
			const_iterator cbegin() const { return m_controlPoints.cbegin(); }
			reverse_iterator rbegin() { return m_controlPoints.rbegin(); }
			const_reverse_iterator rbegin() const { return m_controlPoints.rbegin(); }
			const_reverse_iterator crbegin() const { return m_controlPoints.crbegin(); }

#pragma endregion

#pragma region Capacity

			bool empty() const { return m_controlPoints.empty(); }
			size_type size() const { return m_controlPoints.size(); }

#pragma endregion

#pragma region Modifiers

			void clear() { m_controlPoints.clear(); }

            iterator insert(iterator pos, const_point_reference value) { return m_controlPoints.insert(pos, value); }
            iterator insert(const_iterator pos, const_point_reference value) { return m_controlPoints.insert(pos, value); }
			iterator insert(const_iterator pos, point_type&& value) { return m_controlPoints.insert(pos, value); }
            iterator insert(const_iterator pos, size_type count, const_point_reference value) { return m_controlPoints.insert(pos, count, value); }

			template< class InputIt >
			iterator insert(const_iterator pos, InputIt first, InputIt last) { return m_controlPoints.insert(pos, first, last); }

			iterator insert(const_iterator pos, std::initializer_list<point_type> ilist) { return m_controlPoints.insert(pos, ilist); }

			template< class... Args >
            iterator emplace(const_iterator pos, Args&&... args) { return m_controlPoints.emplace(pos, args...); }

			iterator erase(const_iterator pos) { return m_controlPoints.erase(pos); }
			iterator erase(const_iterator first, const_iterator last) { return m_controlPoints.erase(first, last); }

            void push_back(const_point_reference value) { m_controlPoints.push_back(value); }
			void push_back(point_type&& value) { m_controlPoints.push_back(value); }

			template< class... Args >
            void emplace_back(Args&&... args) { m_controlPoints.emplace_back(args...); }

			void pop_back() { m_controlPoints.pop_back(); }

			void swap(ParametricCurve<T, S> &c) { m_controlPoints.swap(c.m_controlPoints); std::swap(m_degree, c.m_degree); }

#pragma endregion

#pragma region Curve methods

			size_type degree() { return (m_degree == 0 ? (m_controlPoints.empty() ? 0 : m_controlPoints.size() - 1) : m_degree); }

			void setDegree(size_type degree) { m_degree = degree; }

            void addPoint(const_point_reference p) { return m_controlPoints.push_back(p); }

            iterator insertPoint(size_type pos, const_point_reference value) { return m_controlPoints.insert(posToIt(pos), value); }
			iterator insertPoint(size_type pos, point_type&& value) { return m_controlPoints.insert(posToIt(pos), value); }
            iterator insertPoints(size_type pos, size_type count, const_point_reference value) { return m_controlPoints.insert(posToIt(pos), count, value); }

			template< class InputIt >
			iterator insertPoints(size_type pos, InputIt first, InputIt last) { return m_controlPoints.insert(posToIt(pos), first, last); }

			iterator insertPoints(size_type pos, std::initializer_list<point_type> ilist) { return m_controlPoints.insert(posToIt(pos), ilist); }

            void removePoint(const_point_reference p) { m_controlPoints.erase(std::find(m_controlPoints.begin(), m_controlPoints.end(), p)); }

			std::vector<ParametricCurve<T, S>> split(size_type degree) { return this->split(degree, false); }

			std::vector<ParametricCurve<T, S>> split(size_type degree, bool join_rest);

			bool generateNextPoint(CurveConnectionType type)
			{
                return internal_generateNextPoint(type);
			}

            static point_type getAlignedPoint(const T &factor, const_point_reference p1, const_point_reference p2)
			{
				return point_type(p2 + (((p2 - p1) / factor) * (1 - factor)));
			}

			bool generateAlignedPoint(const T &factor)
			{
				if (factor == 0 || m_controlPoints.size() < 2)
					return false;

				m_controlPoints.push_back(getAlignedPoint(factor, m_controlPoints[m_controlPoints.size() - 2], m_controlPoints.back()));
				return true;
			}

            static point_type pointForFactor(const T &factor, const_point_reference p1, const_point_reference p2)
			{
                return (factor == 0 ? p1 : (factor == 1 ? p2 : (p2 * factor) + (p1 * (1 - factor))));
			}

			static container_type pointsForList(const T &factor, const container_type &inPoints);

            point_type buildPoint(const T &factor) const
			{
                return buildPoint(factor, m_controlPoints);
			}

            point_type buildPoint(const T &factor, const container_type &inPoints) const;


            container_type build(size_type subdivisions) const { return internal_build(subdivisions); }

            container_type build(const T &factor) const { return internal_build(factor); }

            container_type build(const std::vector<T> &knot) const { return internal_build(knot); }


#pragma endregion

		protected:
			iterator posToIt(size_type pos)
			{
				iterator it = m_controlPoints.begin();
				std::advance(it, pos);
				return it;
			}

			const_iterator posToIt(size_type pos) const
			{
				const_iterator it = m_controlPoints.begin();
				std::advance(it, pos);
				return it;
			}

			static CurveConnectionType getBestConnectionType(const ParametricCurve<T, S> &a, const ParametricCurve<T, S> b)
			{
				size_type degree = std::min(a.degree(), b.degree());
				switch (degree)
				{
					case 2:
						return C1;

					case 1:
					case 0:
						return C0;

					default:
						return C2;
				}
			}

            bool internal_generateNextPoint(CurveConnectionType _type)
			{
                switch (_type)
				{
                    case C1:
                        return internal_generateNextPointC1();
					case C2:
                        return internal_generateNextPointC2();

                    default:
                    case C0:
						return false;
				}
			}

            bool internal_generateNextPointC1()
			{
				if (m_controlPoints.size() < 3)
					return false;

                const_point_reference p1(m_controlPoints[m_controlPoints.size() - 2]), &p2(m_controlPoints[m_controlPoints.size() - 1]);
				m_controlPoints.push_back(((p2 - p1) + p2));

				return true;
			}

            bool internal_generateNextPointC2()
			{
				if (m_controlPoints.size() < 4)
					return false;

                const_point_reference p0(m_controlPoints[m_controlPoints.size() - 4]), &p1(m_controlPoints[m_controlPoints.size() - 3]), &p2(m_controlPoints[m_controlPoints.size() - 2]), &p3(m_controlPoints[m_controlPoints.size() - 1]);

				T factor = 0;
				if (!Vector<T, S>(p1, p3).isColinear(Vector<T, S>(p1, p2), &factor))
					return false;

				point_type pivot = p1 + (((p1 - p0) / factor) * (1 - factor));

				m_controlPoints.push_back(p3 + (((p3 - pivot) / factor) * (1 - factor)));

				return true;
			}

			bool checkKnot(const std::vector<T> &knot, bool clamped);

            container_type internal_build(size_type subdivisions) const { return subdivisions == 0 ? container_type() : internal_build(static_cast<T>(1.0) / subdivisions); }
		
            container_type internal_build(const T &factor) const;

            container_type internal_build(const std::vector<T> &knot) const
			{
                //if (m_controlPoints.size() < 2 || knot.size() != m_controlPoints.size()*2)
					return container_type();

                /*for (size_type i = 1, imax = knot.size(); i < imax; ++i)
				{
					if (knot[i - 1] > knot[i])
						return container_type();
				}

				container_type pts;
				while (factor < 1.0f)
				{
					pts.push_back(buildPoint(factor, m_controlPoints));
					factor += factor;
				}

				pts.push_back(buildPoint(1, curves[nbSubCurves - 1]));

                return pts;*/
			}
	};
	
	template<typename T, size_t S>
	std::vector<ParametricCurve<T, S>> ParametricCurve<T, S>::split(size_type degree, bool join_rest)
	{
		if (this->degree() <= degree)
			return std::vector<ParametricCurve<T, S>>(1, *this);

		std::vector<ParametricCurve<T, S>> v;
		v.resize(this->degree() / degree);

		const_iterator from = m_controlPoints.begin(), to = m_controlPoints.begin(), end = m_controlPoints.end();
		std::advance(to, degree + 1); // N degree curves have N + 1 points
		while (from < end)
		{
			v.push_back(ParametricCurve<T, S>(from, to));
			std::advance(from, degree); // The last point of the current curve is the first point of the next one
			std::advance(to, degree);
			if (to > end || (join_rest && to > end - degree)) // Avoid out of range error and join last points if required
				to = end;
		}

		return v;
	}

	template<typename T, size_t S>
	bool ParametricCurve<T, S>::checkKnot(const std::vector<T> &knot, bool clamped)
	{
		const size_type order = degree() + 1;
		if (m_controlPoints.size() < 2 || knot.size() != m_controlPoints.size() + order)
			return false;

		for (size_type i = 1, imax = knot.size(); i < imax; ++i)
		{
			if (knot[i - 1] > knot[i])
				return container_type();
		}

		if (!clamped)
			return true;

		size_type i = 0, j = knot.size() - 1, imax = order, jmax = knot.size() - 1 - order;
		while (i < imax && j > jmax)
		{
			if (knot[i] != 0 || knot[j] != 1)
				return false;
			++i;
			--j;
		}

		return true;
	}

	template<typename T, size_t S>
    typename ParametricCurve<T, S>::container_type ParametricCurve<T, S>::pointsForList(const T &factor, const container_type &inPoints)
	{
		container_type outPoints;
		for (size_type i(1), imax(inPoints.size()); i < imax; ++i)
		{
            outPoints.push_back(pointForFactor(factor, inPoints[i - 1], inPoints[i]));
		}

		return outPoints;
	}

	template<typename T, size_t S>
    typename ParametricCurve<T, S>::point_type ParametricCurve<T, S>::buildPoint(const T &factor, const container_type &inPoints) const
	{
		container_type points(inPoints.begin(), inPoints.end());

		while (points.size() > 1)
		{
			points = pointsForList(factor, points);
		}

		return points.front();
	}

	template<typename T, size_t S>
    typename ParametricCurve<T, S>::container_type ParametricCurve<T, S>::internal_build(const T &factor) const
	{
		if (m_controlPoints.size() < 2 || factor == 0)
			return container_type();

        container_type pts;
        T current = 0;
        while (current < 1.0f)
		{
            pts.push_back(buildPoint(current, m_controlPoints));
            current += factor;
		}

        pts.push_back(m_controlPoints.back());

		return pts;
	}

	template<typename T>
	std::vector<std::vector<Point<T, 3>>> generatePointMatrix(std::vector<typename ParametricCurve<T, 3>::container_type> pointLists)
	{
		// In this function we transpose the lists to create a matrix with the curves as columns
		std::vector<std::vector<Point<T, 3>>> ret;
        ret.reserve(pointLists.size() * pointLists.front().size());
		for (size_t i = 0, imax = pointLists.front().size(); i < imax; ++i)
		{
			ret.push_back(std::vector<Point<T, 3>>());
            for (size_t j = 0, jmax = pointLists.size(); j < jmax; ++j)
                ret[i].push_back(pointLists[j][i]);
		}

		return ret;
    }

	template<typename T>
    std::vector<Point<T, 3>> generateSurface(const std::vector<ParametricCurve<T, 3>> &curves, typename ParametricCurve<T, 3>::size_type subdivisions)
	{
        if (curves.size() < 2)
			return std::vector<Point<T, 3>>();

		std::vector<std::vector<Point<T, 3>>> matrix;
		{
			std::vector<typename ParametricCurve<T, 3>::container_type> pointLists;
			for (auto it = curves.begin(), end = curves.end(); it != end; ++it)
                pointLists.push_back(it->build(subdivisions));

            matrix = generatePointMatrix<T>(pointLists);
		}
		
		std::vector<Point<T, 3>> ret;
		for (auto it = matrix.begin(), end = matrix.end(); it != end; ++it)
		{
            std::vector<Point<T, 3>> tmp = ParametricCurve<T, 3>(*it).build(subdivisions);
            ret.insert(ret.end(), tmp.begin(), tmp.end());
        }

        return ret;
	}
}


namespace std
{
	template<typename T,  size_t S>
	void swap(FDMathCore::ParametricCurve<T, S> &a, FDMathCore::ParametricCurve<T, S> &b)
	{
		a.swap(b);
	}
}


#endif // PARAMETRICCURVE_H
