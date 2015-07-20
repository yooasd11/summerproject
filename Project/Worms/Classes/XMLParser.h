#include "tinyxml2\tinyxml2.h"
#include "cocos2d.h"

class XMLParser{
private:
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* findElement(const char*);
public:
	XMLParser();
	XMLParser(const char* filename);
	tinyxml2::XMLElement* getChildByID(const char*, const char*);
	tinyxml2::XMLElement* getFirstChildByName(const char*);
};

