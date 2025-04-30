#pragma once

#include "Array.hpp"

namespace Stass {
	template <class Type>
	class Instance;

	template <class Type>
	class Reference {
	public:
		template <class>
		friend class Instance;

		static const Reference<Type> Null() {
			return Reference<Type>();
		}

		Reference() {};

		Reference(const Reference<Type>& Reference) {
			operator=(Reference);
		}

		~Reference();

		Reference<Type>& operator =(const Reference<Type>& OtherReference);

		Type& operator *() const;

		operator bool() const {
			return pInstance;
		}

		bool operator ==(const Reference<Type>& OtherReference) const {
			if (!pInstance)
				return false;

			return pInstance == OtherReference.pInstance;
		}

		bool operator !=(const Reference<Type>& OtherReference) const {
			if (!pInstance || !OtherReference.pInstance)
				return false;

			return pInstance != OtherReference.pInstance;
		}

	private:
		Instance<Type>* pInstance = nullptr;

		Reference(Instance<Type>* Instance) : pInstance(Instance) {};
	};

	template <class Type>
	class Instance {
		Array<Reference<Type>*> pReferences = {};

		Type* pointer = nullptr;

		using ptrToObjPtr = void*;

	public:
		template <class>
		friend class Reference;

		static ptrToObjPtr operator new(size_t size) {
			Type* ptr = new Type(size);

			return (ptrToObjPtr)(&ptr);
		}

		static void operator delete(ptrToObjPtr pointer) {
			Type** ptr = (Type**)pointer;

			if (*ptr) 
			{
				delete* ptr;

				*ptr = nullptr;
			}
		}

		Instance(Type* objectPtr) {
			pointer = objectPtr;
		}

		Reference<Type> operator &() const {
			return Reference<Type>((Instance<Type>*)this);
		}

		Type& operator *() const {
			return *pointer;
		}

		operator bool() const {
			return pointer;
		}

		operator ptrToObjPtr() const {
			return (ptrToObjPtr)(&pointer);
		}
	};
}