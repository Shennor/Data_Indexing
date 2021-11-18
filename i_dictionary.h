#pragma once

template <class TKey, class TElement>
class IDictionary
{
	virtual int GetCount() = 0;
	virtual int GetCapacity() = 0;
	virtual TElement Get(TKey key) = 0;
	virtual bool ContainsKey(TKey key) = 0;
	virtual void Add(TKey key, TElement element) = 0;
	virtual void Remove(TKey key) = 0;
};