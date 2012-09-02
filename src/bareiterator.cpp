#include "dnasequence.h"

namespace SyntenyBuilder
{
	char DNASequence::ForwardIterator::Spell() const
	{
		return it_->actual;
	}

	void DNASequence::ForwardIterator::MoveForward()
	{
		++it_;
	}

	void DNASequence::ForwardIterator::MoveBackward()
	{
		--it_;
	}

	DNASequence::Direction DNASequence::ForwardIterator::GetDirection() const
	{
		return positive;
	}

	const DNASequence::DNACharacter* DNASequence::ForwardIterator::GetNaked() const
	{
		return &(*it_);
	}

	bool DNASequence::ForwardIterator::Equal(const DNASequence::GenericIterator & toCompare) const
	{
		if(toCompare.GetDirection() == positive)
		{
			return it_ == static_cast<const DNASequence::ForwardIterator&>(toCompare).it_;
		}

		return false;
	}

	DNASequence::GenericIterator* DNASequence::ForwardIterator::Clone() const
	{
		return new ForwardIterator(it_);
	}
	
	DNASequence::GenericIterator* DNASequence::ForwardIterator::Invert() const
	{
		return new BackwardIterator(SequenceNegIterator(it_));
	}

	DNASequence::SequencePosIterator DNASequence::ForwardIterator::Base() const
	{
		return it_;
	}

	char DNASequence::ForwardIterator::TranslateChar(char ch) const
	{
		return ch;
	}

	DNASequence::ForwardIterator::ForwardIterator()
	{
	}

	DNASequence::ForwardIterator::ForwardIterator(SequencePosIterator it): it_(it)
	{
	}

	char DNASequence::BackwardIterator::Spell() const
	{
		return Translate(it_->actual);
	}

	void DNASequence::BackwardIterator::MoveForward()
	{
		++it_;
	}

	void DNASequence::BackwardIterator::MoveBackward()
	{
		--it_;
	}

	DNASequence::Direction DNASequence::BackwardIterator::GetDirection() const
	{
		return negative;
	}

	bool DNASequence::BackwardIterator::Equal(const DNASequence::GenericIterator & toCompare) const
	{
		if(toCompare.GetDirection() == negative)
		{
			return it_ == static_cast<const DNASequence::BackwardIterator&>(toCompare).it_;
		}

		return false;
	}

	const DNASequence::DNACharacter* DNASequence::BackwardIterator::GetNaked() const
	{
		return &(*it_);
	}

	char DNASequence::BackwardIterator::TranslateChar(char ch) const
	{
		return Translate(ch);
	}

	DNASequence::GenericIterator::~GenericIterator()
	{
	}

	DNASequence::BackwardIterator::BackwardIterator()
	{
	}

	DNASequence::BackwardIterator::BackwardIterator(SequenceNegIterator it): it_(it)
	{
	}

	DNASequence::GenericIterator* DNASequence::BackwardIterator::Clone() const
	{
		return new BackwardIterator(it_);
	}

	DNASequence::GenericIterator* DNASequence::BackwardIterator::Invert() const
	{	
		return new ForwardIterator(it_.base());
	}

	DNASequence::SequencePosIterator DNASequence::BackwardIterator::Base() const
	{
		return SequenceNegIterator(it_).base();
	}
}