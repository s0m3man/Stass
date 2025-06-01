#pragma once

#include "Main.hpp"

namespace Stass {
	// The index of nonexistent object in array
	constexpr int nullIndex = -2147483648i32;

	// The dynamic array implementation
	template <typename Type>
	class Array {
	public:
		// Returns an array with given length and null initialized objects
		static Array<Type> Sized(int length) {
			Array<Type> NewArr = {};

			NewArr.SetLength(length);

			return NewArr;
		}

		// Returns an array with given length and initialized objects by given copy
		static Array<Type> Filled(const Type& Object, int count) {
			Array<Type> NewArr = {};

			NewArr.SetLength(count);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = Object;

			return NewArr;
		}

		// Returns an array whith copied objects from a pointer array with its given length
		static Array<Type> Copied(const Type* ptrArray, unsigned int ptrArrayLength) {
			Array<Type> NewArr = {};

			NewArr.SetLength((int)ptrArrayLength);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = ptrArray[i];

			return NewArr;
		}

		// Returns an array whith copied objects from a pointer array with given range
		static Array<Type> CopiedInRange(const Type* ptrArray, unsigned int from, unsigned int to) {
			Array<Type> NewArr = {};

			NewArr.SetLength(int(to - from) + 1);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = ptrArray[i + from];

			return NewArr;
		}

		// Basic constructor
		Array() {};

		// Construction by list
		Array(const std::initializer_list<Type>& list) {
			Create((int)list.size());

			for (int i = 0; i < length; i++)
				pointer[i] = *(list.begin() + i);
		}

		// Copy constructor
		Array(const Array<Type>& Array) {
			operator=(Array);
		}

		// Destructor
		~Array() {
			Clear();
		}

		// Basic assign operator. Assigns all objects from other array.
		Array<Type>& operator =(const Array<Type>& Array) {
			Create(Array.length);

			for (int i = 0; i < length; i++)
				pointer[i] = Array[i];

			return *this;
		}

		// Adds objects from other array at the end of this array
		Array<Type>& operator +=(const Array<Type>& Array) {
			AddMany(Array);

			return *this;
		}

		// Returns a copy of this array with objects from other array at the end
		Array<Type>& operator +(const Array<Type>& OtherArray) const {
			Array<Type> NewArray = *this;

			NewArray += OtherArray;

			return NewArray;
		}

		// Returns true if array has objects
		operator bool() const {
			return (bool)length;
		}

		// Returns true if array has no objects
		bool isEmpty() const {
			return !operator bool();
		}

		// Returns length of array
		int getLength() const {
			return length;
		}

		// Returns the last index of array
		int GetLastIndex() const {
			return length - 1;
		}

		// Returs a pointer to the internal array
		operator const Type* () const {
			return toPtrArray();
		}

		// Returs a pointer to the internal array
		const Type* toPtrArray() const {
			return pointer;
		}

		// Returns a reference to the object of array by index. Index value will be normalized to array bounds
		Type& operator [](int index) const {
			if constexpr (enabledDebug)
				EmptyWarning();

			SetRealIndex(index);

			return pointer[index];
		}

		// Returns a pointer to the first element of array
		const Type* begin() const {
			return pointer;
		}

		// Returns a pointer to the abstract object placed after the last object of array
		const Type* end() const {
			return pointer + length;
		}

		// Deletes all objects in array
		void Clear() {
			if (!length)
				return;

			length = 0;

			delete[] pointer;

			pointer = nullptr;
		}

		// Creates a copy of array
		Type* CreatePtrArray() const {
			return CreatePtrArray(0, length - 1);
		}

		// Creates a copy of array within range. Range values will be normalized by array`s bounds.
		Type* CreatePtrArray(int from, int to) const {
			if constexpr (enabledDebug)
				EmptyWarning();

			SetRealIndex(from);

			SetRealIndex(to);

			Type* newArr = nullptr;

			if (from > to)
			{
				const int newArrLastIndex = from - to + 1;

				newArr = new Type[newArrLastIndex + 1];

				for (int i = to; i <= from; i++)
					newArr[newArrLastIndex - (i - to)] = pointer[i];
			}
			else
			{
				newArr = new Type[to - from + 1];

				for (int i = from; i <= to; i++)
					newArr[i - from] = pointer[i];
			}

			return newArr;
		}

