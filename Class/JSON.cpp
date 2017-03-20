#include "JSON.h"

rapidjson::Document JSON::document;
char *JSON::json_ = nullptr;
bool JSON::isJSON_ = false;
JSON::~JSON() {
	free(json_);
}