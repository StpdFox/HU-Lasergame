#ifndef EMBEDDEDMAP_HPP
#define EMBEDDEDMAP_HPP
#include <map>
class EmbeddedMap : map
{
private:
	EmbeddedMap(const EmbeddedMap& rhs);
	EmbeddedMap& operator=(const EmbeddedMap& rhs);

public:
	EmbeddedMap(uint16_t map_size, bool index_id, bool pre_create);
	~EmbeddedMap();
	
};

#endif // EMBEDDEDMAP_HPP
