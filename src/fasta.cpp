//****************************************************************************
//* Copyright (c) 2012 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include "fasta.h"

namespace SyntenyFinder
{	
	static size_t cread(FILE * stream, char * buf, size_t size)
	{
		return fread(buf, sizeof(char), size, stream);
	}

	KSEQ_INIT(FILE*, cread)

	size_t FASTAReader::GetSequences(std::vector<FASTARecord> & record)
	{
		size_t seqId = 0;
		kseq_t * sequence = kseq_init(fileHandler_);
		for(; kseq_read(sequence) >= 0; seqId++)
		{
			record.push_back(FASTARecord(sequence->seq.s, sequence->name.s, record.size()));			
		}

		kseq_destroy(sequence);
		return seqId;
	}
}