		// Resizes array
		void SetLength(unsigned int newLength) {
			const int newLengthCopy = int(newLength);

			if (newLengthCopy == length)
				return;

			if (newLengthCopy == 0)
			{
				Clear();

				return;
			}
			
			if (isEmpty()) 
			{
				Create(newLengthCopy);

				return;
			} 

			Type* newArray = new Type[newLengthCopy];

			if (newLengthCopy < length)
				for (int i = 0; i < newLengthCopy; i++)
					newArray[i] = pointer[i];
			else
				for (int i = 0; i < length; i++)
					newArray[i] = pointer[i];

			length = newLengthCopy;

			delete[] pointer;

			pointer = newArray;
		}

		/*
		Moves objects after and this given index by given number of steps resizing array.
		Source value will be normalized by array`s bounds.
		Positive number of steps will move objects to the right expanding array with null initialized objects.
		Negative number of steps will move objects to the left shrinking array by deleting objects on path.
		*/
		void Shift(int source, int steps) {
			if constexpr (enabledDebug) {
				EmptyWarning();

				StepsWarning(source, steps);
			}

			SetRealIndex(source);

			Type* buffer = new Type[length];

			for (int i = 0; i < length; i++)
				buffer[i] = pointer[i];

			const int oldLength = length;

			SetLength(length + steps);

			if (steps > 0)
			{
				for (int i = 0; i < source; i++)
					pointer[i] = buffer[i];

				for (int i = 0; i < steps; i++)
					pointer[i + source] = Type();

				for (int i = source; i < oldLength; i++)
					pointer[i + steps] = buffer[i];
			}
			else
			{
				for (int i = 0; i < source + steps; i++)
					pointer[i] = buffer[i];

				for (int i = source; i < oldLength; i++)
					pointer[i + steps] = buffer[i];
			}

			delete[] buffer;
		}

		// Swaps objects at given indexes. Source and destination values will be normalized by array`s bounds.
		void Swap(int source, int destination, unsigned int count = 1) {
			if (!count)
				return;

			if constexpr (enabledDebug)
				EmptyWarning();

			for (unsigned int i = 0; i < count; i++) {
				const int src = i + source, dst = i + destination;

				const Type buff = operator[](src);

				operator[](src) = operator[](dst);

				operator[](dst) = buff;
			}
		}

		/*
		Moves objects from source index to destination index by given count. 
		Source and destination values will be normalized by array`s bounds.
		If destination is nullindex value, objects will be moved to the begining of array.
		*/
		void Move(int source, int destination, unsigned int count = 1) {
			if (!count)
				return;

			if constexpr (enabledDebug)
				EmptyWarning();

			Type* buffer = new Type[count];

			SetRealIndex(source);

			for (unsigned int i = 0; i < count; i++)
				buffer[i] = operator[](i + source);

			if (destination == nullIndex) 
			{
				for (int i = source + count - 1; i >= 0; i--)
					operator[](i) = operator[](i - count);

				for (unsigned int i = 0; i < count; i++)
					operator[](i) = buffer[i];
			}
			else
			{
				SetRealIndex(destination);

				if (source < destination)
					for (int i = source; i <= destination; i++)
						operator[](i) = operator[](i + count);
				else
					for (int i = source + count - 1; i >= destination; i--)
						operator[](i) = operator[](i - count);

				for (unsigned int i = 0; i < count; i++)
					operator[](i + destination) = buffer[i];
			}

			delete[] buffer;
		}

		// Adds an object at the end of array (using nullindex value) or at given index. Index value will be normalized by array`s bounds.
		void Add(Type Object, int index = nullIndex) {
			if (index == nullIndex)
			{
				SetLength(length + 1);

				pointer[length - 1] = Object;

				return;
			}

			SetRealIndex(index);

			Shift(index, 1);

			pointer[index] = Object;
		}

		// Adds a objects at the end of array (using nullindex value) or at given index. Index value will be normalized by array`s bounds.
		void AddMany(const Array<Type>& Objects, int index = nullIndex) {
			if (index == nullIndex)
			{
				const int oldLength = length;

				SetLength(length + Objects.length);

				for (int i = 0; i < Objects.length; i++)
					pointer[i + oldLength] = Objects.pointer[i];

				return;
			}

			SetRealIndex(index);

			Shift(index, Objects.length);

			for (int i = 0; i < Objects.length; i++)
				pointer[i + index] = Objects.pointer[i];
		}

