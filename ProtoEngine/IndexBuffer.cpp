#include "IndexBuffer.h"
#include "RenderInterface.h"

IndexBuffer::IndexBuffer()
{
	mBuffer = NULL;
}

IndexBuffer::~IndexBuffer()
{
	safe_release(&mBuffer);
}


void StaticIndexBuffer::create( RenderInterface* ri, uint32 indexNum, uint32* pData )
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexNum;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = pData;

	d3d_check(ri->mDevice->CreateBuffer(&ibd, &iinitData, &mBuffer));
}
