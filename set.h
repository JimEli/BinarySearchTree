#include "tree_with_parent.h"

template <class T>
struct set : public tree<T>
{
	template <typename T>
	void set_union(const set<T>& s, set<T>& result)
	{
		for (auto i = this->begin(); i != this->end(); ++i)
			result.insert(*i);

		if (this == &s)
			return;

		for (auto i = s.begin(); i != s.end(); ++i)
			result.insert(*i);
	}

	template <typename T>
	void set_intersection(const set<T>& s, set<T>& result)
	{
		if (this == &s)
		{
			for (auto it = this->begin(); it != this->end(); ++it)
				result.insert(*it);

			return;
		}

		auto s1 = s.begin();
		auto s2 = this->begin();

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
	void set_difference(const set<T>& s, set<T>& result)
	{
		if (this != &s)
		{

			auto s1 = this->begin();
			auto s2 = s.begin();

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

			for (auto it = s2; it != s.end(); ++it)
				result.insert(*it);
		}
	}

	void insert(const T data) { if (!tree<T>::search(data)) tree<T>::add(data); }

	T lowerBound() const { return *tree<T>::begin(); }
	T upperBound() const { return *tree<T>::rbegin(); }
};
