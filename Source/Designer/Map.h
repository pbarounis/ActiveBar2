#ifndef __MAP_H__
#define __MAP_H__


//
//  Copyright � 1995-1999, Data Dynamics. All rights reserved.
//
//  Unpublished -- rights reserved under the copyright laws of the United
//  States.  USE OF A COPYRIGHT NOTICE IS PRECAUTIONARY ONLY AND DOES NOT
//  IMPLY PUBLICATION OR DISCLOSURE.
//
//

typedef void* POSITION;

#define FSTART_POSITION ((void*)-1L)

struct FPlex     // warning variable length structure
{
	FPlex* pNext;
#if (_AFX_PACKING >= 8)
	DWORD dwReserved[1];    // align on 8 byte boundary
#endif
	// BYTE data[maxNum*elementSize];

	void* data() { return this+1; }

	static FPlex* PASCAL Create(FPlex*& head, UINT nMax, UINT cbElement);
			// like 'calloc' but no zero fill
			// may throw memory exceptions

	void FreeDataChain();       // free this one and links
};

//
// FMap
//

class FMap
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
		void* key;
		void* value;
	};

public:

// Construction
	FMap(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(void* key, void*& rValue) const;

// Operations
	// Lookup and add if not there
	void*& operator[](void* key);

	// add a new (key, value) pair
	void SetAt(void* key, void* newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(void* key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION& rNextPosition, void*& rKey, void*& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
	// Overridables: special non-virtual (see map implementation for details)
	// Routine used to user-provided hash keys
	UINT HashKey(void* key) const;
// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	struct FPlex* m_pBlocks;
	int m_nBlockSize;

	CAssoc* NewAssoc();
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(void*, UINT&) const;
public:
	~FMap();
protected:
	// local typedefs for CTypedPtrMap class template
	typedef void* BASE_KEY;
	typedef void* BASE_ARG_KEY;
	typedef void* BASE_VALUE;
	typedef void* BASE_ARG_VALUE;
};

inline void FMap::SetAt(void* key, void* newValue)
	{ (*this)[key] = newValue; }


//
// TypedMap
//

template <class TKEY, class TDATA> 
class TypedMap : public FMap
{
public:
	// Lookup
	BOOL Lookup(TKEY key, TDATA& rValue) const;
	void SetAt(TKEY key, TDATA newValue);
	TDATA& operator[](TKEY key);
	BOOL RemoveKey(TKEY key);
	void GetNextAssoc(POSITION& rNextPosition, TKEY& rKey, TDATA& rValue) const;
};

template <class TKEY, class TDATA>
BOOL TypedMap<TKEY, TDATA>::Lookup(TKEY key, TDATA& rValue) const
{
	return FMap::Lookup((void*)key, (void*&)rValue);
}

template <class TKEY, class TDATA>
void TypedMap<TKEY, TDATA>::SetAt(TKEY key, TDATA newValue)
{
	FMap::SetAt((void*)key, newValue);
}

template <class TKEY, class TDATA>
TDATA& TypedMap<TKEY, TDATA>::operator[](TKEY key)
{
	return FMap::operator[]((void*)key);
}

template <class TKEY, class TDATA>
BOOL TypedMap<TKEY, TDATA>::RemoveKey(TKEY key)
{
	return FMap::RemoveKey((void*)key);
}

template <class TKEY, class TDATA>
void TypedMap<TKEY, TDATA>::GetNextAssoc(POSITION& rNextPosition, TKEY& rKey, TDATA& rValue) const
{
	FMap::GetNextAssoc(rNextPosition, (void*&)rKey, (void*&)rValue);
}

//
// FPtrList 
//

class FPtrList 
{
protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		void* data;
	};
public:

// Construction
	FPtrList(int nBlockSize = 10);

// Attributes (head and tail)
	// count of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// peek at head or tail
	void*& GetHead();
	void* GetHead() const;
	void*& GetTail();
	void* GetTail() const;

// Operations
	// get head or tail (and remove it) - don't call on empty list!
	void* RemoveHead();
	void* RemoveTail();

	// add before head or after tail
	POSITION AddHead(void* newElement);
	POSITION AddTail(void* newElement);

	// add another list of elements before head or after tail
	void AddHead(FPtrList* pNewList);
	void AddTail(FPtrList* pNewList);

	// remove all elements
	void RemoveAll();

	// iteration
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	void*& GetNext(POSITION& rPosition); // return *Position++
	void* GetNext(POSITION& rPosition) const; // return *Position++
	void*& GetPrev(POSITION& rPosition); // return *Position--
	void* GetPrev(POSITION& rPosition) const; // return *Position--

	// getting/modifying an element at a given position
	void*& GetAt(POSITION position);
	void* GetAt(POSITION position) const;
	void SetAt(POSITION pos, void* newElement);
	void RemoveAt(POSITION position);

	// inserting before or after a given position
	POSITION InsertBefore(POSITION position, void* newElement);
	POSITION InsertAfter(POSITION position, void* newElement);

	// helper functions (note: O(n) speed)
	POSITION Find(void* searchValue, POSITION startAfter = NULL) const;
						// defaults to starting at the HEAD
						// return NULL if not found
	POSITION FindIndex(int nIndex) const;
						// get the 'nIndex'th element (may return NULL)

// Implementation
protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	int m_nCount;
	CNode* m_pNodeFree;
	struct FPlex* m_pBlocks;
	int m_nBlockSize;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);

public:
	~FPtrList();
	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};

