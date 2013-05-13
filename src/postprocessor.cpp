//****************************************************************************
//* Copyright (c) 2012 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include "postprocessor.h"

namespace SyntenyFinder
{
	namespace
	{
		struct Stripe
		{
			int firstBlock;
			int secondBlock;
			Stripe() {}
			Stripe(int firstBlock, int secondBlock): firstBlock(firstBlock), secondBlock(secondBlock) {}
			bool operator < (const Stripe & toCompare) const
			{
				return firstBlock < toCompare.firstBlock;
			}
		};

		std::string ReadInReverseDirection(const std::string & str)
		{
			std::string::const_reverse_iterator it1 = str.rbegin();
			std::string::const_reverse_iterator it2 = str.rend();
			return std::string(CFancyIterator(it1, DNASequence::Translate, ' '), CFancyIterator(it2, DNASequence::Translate, ' '));
		}
	}

	void Postprocessor::GlueStripes(std::vector<BlockInstance> & block)
	{
		std::vector<std::vector<BlockInstance> > perm(chr_->size());
		for(size_t i = 0; i < block.size(); i++)
		{
			perm[block[i].GetChrId()].push_back(block[i]);
		}

		for(size_t i = 0; i < perm.size(); i++)
		{
			std::sort(perm[i].begin(), perm[i].end(), compareByStart);
		}

		int sentinel = INT_MAX >> 1;
		bool glue = false;
		do
		{
			std::vector<Stripe> stripe;
			for(size_t chr = 0; chr < perm.size(); chr++)
			{
				for(size_t i = 0; i < perm[chr].size(); i++)
				{
					int bid = perm[chr][i].GetSignedBlockId();
					if(bid > 0)
					{
						int nextBid = i < perm[chr].size() - 1 ? perm[chr][i + 1].GetSignedBlockId() : sentinel;
						stripe.push_back(Stripe(bid, nextBid));
					}
					else
					{
						int prevBid = i > 0 ? perm[chr][i - 1].GetSignedBlockId() : -sentinel;
						stripe.push_back(Stripe(-bid, -prevBid));
					}
				}
			}

			size_t now = 0;
			size_t next = 0;
			std::sort(stripe.begin(), stripe.end());
			for(; now < stripe.size(); now = next)
			{
				glue = true;
				for(; next < stripe.size() && stripe[next].firstBlock == stripe[now].firstBlock; next++)
				{
					if(stripe[next].secondBlock != stripe[now].secondBlock || stripe[next].secondBlock == sentinel || Abs(stripe[next].secondBlock) == stripe[next].firstBlock)
					{
						glue = false;
					}
				}

				if(glue)
				{
					typedef std::vector<Stripe>::iterator It;
					std::pair<It, It> range = std::equal_range(stripe.begin(), stripe.end(), Stripe(Abs(stripe[now].secondBlock), 0));
					if(range.second - range.first != next - now)
					{
						glue = false;
					}
					else
					{
						break;
					}
				}
			}

			if(glue)
			{
				assert(next - now > 1);
				int glueBid = stripe[now].firstBlock;
				for(size_t chr = 0; chr < perm.size(); chr++)
				{
					for(size_t i = 0; i < perm[chr].size(); i++)
					{
						int bid = perm[chr][i].GetBlockId();
						if(bid == glueBid)
						{
							bid = perm[chr][i].GetSignedBlockId();
							if(bid > 0)
							{
								BlockInstance & a = perm[chr][i];
								BlockInstance & b = perm[chr][i + 1];
								a = BlockInstance(a.GetSignedBlockId(), &a.GetChrInstance(), a.GetStart(), b.GetEnd());
								perm[chr].erase(perm[chr].begin() + i + 1);
							}
							else
							{
								BlockInstance & a = perm[chr][--i];
								BlockInstance & b = perm[chr][i + 1];
								a = BlockInstance(b.GetSignedBlockId(), &a.GetChrInstance(), a.GetStart(), b.GetEnd());
								perm[chr].erase(perm[chr].begin() + i + 1);
							}
						}
					}
				}
			}
		}
		while(glue);

		block.clear();
		std::vector<int> oldId;
		for(size_t chr = 0; chr < perm.size(); chr++)
		{
			for(size_t i = 0; i < perm[chr].size(); i++)
			{
				block.push_back(perm[chr][i]);
				oldId.push_back(perm[chr][i].GetBlockId());
			}
		}

		std::sort(oldId.begin(), oldId.end());
		oldId.erase(std::unique(oldId.begin(), oldId.end()), oldId.end());
		for(std::vector<BlockInstance>::iterator it = block.begin(); it != block.end(); ++it)
		{
			int sign = it->GetSignedBlockId() > 0 ? +1 : -1;
			size_t newId = std::lower_bound(oldId.begin(), oldId.end(), it->GetBlockId()) - oldId.begin() + 1;
			*it = BlockInstance(static_cast<int>(newId) * sign, &it->GetChrInstance(), it->GetStart(), it->GetEnd());
		}
	}

