#include <alloc.h>
#include <video.h>




uint m_numOfBlocks = 20; // Num of blocks
uint m_sizeOfEachBlock = (uint) 0x1000; // Size of each block
uint m_numFreeBlocks = 20; // Num of remaining blocks
uint m_numInitialized = 0; // Num of initialized blocks
uchar* m_memStart = (uchar *)0x600000; // Beginning of memory pool
uchar* m_next = (uchar *)0x600000; // Num of next free block 


uchar* addr_from_index(uint i)
{
	return m_memStart + ( i * m_sizeOfEachBlock );
}

uint index_from_addr(const uchar* p)
{
	return (((uint)(p - m_memStart)) / m_sizeOfEachBlock);
}

void* allocate()
{
	if (m_numInitialized < m_numOfBlocks )
 	{
 		//__video_debug(m_memStart+'0');
		uint* p = (uint*)addr_from_index( m_numInitialized );
 		*p = m_numInitialized + 1;
 		m_numInitialized++;
 	}
 	
 	void* ret = NULL;
 	if ( m_numFreeBlocks > 0 )
 	{
 		//__video_debug('b');
 		ret = (void*)m_next;
 		--m_numFreeBlocks;
 		if (m_numFreeBlocks!=0)
		{
	 		m_next = addr_from_index( *((uint*)m_next) );
 		}
 		else
 		{
 			m_next = NULL;
 		}
 	}
 	//char * aa = ((char *)0xB8000);
 	//*aa=ret;
 	//__video_debug(ret+'0');
 	return ret;
 }
 

 void free(void* p)
 {
 	if (m_next != NULL)
 	{
 		(*(uint*)p) = index_from_addr( m_next );
 		m_next = (uchar*)p;
	}
 	else
 	{
 		*((uint*)p) = m_numOfBlocks;
 		m_next = (uchar*)p;
 	}
 	++m_numFreeBlocks;
 } 