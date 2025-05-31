#pragma once

#include "Array.hpp"

namespace Stass {
	// The string implementation
	template <typename Char>
	class String {
	public:
		// Returns an empty string
		static String<Char> Null() {
			return String<Char>(Char());
		}

		// Returns a string with numbers converted from an unsigned 64-bit integer
		static String<Char> FromUnsInt(unsigned long long int number) {
			String<Char> NewString = Null();

			do
			{
				const Char digit = Char(number % 10 + 48); // 48 is '0'

				NewString.AddCharacter(digit, 0);

				number /= 10;
			} 
			while (number > 0);

			return NewString;
		}

		// Returns a string with numbers converted from a signed 64-bit integer
		static String<Char> FromInt(long long int number) {
			String<Char> NewString = Null();

			if (number < 0)
			{
				NewString += 45; // 45 is '-'

				number = -number;
			}

			NewString += FromUnsInt((unsigned long long int)number);

			return NewString;
		}

		// Basic constructor
		String() {
			CharArray.SetLength(1);

			CharArray.operator[](0) = Char();
		}

		// Construction from other String with different character type
		template <typename Char2>
		String(const String<Char2>& String) {
			CharArray.SetLength(String.getLength());

			for (int i = 0; i < String.getLength(); i++)
				CharArray.operator[](i) = Char(String[i]);
		}

		// Construction from c-style string with different character type
		template <typename Char2>
		String(const Char2* string) {
			unsigned int lastIndex = 0;

			while (string[lastIndex] != 0)
				lastIndex++;

			Char* buffer = new Char[lastIndex + 1];

			for (unsigned int i = 0; i <= lastIndex; i++)
				buffer[i] = (Char)string[i];

			CharArray.operator=(Array<Char>::Copied(buffer, lastIndex + 1));

			delete[] buffer;
		}

		// Construction from symbol
		template <typename Char2>
		String(Char2 character) {
			if (character == 0)
			{
				CharArray.SetLength(1);
			}
			else
			{
				CharArray.SetLength(2);

				CharArray.operator[](1) = Char();
			}

			CharArray.operator[](0) = (Char)character;
		}

		// Virtual destructor
		virtual ~String() {};

		// Returns a reference to the internal character array
		const Array<Char>& toCharArray() const {
			return CharArray;
		}

		// Returns a reference to the internal character array
		operator const Array<Char>& () const {
			return CharArray;
		}

		// Creates a copy of the internal character array
		Array<Char> CreateCharArray() const {
			return CharArray;
		}

		// Returns a reference to the internal character array
		const Char* toCharPtrArray() const {
			return CharArray.toPtrArray();
		}

		// Creates a copy of the internal character array
		Char* CreateCharPtrArray() const {
			return CharArray.CreatePtrArray();
		}

		// Returns a pointer to the internal character array
		operator const Char* () const {
			return CharArray.toPtrArray();
		}

		// Returns the length of the string including the null terminator
		int getLength() const {
			return CharArray.getLength();
		}

		// Returns the null terminator index
		int GetLastIndex() const {
			return CharArray.GetLastIndex();
		}

		// Returns true if the string is empty (contains only null terminator)
		bool isNull() const {
			return getLength() <= 1;
		}

		// Returns true if the string is Not empty (contains at least one character)
		operator bool() const {
			return isNull();
		}

		// Adds a character before the null terminator
		virtual void AddCharacter(const Char& character, const int index) {
			CharArray.Add(character, index);
		}

		// Adds all characters of given string before the null terminator
		virtual void AddString(const String<Char>& OtherString, const int index) {
			if (this == &OtherString)
			{
				const String<Char> Str = *this;

				CharArray.AddMany(Str.CharArray, index);

				CharArray.Remove(index + Str.GetLastIndex());

				return;
			}

			CharArray.AddMany(OtherString.CharArray, index);

			CharArray.Remove(index + OtherString.GetLastIndex());
		}

		// Removes the character after the null terminator from the string
		virtual void RemoveLastCharacter() {
			if (isNull())
				return;

			CharArray.Remove(CharArray.getLength() - 2);
		}

		// Basic assign operator
		virtual String<Char>& operator =(const String<Char>& String) {
			CharArray.operator=(String);

			return *this;
		}

		// Adds all characters of given string before the null terminator
		virtual String<Char>& operator +=(const String<Char>& String) {
			AddString(String, GetLastIndex());

			return *this;
		}

		// Returns a copy of string with added characters from another string before the null terminator
		String<Char> operator +(const Char* string) const {
			String<Char> NewString = *this;

			NewString += string;

			return NewString;
		}

		// Returns a copy of string with added character before the null terminator
		String<Char> operator +(const Char& character) const {
			String<Char> NewString = *this;

			NewString.AddCharacter(character, GetLastIndex());

			return NewString;
		}

		// Returns a reference to the character at given index. Index value will be normalized to string bounds.
		const Char& operator [](const int& index) const {
			return CharArray[index];
		}

		// Returns a pointer to the first character of the string
		const Char* begin() const {
			return CharArray.begin();
		}

		// Returns a pointer to the abstract character placed after the null terminator
		const Char* end() const {
			return CharArray.end();
		}

		// Rwturns true if strings have same characters and same length
		bool operator ==(const String<Char>& Str) const {
			return CharArray == Str.CharArray;
		}

		// Rwturns true if strings have not same characters and not same length
		bool operator !=(const String<Char>& Str) const {
			return CharArray != Str.CharArray;
		}

	private:
		// Internal character array
		Array<Char> CharArray;
	};

	// Template ostream operator for String class
	template <typename Char>
	std::basic_ostream<Char, std::char_traits<Char>>& operator <<(std::basic_ostream<Char, std::char_traits<Char>>& os, const String<Char>& String) {
		os << String.toCharPtrArray();

		return os;
	}
}