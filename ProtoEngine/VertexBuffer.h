#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "reference.h"
#include <vector>

class VertexBuffer
{
public:
	friend class RenderInterface;
	VertexBuffer();
	virtual ~VertexBuffer();

protected:
	ID3D11Buffer* mBuffer;
};

class StaticVertexBuffer : public VertexBuffer
{
public:
	void create(RenderInterface* ri, uint32 vertexSize, uint32 vertexNum, void* pData);
};

class DynamicVertexBuffer : public VertexBuffer
{
public:
	void create(RenderInterface* ri, uint32 vertexSize, uint32 vertexNum);

	template<class VERTEX_TYPE>
	void update(RenderInterface* ri, std::vector<VERTEX_TYPE>& vertices);
};

#endif