#pragma once

#include "Array.hpp"

namespace Stass {
	template <typename Char>
	class String {
	public:
		static String<Char> Null() {
			return String<Char>(Char());
		}

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

		template <typename Char2>
		String(const String<Char2>& String) {
			CharArray.SetLength(String.getLength());

			for (int i = 0; i < String.getLength(); i++)
				CharArray.operator[](i) = Char(String[i]);
		}

		String(const Char* string) {
			unsigned int lastIndex = 0;

			while (string[lastIndex] != 0)
				lastIndex++;

			CharArray.operator=(Array<Char>::Copied(string, lastIndex + 1));
		}

		String(const Char& character = Char()) {
			if (character == 0)
			{
				CharArray.SetLength(1);
			}
			else
			{
				CharArray.SetLength(2);

				CharArray.operator[](1) = Char();
			}

			CharArray.operator[](0) = character;
		}

		virtual ~String() {};

		const Array<Char>& toCharArray() const {
			return CharArray;
		}

		operator const Array<Char>& () const {
			return CharArray;
		}

		Array<Char> CreateCharArray() const {
			return CharArray;
		}

		const Char* toCharPtrArray() const {
			return CharArray.toPtrArray();
		}

		Char* CreateCharPtrArray() const {
			return CharArray.CreatePtrArray();
		}

		operator const Char* () const {
			return CharArray.toPtrArray();
		}

		int getLength() const {
			return CharArray.getLength();
		}

		int GetLastIndex() const {
			return CharArray.GetLastIndex();
		}

		bool isNull() const {
			return getLength() <= 1;
		}

		operator bool() const {
			return isNull();
		}

		virtual void AddCharacter(const Char& character, const int& index) {
			CharArray.Add(character, index);
		}

		virtual void AddString(const String<Char>& OtherString, const int& index) {
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

		virtual String<Char>& operator =(const String<Char>& String) {
			CharArray.operator=(String);

			return *this;
		}

		virtual String<Char>& operator +=(const String<Char>& String) {
			AddString(String, GetLastIndex());

			return *this;
		}

		String<Char> operator +(const Char* string) const {
			String<Char> NewString = *this;
		
			NewString += string;
		
			return NewString;
		}

		String<Char> operator +(const Char& character) const {
			String<Char> NewString = *this;
		
			NewString.AddCharacter(character, GetLastIndex());
		
			return NewString;
		}

		const Char& operator [](const int& index) const {
			return CharArray[index];
		}

		const Char* begin() const {
			return CharArray.begin();
		}

		const Char* end() const {
			return CharArray.end();
		}

	private:
		Array<Char> CharArray;
	};

	template <typename Char>
	std::basic_ostream<Char, std::char_traits<Char>>& operator <<(std::basic_ostream<Char, std::char_traits<Char>>& os, const String<Char>& String) {
		os << String.toCharPtrArray();

		return os;
	}
}