inline int FPtrList::GetCount() const
	{ return m_nCount; }
inline BOOL FPtrList::IsEmpty() const
	{ return m_nCount == 0; }
inline void*& FPtrList::GetHead()
	{ assert(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
inline void* FPtrList::GetHead() const
	{ assert(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
inline void*& FPtrList::GetTail()
	{ assert(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
inline void* FPtrList::GetTail() const
	{ assert(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
inline POSITION FPtrList::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
inline POSITION FPtrList::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
inline void*& FPtrList::GetNext(POSITION& rPosition) // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline void* FPtrList::GetNext(POSITION& rPosition) const // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline void*& FPtrList::GetPrev(POSITION& rPosition) // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline void* FPtrList::GetPrev(POSITION& rPosition) const // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline void*& FPtrList::GetAt(POSITION position)
	{ CNode* pNode = (CNode*) position;
		return pNode->data; }
inline void* FPtrList::GetAt(POSITION position) const
	{ CNode* pNode = (CNode*) position;
		return pNode->data; }
inline void FPtrList::SetAt(POSITION pos, void* newElement)
	{ CNode* pNode = (CNode*) pos;
		pNode->data = newElement; }


/////////////////////////////////////////////////////////////////////////////
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions

inline int FMap::GetCount() const
	{ return m_nCount; }
inline BOOL FMap::IsEmpty() const
	{ return m_nCount == 0; }
inline POSITION FMap::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : FSTART_POSITION; }
inline UINT FMap::GetHashTableSize() const
	{ return m_nHashTableSize; }



////////////// FARRAY

class FArray
{
public:

// Construction
	FArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	void* GetAt(int nIndex) const;
	void SetAt(int nIndex, void* newElement);
	void*& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const void** GetData() const;
	void** GetData();

	// Potentially growing the array
	void SetAtGrow(int nIndex, void* newElement);
	int Add(void* newElement);
	int Append(const FArray& src);
	void Copy(const FArray& src);

	// overloaded operator helpers
	void* operator[](int nIndex) const;
	void*& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, void* newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, FArray* pNewArray);

// Implementation
protected:
	void** m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~FArray();

protected:
	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};


inline int FArray::GetSize() const
	{ return m_nSize; }
inline int FArray::GetUpperBound() const
	{ return m_nSize-1; }
inline void FArray::RemoveAll()
	{ SetSize(0); }
inline void* FArray::GetAt(int nIndex) const
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
inline void FArray::SetAt(int nIndex, void* newElement)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
inline void*& FArray::ElementAt(int nIndex)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
inline const void** FArray::GetData() const
	{ return (const void**)m_pData; }
inline void** FArray::GetData()
	{ return (void**)m_pData; }
inline int FArray::Add(void* newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
inline void* FArray::operator[](int nIndex) const
	{ return GetAt(nIndex); }
inline void*& FArray::operator[](int nIndex)
	{ return ElementAt(nIndex); }

//
// TypedArray
//

template <class TYPE> class TypedArray : public FArray
{
public:
	int Add(TYPE pElement);
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, TYPE pElement);
};

//
// Add
//

template <class TYPE>
int TypedArray<TYPE>::Add(TYPE pElement)
{
	return FArray::Add((void*)pElement);
}

//
// GetAt
//

template <class TYPE>
TYPE TypedArray<TYPE>::GetAt(int nIndex) const
{
	return (TYPE)FArray::GetAt(nIndex);
}


//
// SetAt
//

template <class TYPE>
void TypedArray<TYPE>::SetAt(int nIndex, TYPE pElement)
{
	FArray::SetAt(nIndex, (void*)pElement);
}

#endif
