#ifndef ID_GENERATE_H
#define ID_GENERATE_H

class id_generate_t {
public:
	id_generate_t();

	~id_generate_t();

	int gen();

private:
	int32 cur_;
};

#endif