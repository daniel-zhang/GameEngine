#include "VertexBuffer.h"
#include "RenderInterface.h"

VertexBuffer::VertexBuffer()
{
	mBuffer = NULL;
}

VertexBuffer::~VertexBuffer()
{
	safe_release(&mBuffer);
}

void StaticVertexBuffer::create( RenderInterface* ri, uint32 vertexSize, uint32 vertexNum, void* pData )
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(vertexSize) * vertexNum;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = pData;

	d3d_check(ri->mDevice->CreateBuffer(&vbd, &vinitData, &mBuffer));
}

void DynamicVertexBuffer::create( RenderInterface* ri, uint32 vertexSize, uint32 vertexNum )
{
	D3D11_BUFFER_DESC vbd;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = vertexSize * vertexNum;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	vbd.Usage = D3D11_USAGE_DYNAMIC; 
	 
	d3d_check(ri->mDevice->CreateBuffer(&vbd, 0, &mBuffer));
}

template<class VERTEX_TYPE>
void DynamicVertexBuffer::update( RenderInterface* ri, std::vector<VERTEX_TYPE>& vertices )
{
	// Map the buffer
	D3D11_MAPPED_SUBRESOURCE mappedData;
	ri->mCtx->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	VERTEX_TYPE* vStart = reinterpret_cast<VERTEX_TYPE*>(mappedData.pData);
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		vStart[i] = vertices[i];
	}
	ri->mCtx->Unmap(mBuffer, 0);
}
