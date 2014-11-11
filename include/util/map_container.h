#include <map>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

template<typename _key, typename _value>
class MapContainer
{
public:
	typedef boost::unordered_map<_key, _value> CacheContainer;
	MapContainer(void) {};
	~MapContainer(void) {};

	bool Insert(_key& key, _value value);
	_value* Getvalue(_key key);
	std::vector<_value*> AllValue();

	CacheContainer GetContainer();
protected:
	

private:
	_value* Find(_key& key);
	CacheContainer m_container;
};

template<typename _key, typename _value>
_value* MapContainer<_key, _value>::Find(_key& key)
{
	auto itr = m_container.find(key);
	if (itr != m_container.end())
	{
		return &itr->second;
	}

	return 0;	
}

template<typename _key, typename _value>
bool MapContainer<_key, _value>::Insert(_key& key, _value value)
{
	// 컨테이너에 데이터 존재하지 않으면 데이터 삽입한다.
	_value* p = Find(key);
	if (!p)
	{
		m_container.insert(std::make_pair(key, value));	
		return true;
	}

	return false;
}

template<typename _key, typename _value>
_value* MapContainer<_key, _value>::Getvalue(_key key)
{
	// 컨테이너에 데이터가 존재하면 데이터를 리턴해준다.
	_value* p = Find(key);
	if (p)
	{
		return p;
	}

	return 0;
}

template<typename _key, typename _value>
CacheContainer MapContainer<_key, _value>::GetContainer()
{
	return m_container;
}

template<typename _key, typename _value>
std::vector<_value*> MapContainer<_key, _value>::AllValue()
{
	std::vector<_value*> result;
	for(auto value : m_container)
	{
		result.push_back(&value.second);
	}
	
	return result;
}