#include "Garbo.h"

Garbo Garbo::garbo_;

Garbo::Garbo() {

}

Garbo::~Garbo() {
	for (auto it = pool_.begin(); it != pool_.end(); ++it) {
		delete *it;
	}
}

void Garbo::autorelease() {
	pool_.push_back(this);
}
