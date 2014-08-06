#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "reference.h"

class IndexBuffer
{
public:
	friend class RenderInterface;

	IndexBuffer();
	~IndexBuffer();

protected:
	ID3D11Buffer* mBuffer;
};

class StaticIndexBuffer : public IndexBuffer
{
public:
	void create(RenderInterface* ri, uint32 indexNum, uint32* pData);
};

#endif