#include <vector>
#include <string>
#include "JYObject.h"

class JYObjectManager{
private:
	JYObjectManager() {
		m_pJYObjectVector.clear();
	}
	~JYObjectManager() {}
	std::vector<JYObject*>::iterator findIteratorByName(const std::string& sName);
	std::vector<JYObject*>::iterator findIteratorByObject(JYObject* const pObject);
	std::vector<JYObject*>::iterator findIteratorByUID(const UINT& nUID);
public:
	static JYObjectManager* getInstance(){
		static JYObjectManager instance;
		return &instance;
	}
	std::vector<JYObject*> m_pJYObjectVector;

	void pushObject(JYObject* const pObject);
	void popObject(JYObject* const pObject);
	JYObject* findObjectByName(const std::string& sName);
	JYObject* findObjectByUID(const UINT& nUID);
};