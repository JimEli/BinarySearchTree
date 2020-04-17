#ifndef SET_H
#define SET_H

#include "tree_with_parent.h"

template <class T>
struct set : public tree<T>
{
	using base_iterator = typename tree<T>::iterator;

	template <typename T>
	void set_union(const set<T>& s, set<T>& result)
	{
		for (base_iterator it = this->begin(); it != this->end(); ++it)
			result.insert(*it);

		if (this == &s)
			return;

		for (base_iterator it = s.begin(); it != s.end(); ++it)
			result.insert(*it);
	}

	template <typename T>
	void set_intersection(const set<T>& s, set<T>& result)
	{
		if (this == &s)
		{
			for (base_iterator it = this->begin(); it != this->end(); ++it)
				result.insert(*it);

			return;
		}

		base_iterator s1 = s.begin(), s2 = this->begin();

		while (s1 != s.end() && s2 != this->end())
		{
			if (*s1 < *s2)
				++s1;
			else if (*s2 < *s1)
				++s2;
			else // if (*s1 == *s2)
			{
				result.insert(*s1);
				++s1;
				++s2;
			}
		}
	}

	template <typename T>
	void set_symmetric_difference(const set<T>& s, set<T>& result)
	{
		if (this != &s)
		{

			base_iterator s1 = this->begin(), s2 = s.begin();

			while (s1 != this->end() && s2 != s.end())
			{
				if (*s1 < *s2)
				{
					result.insert(*s1);
					++s1;
				}
				else if (*s2 < *s1)
				{
					result.insert(*s2);
					++s2;
				}
				else
				{
					++s1;
					++s2;
				}
			}

			for (; s2 != s.end(); ++s2)
				result.insert(*s2);
		}
	}

	template <typename T>
	void set_difference(const set<T>& s, set<T>& result)
	{
		if (this != &s)
		{
			base_iterator s1 = this->begin(), s2 = s.begin();

			while (s1 != this->end() && s2 != s.end())
			{
				if (*s1 < *s2)
				{
					result.insert(*s1);
					++s1;
				}
				else if (*s2 < *s1)
					++s2;
				else
				{
					++s1;
					++s2;
				}
			}

			for (; s1 != this->end(); ++s1)
				result.insert(*s1);
		}
	}

	// Reject identical data.
	void insert(const T data) { if (!tree<T>::search(data)) tree<T>::add(data); }

	T lowerBound() const { return *tree<T>::begin(); }
	T upperBound() const { return *tree<T>::rbegin(); }
};

#endif
