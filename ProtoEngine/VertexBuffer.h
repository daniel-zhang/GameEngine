#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>

#include "reference.h"
#include "VertexFormat.h"

class RenderInterface;
class VertexBuffer
{
public:
	friend class RenderInterface;
	VertexBuffer();
	virtual ~VertexBuffer();

    void create(RenderInterface* ri );

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