	typedef std::vector<BlockInstance>::const_iterator BLCIterator;

	Postprocessor::Postprocessor(const std::vector<FASTARecord> & chr, size_t minBlockSize):
		chr_(&chr), minBlockSize_(minBlockSize)
	{
	}

	const BlockInstance* Postprocessor::PreviousBlock(const BlockInstance & block, const std::vector<BlockInstance> & blockList)
	{
		const BlockInstance* ret = 0;
		size_t start = block.GetStart();
		for(size_t i = 0; i < blockList.size(); i++)
		{
			if(blockList[i] != block && blockList[i].GetChrId() == block.GetChrId() && blockList[i].GetEnd() <= start)
			{
				if(ret == 0 || start - blockList[i].GetEnd() < start - ret->GetEnd())
				{
					ret = &blockList[i];
				}
			}
		}

		return ret;
	}

	const BlockInstance* Postprocessor::NextBlock(const BlockInstance & block, const std::vector<BlockInstance> & blockList)
	{
		const BlockInstance* ret = 0;
		size_t end = block.GetEnd();
		for(size_t i = 0; i < blockList.size(); i++)
		{
			if(blockList[i] != block && blockList[i].GetChrId() == block.GetChrId() && blockList[i].GetStart() >= end)
			{
				if(ret == 0 || blockList[i].GetStart() - end < ret->GetStart() - end)
				{
					ret = &blockList[i];
				}
			}
		}

		return ret;
	}
	
	std::pair<size_t, size_t> Postprocessor::DetermineLeftProbableBoundaries(std::vector<BlockInstance> & blockList, size_t blockid)
	{
		std::pair<size_t, size_t> ret;
		BlockInstance & block = blockList[blockid];
		size_t nowStart = block.GetStart();		
		ret.second = block.GetStart() + minBlockSize_;
		const BlockInstance * previousBlock = PreviousBlock(block, blockList);
		if(previousBlock != 0)
		{	
			size_t previousEnd = previousBlock->GetEnd();
			ret.first = std::max(previousEnd, nowStart - minBlockSize_) + 1;			
		}
		else
		{
			ret.first = nowStart >= minBlockSize_ ? nowStart - minBlockSize_ + 1: 0;
		}
				
		return ret;
	}

	std::pair<size_t, size_t> Postprocessor::DetermineRightProbableBoundaries(std::vector<BlockInstance> & blockList, size_t blockid)
	{
		std::pair<size_t, size_t> ret;
		BlockInstance & block = blockList[blockid];
		size_t nowEnd = block.GetEnd();
		ret.first = block.GetEnd() - minBlockSize_ + 1;
		const BlockInstance * nextBlock = NextBlock(block, blockList);
		if(nextBlock != 0)
		{			
			size_t nextStart = nextBlock->GetStart();
			ret.second = std::min(nextStart, nowEnd + minBlockSize_);
		}
		else
		{
			size_t chrSize = block.GetChrInstance().GetSequence().size();
			ret.second = nowEnd + minBlockSize_ < chrSize ? nowEnd + minBlockSize_ : chrSize;
		}
				
		return ret;
	}

	void Postprocessor::GetBoundariesSequence(const BlockInstance & block, std::pair<size_t, size_t> leftBoundaries, std::pair<size_t, size_t> rightBoundaries, std::string & start, std::string & end)
	{		
		const std::string::const_iterator & chr = block.GetChrInstance().GetSequence().begin();
		if(block.GetDirection() == DNASequence::positive)
		{			
			start.assign(chr + leftBoundaries.first, chr + leftBoundaries.second);
			end.assign(chr + rightBoundaries.first, chr + rightBoundaries.second);
		}
		else
		{
			start.assign(chr + rightBoundaries.first, chr + rightBoundaries.second);
			end.assign(chr + leftBoundaries.first, chr + leftBoundaries.second);
			start = ReadInReverseDirection(start);
			end = ReadInReverseDirection(end);
		}
	}

	void Postprocessor::LocalAlignment(const std::string & sequence1, const std::string & sequence2, std::pair<size_t, size_t> & coord1, std::pair<size_t, size_t> & coord2)
	{
		using namespace seqan;
		typedef String<char>				TSequence;	// sequence type
		typedef Align<TSequence, ArrayGaps>	TAlign;		// align type
		typedef Row<TAlign>::Type			TRow;
		typedef Iterator<TRow>::Type		TIterator;
		typedef Position<TAlign>::Type		TPosition;
		
		TSequence seq1 = sequence1;
		TSequence seq2 = sequence2;
		TAlign align;
		resize(rows(align), 2); 
		assignSource(row(align, 0), seq1);
		assignSource(row(align, 1), seq2);
		localAlignment(align, Score<int>(1, -1, -1, -1));		
		coord1.first = clippedBeginPosition(row(align, 0));
		coord1.second = clippedEndPosition(row(align, 0));
		coord2.first = clippedBeginPosition(row(align, 1));
		coord2.second = clippedEndPosition(row(align, 1));
	}

