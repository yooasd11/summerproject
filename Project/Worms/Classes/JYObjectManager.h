#include <vector>
#include <string>
class JYObject;

class JYObjectManager{
private:
	JYObjectManager() {
		m_pJYObjectVector.clear();
	}
	~JYObjectManager() {}
	std::vector<JYObject*>::iterator findIteratorByName(const std::string& sName);
	std::vector<JYObject*>::iterator findIteratorByObject(JYObject* const pObject);
public:
	static JYObjectManager* getInstance(){
		static JYObjectManager instance;
		return &instance;
	}
	std::vector<JYObject*> m_pJYObjectVector;

	void pushObject(JYObject* const pObject);
	void popObject(JYObject* const pObject);
	JYObject* findObjectByName(const std::string& sName);
};