#pragma once

#include "Main.hpp"

namespace Stass {
	constexpr int nullIndex = -2147483648i32;

	template <typename Type>
	class Array {
	public:
		static Array<Type> Sized(int length) {
			Array<Type> NewArr = {};

			NewArr.SetLength(length);

			return NewArr;
		}

		static Array<Type> Filled(Type Object, int count) {
			Array<Type> NewArr = {};

			NewArr.SetLength(count);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = Object;

			return NewArr;
		}

		static Array<Type> Copied(const Type* ptrArray, const unsigned int& ptrArrayLength) {
			Array<Type> NewArr = {};

			NewArr.SetLength((int)ptrArrayLength);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = ptrArray[i];

			return NewArr;
		}

		static Array<Type> CopiedInRange(const Type* ptrArray, const unsigned int& from, const unsigned int& to) {
			Array<Type> NewArr = {};

			NewArr.SetLength(int(to - from) + 1);

			for (int i = 0; i < NewArr.length; i++)
				NewArr.pointer[i] = ptrArray[i + from];

			return NewArr;
		}

		Array() {};

		Array(const std::initializer_list<Type>& list) {
			Create((int)list.size());

			for (int i = 0; i < length; i++)
				pointer[i] = *(list.begin() + i);
		}

		Array(const Array<Type>& Array) {
			operator=(Array);
		}

		~Array() {
			Clear();
		}

		Array<Type>& operator =(const Array<Type>& Array) {
			Create(Array.length);

			for (int i = 0; i < length; i++)
				pointer[i] = Array[i];

			return *this;
		}

		Array<Type>& operator +=(const Array<Type>& Array) {
			AddMany(Array);

			return *this;
		}

		Array<Type>& operator +(const Array<Type>& OtherArray) const {
			Array<Type> NewArray = *this;

			NewArray += OtherArray;

			return NewArray;
		}

		operator bool() const {
			return (bool)length;
		}

		bool isEmpty() const {
			return !operator bool();
		}

		int getLength() const {
			return length;
		}

		int GetLastIndex() const {
			return length - 1;
		}

		operator const Type* () const {
			return toPtrArray();
		}

		const Type* toPtrArray() const {
			return pointer;
		}

		Type& operator [](int index) const {
			CheckIfEmpty();

			SetRealIndex(index);

			return pointer[index];
		}

		const Type* begin() const {
			return pointer;
		}

		const Type* end() const {
			return pointer + length;
		}

		void Clear() {
			if (!length)
				return;

			length = 0;

			delete[] pointer;

			pointer = nullptr;
		}

		Type* CreatePtrArray() const {
			return CreatePtrArray(0, length - 1);
		}

		Type* CreatePtrArray(int from, int to) const {
			CheckIfEmpty();

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

		void SetLength(const unsigned int& newLength) {
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

			delete pointer;

			pointer = newArray;
		}

		void Shift(int source, const int& steps) {
			// Checks if array is empty
			CheckIfEmpty();

			// Normalizes index for array bounds
			SetRealIndex(source);

			// Safety assert
			if (-steps > source)
				assert("Steps count got out of array range!");

			// Creates a buffer array
			Type* buffer = new Type[length];

			// Copy array`s contents to buffer
			for (int i = 0; i < length; i++)
				buffer[i] = pointer[i];

			const int oldLength = length;

			// Sets new array length
			SetLength(length + steps);

			if (steps > 0)
			{
				// Copies untouched part as is
				for (int i = 0; i < source; i++)
					pointer[i] = buffer[i];

				// Sets free space to zero
				for (int i = 0; i < steps; i++)
					pointer[i + source] = Type();

				// Copies moved part to new array with shifting
				for (int i = source; i < oldLength; i++)
					pointer[i + steps] = buffer[i];
			}
			else
			{
				// Copies untouched part as is
				for (int i = 0; i < source + steps; i++)
					pointer[i] = buffer[i];

				// Copies moved part to new array with shifting
				for (int i = source; i < oldLength; i++)
					pointer[i + steps] = buffer[i];
			}

			// Deletes buffer
			delete[] buffer;
		}

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

		void Remove(int index, const unsigned int& count = 1) {
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

		int getObjectIndex(Type Object) const {
			CheckIfEmpty();

			for (int i = 0; i < length; i++)
				if (Object == pointer[i])
					return i;

			return nullIndex;
		}

		void Erase(Type Object) {
			Remove(getObjectIndex(Object));
		}

		bool operator ==(const Array& Array) const {
			if (length != Array.length)
				return false;

			for (int i = 0; i < length; i++)
				if (pointer[i] != Array.pointer[i])
					return false;

			return true;
		}

		bool operator !=(const Array& Array) const {
			if (length != Array.length)
				return true;

			for (int i = 0; i < length; i++)
				if (pointer[i] == Array.pointer[i])
					return false;

			return true;
		}

		template <typename Func, typename... Args>
		void Foreach(Func func, Args... args) const {
			for (int i = 0; i < length; i++)
				func(pointer[i], i, args...);
		}

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
		Type* pointer = nullptr;

		int length = 0;

		void SetRealIndex(int& index) const {
			while (index < 0)
				index += length;

			while (index >= length)
				index -= length;
		}

		void Create(int newLength) {
			Clear();

			if (newLength < 0)
				newLength = -newLength;

			pointer = new Type[newLength];

			length = newLength;
		}

		void CheckIfEmpty() const {
			if (isEmpty())
				assert("Array is empty!");
		}
	};

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
}