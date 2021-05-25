#include "mem/cache/replacement_policies/bit_plru_rp.hh"

#include <cassert>
#include <iostream>

#include "params/BitPLRURP.hh"
#include "sim/core.hh"

namespace ReplacementPolicy {

BitPLRU::BitPLRU(const Params &p) : Base(p), cacheID(p.cache_ID), numFlagResets(0), numFlagResetsPtr(new int) {
	*numFlagResetsPtr = 0;
}

void
BitPLRU::touch(const std::shared_ptr<ReplacementData>&
                                                  replacement_data) const
{
    std::static_pointer_cast<BitPLRUReplData>(replacement_data)->
                                                  flag = 1;
}

// Note: getVictim will set all candidate flags to 0 if no candidate with a 0 flag is found
ReplaceableEntry*
BitPLRU::getVictim(const ReplacementCandidates& candidates) const {
    assert(candidates.size() > 0);

    ReplaceableEntry* victim = candidates[0];

    for (const auto& candidate : candidates) {
        // candidate's replacement data
        std::shared_ptr<BitPLRUReplData> candidate_replacement_data =
            std::static_pointer_cast<BitPLRUReplData>(candidate->replacementData);

        if (candidate_replacement_data->flag == 0) {
            victim = candidate;
            return victim;
        }
        
    }

    // If we arrive at this point, no candidate was found, so every flag is 1.
    // Time to reset the flags!

    for (const auto& candidate : candidates) {
        std::shared_ptr<BitPLRUReplData> candidate_replacement_data =
            std::static_pointer_cast<BitPLRUReplData>(candidate->replacementData);

        candidate_replacement_data->flag = 0;
    }

    (*numFlagResetsPtr)++;

    return victim;
}

void
BitPLRU::reset(const std::shared_ptr<ReplacementData>&
                                                    replacement_data) const
{
    // Set flag to 1
    std::static_pointer_cast<BitPLRUReplData>(
        replacement_data)->flag = 1;
}

void BitPLRU::invalidate(const std::shared_ptr<ReplacementData>&replacement_data) const {
    std::static_pointer_cast<BitPLRUReplData>(
        replacement_data)->flag = 0;
}

std::shared_ptr<ReplacementData>
BitPLRU::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new BitPLRUReplData);
}

void
BitPLRU::regStats() {
	//MemObject::regStats();
	
	numFlagResets.name(name() + ".numFlagResets")
			.desc("Number of resets");
}

BitPLRU::~BitPLRU() {
    numFlagResets = *numFlagResetsPtr;
    delete numFlagResetsPtr;
    //std::cout << "Number of resets for cache with identifier " << cacheID << ": " << *numFlagResets << std::endl;
}

} // namespace ReplacementPolicy
