#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "reference.h"
#include <vector>

class IndexBuffer
{
public:
	friend class RenderInterface;

	IndexBuffer();
	~IndexBuffer();

    void create(RenderInterface*, uint32 indexNum, uint32* pData);

    uint32 getIndexNum(){return mIndexNum;}
    uint32 getFaceNum(){return mFaceNum;}
    std::vector<uint32>* getData(){return &mData;}

protected:
    // CPU copy of data
    uint32 mIndexNum;
    uint32 mFaceNum;
    std::vector<uint32> mData;
	ID3D11Buffer* mBuffer;
};

#endif