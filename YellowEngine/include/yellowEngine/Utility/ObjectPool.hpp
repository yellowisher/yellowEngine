#ifndef __H_OBJECTPOOL__
#define __H_OBJECTPOOL__

#include <string>

namespace yellowEngine
{
	typedef int ObjectId;
	const int NullObject = -1;

	class Poolable
	{
	public:
		virtual void constructor() = 0;
		virtual void destructor() = 0;
	};

	template <class T>
	class ObjectPool
	{
	public:
		ObjectPool(int capacity = 4);
		~ObjectPool();

		ObjectId createObject();
		void returnObject(ObjectId id);

		// should not store T&
		inline T& getObject(ObjectId id);
		T& operator[](ObjectId id);

		const int& size;
		const int& capacity;

	private:
		T* _data;
		int _capacity;
		int _size;
		int _freeHead;
	};


	template <class T>
	ObjectPool<T>::ObjectPool(int capacity) :size(_size), capacity(_capacity)
	{
		_capacity = capacity;
		_data = new T[capacity];
		_size = 0;
		_freeHead = 0;
		for (int i = 0; i < capacity - 1; i++)
		{
			_data[i].next = i + 1;
		}
		_data[capacity - 1].next = NullObject;
	}


	template <class T>
	ObjectPool<T>::~ObjectPool()
	{
		delete[](_data);
	}


	template <class T>
	int ObjectPool<T>::createObject()
	{
		if (_freeHead == NullObject)
		{
			int capacity = _capacity * 2;
			T* newData = new T[capacity];
			memcpy(newData, _data, sizeof(T) * _capacity);
			_data = newData;

			for (int i = _capacity; i < capacity - 1; i++)
			{
				_data[i].next = i + 1;
			}
			_data[_capacity - 1].next = NullObject;
			_freeHead = _capacity;
			_capacity = capacity;
		}

		_size++;
		ObjectId newObject = _freeHead;
		_freeHead = _data[_freeHead].next;

		_data[newObject].constructor();
		return newObject;
	}


	template <class T>
	void ObjectPool<T>::returnObject(ObjectId id)
	{
		_size--;
		_data[id].destructor();
		_data[id].next = _freeHead;
		_freeHead = id;
	}


	template <class T>
	T& ObjectPool<T>::operator[](ObjectId id)
	{
		return getObject(id);
	}


	template <class T>
	T& ObjectPool<T>::getObject(ObjectId id)
	{
		return _data[id];
	}
}

#endif