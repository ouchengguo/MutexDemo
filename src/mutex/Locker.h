#ifndef Locker_h
#define Locker_h

template <typename T> class Locker {
public:
	Locker(T& lockable) : m_lockable(lockable) { m_lockable.lock(); }
	~Locker() { m_lockable.unlock(); }
private:
	T& m_lockable;
};

#endif //Locker_h