	void Postprocessor::UpdateBlockBoundaries(BlockInstance & block, std::pair<size_t, size_t> leftBoundaries, std::pair<size_t, size_t> rightBoundaries, std::pair<size_t, size_t> startAlignmentCoords, std::pair<size_t, size_t> endAlignmentCoords)
	{
		if(block.GetDirection() == DNASequence::positive)
		{
			size_t newStart = leftBoundaries.first + startAlignmentCoords.first;
			size_t newEnd = rightBoundaries.first + endAlignmentCoords.second;
			block = BlockInstance(block.GetSignedBlockId(), &block.GetChrInstance(), newStart, newEnd);
		}
		else
		{			
			size_t newStart = leftBoundaries.second - endAlignmentCoords.second;
			size_t newEnd = rightBoundaries.second - startAlignmentCoords.first;
			block = BlockInstance(block.GetSignedBlockId(), &block.GetChrInstance(), newStart, newEnd);
		}
	}

	void Postprocessor::CorrectBlocksBoundaries(std::vector<BlockInstance> & blockList, size_t referenceBlock, size_t assemblyBlock)
	{		
		std::pair<size_t, size_t> referenceStartCoord;
		std::pair<size_t, size_t> referenceEndCoord;
		std::pair<size_t, size_t> assemblyStartCoord;
		std::pair<size_t, size_t> assemblyEndCoord;
		std::string referenceStart;
		std::string referenceEnd;
		std::string assemblyStart;
		std::string assemblyEnd;
		std::pair<size_t, size_t> referenceLeftBoundaries = DetermineLeftProbableBoundaries(blockList, referenceBlock);
		std::pair<size_t, size_t> referenceRightBoundaries = DetermineRightProbableBoundaries(blockList, referenceBlock);
		std::pair<size_t, size_t> assemblyLeftBoundaries = DetermineLeftProbableBoundaries(blockList, assemblyBlock);
		std::pair<size_t, size_t> assemblyRightBoundaries = DetermineRightProbableBoundaries(blockList, assemblyBlock);
		GetBoundariesSequence(blockList[referenceBlock], referenceLeftBoundaries, referenceRightBoundaries, referenceStart, referenceEnd);
		GetBoundariesSequence(blockList[assemblyBlock], assemblyLeftBoundaries, assemblyRightBoundaries, assemblyStart, assemblyEnd);		
		LocalAlignment(referenceStart, assemblyStart, referenceStartCoord, assemblyStartCoord);
		LocalAlignment(referenceEnd, assemblyEnd, referenceEndCoord, assemblyEndCoord);
		UpdateBlockBoundaries(blockList[referenceBlock], referenceLeftBoundaries, referenceRightBoundaries, referenceStartCoord, referenceEndCoord);
		UpdateBlockBoundaries(blockList[assemblyBlock], assemblyLeftBoundaries, assemblyRightBoundaries, assemblyStartCoord, assemblyEndCoord);
	}

	void Postprocessor::ImproveBlockBoundaries(std::vector<BlockInstance> & blockList, const std::set<size_t> & referenceSequenceId)
	{
		referenceSequenceId_ = referenceSequenceId;
		std::vector<IndexPair> group;
		GroupBy(blockList, compareById, std::back_inserter(group));
		for(std::vector<IndexPair>::iterator it = group.begin(); it != group.end(); ++it)
		{
			size_t inReference = 0;
			size_t inAssembly = 0;
			for(size_t i = it->first; i < it->second; i++) 
			{
				inReference += referenceSequenceId_.count(blockList[i].GetChrId()) > 0 ? 1 : 0;
				inAssembly += referenceSequenceId_.count(blockList[i].GetChrId()) == 0 ? 1 : 0;
			}
				
			if(inReference == 1 && inAssembly == 1)
			{
				if(referenceSequenceId_.count(blockList[it->first].GetChrId()) == 0)
				{
					std::swap(blockList[it->first], blockList[it->first + 1]);
				}

				if(blockList[it->first].GetDirection() != DNASequence::positive)
				{					
					blockList[it->first].Reverse();
					blockList[it->first + 1].Reverse();
				}
						
				CorrectBlocksBoundaries(blockList, it->first, it->first + 1);						
			}
		}
	}
}