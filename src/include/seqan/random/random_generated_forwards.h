// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2010, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================


#ifndef SEQAN_HEADER_RANDOM_GENERATED_FORWARDS_H 
#define SEQAN_HEADER_RANDOM_GENERATED_FORWARDS_H 

//////////////////////////////////////////////////////////////////////////////
// NOTE: This file is automatically generated by build_forwards.py
//       Do not edit this file manually!
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////////////////////////////

namespace seqan {
namespace ext {

//____________________________________________________________________________
// MTRand

class MTRand;       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(73)

} //namespace ext
} //namespace seqan
//////////////////////////////////////////////////////////////////////////////

namespace seqan {

//____________________________________________________________________________
// GeometricFairCoin

struct GeometricFairCoin;       	// "seqan-1.3.1/seqan/random/random_geometric.h"(47)

//____________________________________________________________________________
// LogNormal

struct LogNormal;       	// "seqan-1.3.1/seqan/random/random_lognormal.h"(50)

//____________________________________________________________________________
// MeanStdDev

struct MeanStdDev;       	// "seqan-1.3.1/seqan/random/random_lognormal.h"(71)

//____________________________________________________________________________
// MersenneTwister

struct MersenneTwister;       	// "seqan-1.3.1/seqan/random/random_base.h"(47)
struct MersenneTwister;       	// "seqan-1.3.1/seqan/random/random_mt19937.h"(49)

//____________________________________________________________________________
// MuSigma

struct MuSigma;       	// "seqan-1.3.1/seqan/random/random_lognormal.h"(61)

//____________________________________________________________________________
// Normal

struct Normal;       	// "seqan-1.3.1/seqan/random/random_lognormal.h"(47)
struct Normal;       	// "seqan-1.3.1/seqan/random/random_normal.h"(47)

//____________________________________________________________________________
// Pdf

template <typename TSpec> class Pdf;       	// "seqan-1.3.1/seqan/random/random_base.h"(71)

//____________________________________________________________________________
// Rng

template <typename TSpec > class Rng;       	// "seqan-1.3.1/seqan/random/random_base.h"(60)

//____________________________________________________________________________
// RngFunctor

template <typename TRng, typename TPdf> struct RngFunctor;       	// "seqan-1.3.1/seqan/random/random_rng_functor.h"(48)

//____________________________________________________________________________
// Uniform

template <typename T> struct Uniform;       	// "seqan-1.3.1/seqan/random/random_uniform.h"(48)

} //namespace seqan


//////////////////////////////////////////////////////////////////////////////
// TYPEDEFS


//////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

namespace seqan {
namespace ext {

//____________________________________________________________________________
// MTRand::MTRand

inline MTRand::MTRand( const uint32 oneSeed );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(269)
inline MTRand::MTRand( uint32 *const bigSeed, const uint32 seedLength );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(272)
inline MTRand::MTRand();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(275)
inline MTRand::MTRand( const MTRand& o );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(278)

//____________________________________________________________________________
// MTRand::hash

inline MTRand::uint32 MTRand::hash( time_t t, clock_t c );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(146)

//____________________________________________________________________________
// MTRand::initialize

inline void MTRand::initialize( const uint32 seed );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(171)

//____________________________________________________________________________
// MTRand::load

inline void MTRand::load( uint32 *const loadArray );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(377)

//____________________________________________________________________________
// MTRand::operator()

inline double MTRand::operator()();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(363)

//____________________________________________________________________________
// MTRand::operator=

inline MTRand& MTRand::operator=( const MTRand& o );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(405)

//____________________________________________________________________________
// MTRand::rand

inline double MTRand::rand();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(323)
inline double MTRand::rand( const double n );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(326)

//____________________________________________________________________________
// MTRand::rand53

inline double MTRand::rand53();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(341)

//____________________________________________________________________________
// MTRand::randDblExc

inline double MTRand::randDblExc();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(335)
inline double MTRand::randDblExc( const double n );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(338)

//____________________________________________________________________________
// MTRand::randExc

inline double MTRand::randExc();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(329)
inline double MTRand::randExc( const double n );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(332)

//____________________________________________________________________________
// MTRand::randInt

inline MTRand::uint32 MTRand::randInt();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(288)
inline MTRand::uint32 MTRand::randInt( const uint32 n );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(304)

//____________________________________________________________________________
// MTRand::randNorm

inline double MTRand::randNorm( const double mean, const double stddev );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(347)

//____________________________________________________________________________
// MTRand::reload

inline void MTRand::reload();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(188)

//____________________________________________________________________________
// MTRand::save

inline void MTRand::save( uint32* saveArray ) const;       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(368)

//____________________________________________________________________________
// MTRand::seed

inline void MTRand::seed( const uint32 oneSeed );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(204)
inline void MTRand::seed( uint32 *const bigSeed, const uint32 seedLength );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(211)
inline void MTRand::seed();       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(246)

//____________________________________________________________________________
// operator<<

inline std::ostream& operator<<( std::ostream& os, const MTRand& mtrand );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(387)

//____________________________________________________________________________
// operator>>

inline std::istream& operator>>( std::istream& is, MTRand& mtrand );       	// "seqan-1.3.1/seqan/random/ext_MersenneTwister.h"(395)

} //namespace ext
} //namespace seqan
//////////////////////////////////////////////////////////////////////////////

namespace seqan {

//____________________________________________________________________________
// _pickRandomNumber

template <typename TRNG, typename T> inline typename Value<Pdf<Uniform<T> > >::Type _pickRandomNumber(TRNG & rng, Pdf<Uniform<T> > const & pdf, True const &);       	// "seqan-1.3.1/seqan/random/random_uniform.h"(111)
template <typename TRNG, typename T> inline typename Value<Pdf<Uniform<T> > >::Type _pickRandomNumber(TRNG & rng, Pdf<Uniform<T> > const & pdf, False const &);       	// "seqan-1.3.1/seqan/random/random_uniform.h"(127)

//____________________________________________________________________________
// pickRandomNumber

template <typename TRandomNumberGenerator> inline typename Value<Pdf<LogNormal> >::Type pickRandomNumber(TRandomNumberGenerator & rng, Pdf<LogNormal> const & pdf);       	// "seqan-1.3.1/seqan/random/random_lognormal.h"(150)
template <typename TRng, typename TPdf> inline unsigned pickRandomNumber(Rng<RngFunctor<TRng, TPdf> > & rng);       	// "seqan-1.3.1/seqan/random/random_rng_functor.h"(119)
template <typename TRNG, typename T> inline typename Value<Pdf<Uniform<T> > >::Type pickRandomNumber(TRNG & rng, Pdf<Uniform<T> > const & pdf);       	// "seqan-1.3.1/seqan/random/random_uniform.h"(138)
template <typename TRNG> inline typename Value<Pdf<Normal> >::Type pickRandomNumber(TRNG & rng, Pdf<Normal> const & pdf);       	// "seqan-1.3.1/seqan/random/random_normal.h"(109)
template <typename TRNG> inline typename Value<Pdf<GeometricFairCoin> >::Type pickRandomNumber(TRNG & rng, Pdf<GeometricFairCoin> const & );       	// "seqan-1.3.1/seqan/random/random_geometric.h"(102)
inline unsigned pickRandomNumber(Rng<MersenneTwister> & mt);       	// "seqan-1.3.1/seqan/random/random_mt19937.h"(112)

//____________________________________________________________________________
// shuffle

template <typename TContainer, typename TRNG> void shuffle(TContainer & container, TRNG & rng);       	// "seqan-1.3.1/seqan/random/random_shuffle.h"(70)

} //namespace seqan

#endif

