//****************************************************************************
//* Copyright (c) 2012 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include "blockfinder.h"

namespace SyntenyFinder
{
	BlockFinder::Edge::Edge(size_t chr, DNASequence::Direction direction, size_t startVertex, size_t endVertex, size_t actualPosition, size_t actualLength, size_t originalPosition, size_t originalLength, char firstChar):
		chr(chr), direction(direction), startVertex(startVertex), endVertex(endVertex), actualPosition(actualPosition), actualLength(actualLength), originalPosition(originalPosition), originalLength(originalLength), firstChar(firstChar) {}

	bool BlockFinder::CompareEdgesByDirection(const Edge & a, const Edge & b)
	{
		return a.GetDirection() < b.GetDirection();
	}

	bool BlockFinder::Edge::Coincide(const Edge & edge) const
	{
		return startVertex == edge.startVertex && endVertex == edge.endVertex && firstChar == edge.firstChar;
	}

	std::vector<size_t> BlockFinder::EdgeToVector(const Edge & a)
	{
		size_t feature[] = {a.GetStartVertex(), a.GetEndVertex(), a.GetFirstChar()};
		return std::vector<size_t>(feature, feature + sizeof(feature) / sizeof(feature[0]));
	}

	bool BlockFinder::EdgeEmpty(const Edge & a, size_t k)
	{
		return a.GetOriginalLength() < k;
	}

	bool BlockFinder::CompareEdgesNaturally(const Edge & a, const Edge & b)
	{
		return EdgeToVector(a) < EdgeToVector(b);
	}

	bool BlockFinder::Edge::Overlap(const Edge & edge) const
	{
		size_t a1 = this->GetOriginalPosition();
		size_t b1 = this->GetOriginalPosition() + this->GetOriginalLength();
		size_t a2 = edge.GetOriginalPosition();
		size_t b2 = edge.GetOriginalPosition() + edge.GetOriginalLength();
		size_t overlap = 0;
		if(a1 >= a2 && a1 <= b2)
		{
			overlap = std::min(b1, b2) - a1;
		}

		if(a2 >= a1 && a2 <= b1)
		{
			overlap = std::min(b1, b2) - a2;
		}

		return edge.chr == chr && overlap > 0;
	}

	size_t BlockFinder::Edge::GetChr() const
	{
		return chr;
	}
	
	DNASequence::Direction BlockFinder::Edge::GetDirection() const
	{
		return direction;
	}
	
	size_t BlockFinder::Edge::GetStartVertex() const
	{
		return startVertex;
	}
	
	size_t BlockFinder::Edge::GetEndVertex() const
	{
		return endVertex;
	}
	
	size_t BlockFinder::Edge::GetActualPosition() const
	{
		return actualPosition;
	}
	
	size_t BlockFinder::Edge::GetActualLength() const
	{
		return actualLength;
	}
	
	size_t BlockFinder::Edge::GetOriginalPosition() const
	{
		return originalPosition;
	}
	
	size_t BlockFinder::Edge::GetOriginalLength() const
	{
		return originalLength;
	}
	
	char BlockFinder::Edge::GetFirstChar() const
	{
		return firstChar;
	}
}