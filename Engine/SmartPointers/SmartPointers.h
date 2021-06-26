#pragma once
#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

template<typename T>
class StrongPtr;

template<typename T>
class WeakPtr;

struct Counters {
	unsigned long Owners;
	unsigned long Observers;

	Counters(unsigned long i_initialOwners, unsigned long i_initialObservers) :
		Owners(i_initialOwners),
		Observers(i_initialObservers)
	{ }
};

template<typename T>
class StrongPtr {
public:
	StrongPtr(T* i_ptr) :
		m_ptr(i_ptr),
		m_pReferenceCounters(new Counters(1, 0))
	{ }

	StrongPtr(const StrongPtr<T>& i_other) :
		m_ptr(i_other.m_ptr),
		m_pReferenceCounters(i_other.m_pReferenceCounters)
	{
		m_pReferenceCounters->Owners++;
	}

	StrongPtr(const WeakPtr<T>& i_other) :
		m_ptr(i_other.m_ptr),
		m_pReferenceCounters(i_other.m_pReferenceCounters)
	{
		assert(m_pReferenceCounters->Owners > 0);
		m_pReferenceCounters->Owners++;
	}


	~StrongPtr() {
		m_pReferenceCounters->Owners--;
		if (m_pReferenceCounters->Owners == 0) {
			delete m_ptr;

			if ((m_pReferenceCounters->Observers) == 0) {
				delete m_pReferenceCounters;
			}
		}

	}

	T* GetPtr() {
		return m_ptr;
	}


	T* operator->() {
		return m_ptr;
	}

	T& operator*() {
		assert(m_ptr);
		return *m_ptr;
	}

	operator bool() {
		return m_pReferenceCounters->Owners > 0 && m_ptr != nullptr;
	}

	bool operator==(StrongPtr<T>& i_other) {
		return m_ptr == i_other.m_ptr;
	}

	bool operator!=(StrongPtr<T>& i_other) {
		return m_ptr != i_other.m_ptr;
	}

	bool operator==(WeakPtr<T>& i_other) {
		return m_ptr == i_other.m_ptr;
	}

	bool operator!=(WeakPtr<T>& i_other) {
		return m_ptr != i_other.m_ptr;
	}

	void operator=(StrongPtr<T>&& i_other) {
		m_ptr = i_other.m_ptr;
		m_pReferenceCounters = i_other.m_pReferenceCounters;
	}

	void operator=(WeakPtr<T>&& i_other) {
		m_ptr = i_other.m_ptr;
		m_pReferenceCounters = i_other.m_pReferenceCounters;
	}


private:
	T* m_ptr;
	Counters* m_pReferenceCounters;

	template<typename T>
	friend class WeakPtr;
};

template<typename T>
class WeakPtr {

public:
	WeakPtr(const WeakPtr<T>& i_other) :
		m_ptr(i_other.m_ptr),
		m_pReferenceCounters(i_other.m_pReferenceCounters)
	{
		m_pReferenceCounters->Observers++;
	}

	WeakPtr(const StrongPtr<T>& i_other) :
		m_ptr(i_other.m_ptr),
		m_pReferenceCounters(i_other.m_pReferenceCounters)
	{
		m_pReferenceCounters->Observers++;
	}

	~WeakPtr() {
		m_pReferenceCounters->Observers--;

		if (m_pReferenceCounters->Observers == 0 && m_pReferenceCounters->Owners == 0) {
			delete m_pReferenceCounters;
		}
	}

	StrongPtr<T> CreateOwner() {
		if (m_pReferenceCounters->Owners > 0) {
			return StrongPtr<T>(*this);
		}
		else {
			return StrongPtr<T>(nullptr);
		}
	}

	T* GetPtr() {
		return m_ptr;
	}

	operator bool() {
		return m_pReferenceCounters->Owners > 0 && m_ptr != nullptr;
	}

	bool operator==(StrongPtr<T>& i_other) {
		return m_ptr == i_other.m_ptr;
	}

	bool operator!=(StrongPtr<T>& i_other) {
		return m_ptr != i_other.m_ptr;
	}

	bool operator==(WeakPtr<T>& i_other) {
		return m_ptr == i_other.m_ptr;
	}

	bool operator!=(WeakPtr<T>& i_other) {
		return m_ptr != i_other.m_ptr;
	}

	void operator=(StrongPtr<T>&& i_other) {
		m_ptr = i_other.m_ptr;
		m_pReferenceCounters = i_other.m_pReferenceCounters;
	}

	void operator=(WeakPtr<T>&& i_other) {
		m_ptr = i_other.m_ptr;
		m_pReferenceCounters = i_other.m_pReferenceCounters;
	}

private:
	T* m_ptr;
	Counters* m_pReferenceCounters;

	template<typename T>
	friend class StrongPtr;
};
