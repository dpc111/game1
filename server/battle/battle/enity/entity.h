#ifndef ENTITY_H
#define ENTITY_H

class entity_t {
public:
	entity_t();

	~entity_t();

private:
	int32 type_id_;

	int32 level_;

	int32 cd_;

	int32 blood_;

	grid_t *grid_;
};

#endif