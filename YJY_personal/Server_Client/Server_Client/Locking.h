class LOCKING{
private:
	CRITICAL_SECTION cs;
public:
	LOCKING(){
		InitializeCriticalSection(&(this->cs));
	}
	~LOCKING(){
		DeleteCriticalSection(&(this->cs));
	}
	void LOCK(){
		EnterCriticalSection(&(this->cs));
		return;
	}
	void UNLOCK(){
		LeaveCriticalSection(&(this->cs));
		return;
	}
};