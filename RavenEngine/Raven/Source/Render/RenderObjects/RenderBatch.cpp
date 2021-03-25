#include "RenderBatch.h"




namespace Raven {




RenderBatch::RenderBatch()
{

}


RenderBatch::~RenderBatch()
{

}


void RenderBatch::Add(RenderPrimitive* primitive)
{
	elements.push_back(primitive);
}


void RenderBatch::Clear()
{
	elements.clear();
}


} // End of namespace Raven.