		// Deletes a objects (an object) at given index. Index value will be normalized by array`s bounds.
		void Remove(int index, unsigned int count = 1) {
			if (count == 0)
				return;

			SetRealIndex(index);

			const int& intCount = int(count);

			if (index + intCount == length)
			{
				SetLength(length - intCount);

				return;
			}

			Shift(index + intCount, -intCount);
		}

		// Returns an index of the first object in array that is equal to given object. If no such object exists, returns nullIndex.
		int getObjectIndex(Type Object) const {
			for (int i = 0; i < length; i++)
				if (Object == pointer[i])
					return i;

			return nullIndex;
		}

		// Removes an object by its value
		void Erase(Type Object) {
			Remove(getObjectIndex(Object));
		}

		// Returns true if array contains same objects by value and has same length as other array
		bool operator ==(const Array& Array) const {
			if (length != Array.length)
				return false;

			for (int i = 0; i < length; i++)
				if (pointer[i] != Array.pointer[i])
					return false;

			return true;
		}

		// Returns true if array doesn`t contain same objects by value and doesn`t have same length as other array
		bool operator !=(const Array& Array) const {
			if (length != Array.length)
				return true;

			for (int i = 0; i < length; i++)
				if (pointer[i] == Array.pointer[i])
					return false;

			return true;
		}

		/*
		Calls a function for each object in array.
		First argument of function should be an object of array.
		Second argument should be an index of object in array.
		*/
		template <typename Func, typename... Args>
		void Foreach(Func func, Args... args) const {
			for (int i = 0; i < length; i++)
				func(pointer[i], i, args...);
		}

		/*
		Calls a function for each object in array within range.
		Range values will be normalized by array`s bounds.
		If "from" index is greater than "to" index, function will work in reverse order.
		First argument of function should be an object of array.
		Second argument should be an index of object in array.
		*/
		template <typename Func, typename... Args>
		void Foreach(int from, int to, Func func, Args... args) const {
			SetRealIndex(from);

			SetRealIndex(to);

			if (from > to)
			{
				for (int i = to; i <= from; i++)
				{
					const int index = from - i + 1;

					func(pointer[index], index, args...);
				}
			}
			else
			{
				for (int i = from; i <= to; i++)
					func(pointer[i], i, args...);
			}
		}

	private:
		// Pointer to the actual array
		Type* pointer = nullptr;

		// Length of array
		int length = 0;

		// Normalizes index value to array bounds
		void SetRealIndex(int& index) const {
			while (index < 0)
				index += length;

			while (index >= length)
				index -= length;
		}

		// Creates an internal array with given length
		void Create(int newLength) {
			Clear();

			if (newLength < 0)
				newLength = -newLength;

			pointer = new Type[newLength];

			length = newLength;
		}

		// Empty array assertion
		void EmptyWarning() const {
			if (isEmpty())
				assert("Array is empty!");
		}

		// Incorrect steps count assertion
		void StepsWarning(int source, int steps) const {
			if (-steps > source)
				assert("Steps count got out of array range!");
		}
	};

	// ostream operator for Array class
	template <typename Type>
	std::ostream& operator <<(std::ostream& os, const Array<Type>& Array) {
		if (Array.isEmpty())
		{
			os << "{ null }";

			return os;
		}

		const int lastIndex = Array.getLength() - 1;

		os << "{ ";

		for (int i = 0; i < lastIndex; i++)
			os << Array.toPtrArray()[i] << ", ";

		os << Array.toPtrArray()[lastIndex] << " }";

		return os;
	}

	// wostream operator for Array class
	template <typename Type>
	std::wostream& operator <<(std::wostream& os, const Array<Type>& Array) {
		if (Array.isEmpty())
		{
			os << L"{ null }";

			return os;
		}

		const int lastIndex = Array.getLength() - 1;

		os << L"{ ";

		for (int i = 0; i < lastIndex; i++)
			os << Array.toPtrArray()[i] << L", ";

		os << Array.toPtrArray()[lastIndex] << L" }";

		return os;
	}
}