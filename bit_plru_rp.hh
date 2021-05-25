#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_BIT_PLRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_BIT_PLRU_RP_HH__

#include <memory>
#include <string>

#include "base/types.hh"
#include "mem/cache/replacement_policies/base.hh"
#include "base/statistics.hh"
//#include "mem/mem_object.hh"

struct BitPLRURPParams;

namespace ReplacementPolicy {

class BitPLRU : public Base {
    protected:
        struct BitPLRUReplData : ReplacementData {
                /* PLRU flag */
                int flag;

                /**
                * Default constructor. Invalidate data.
                */
               BitPLRUReplData() : ReplacementData(), flag(0) {}
        };
        
        std::string cacheID;
        Stats::Scalar numFlagResets;
        int* numFlagResetsPtr;
    
    public:
        typedef BitPLRURPParams Params;
        BitPLRU(const Params &p);
        ~BitPLRU();

        void invalidate(const std::shared_ptr<ReplacementData>&replacement_data)
                                                            const override;

        void touch(const std::shared_ptr<ReplacementData>& replacement_data)
                                                            const override;

        void reset(const std::shared_ptr<ReplacementData>& replacement_data)
                                                            const override;

	ReplaceableEntry* getVictim(const ReplacementCandidates& candidates)
							    const override;
						
        //ReplaceableEntry* getVictimWithReset(ReplacementCandidates& candidates);

        std::shared_ptr<ReplacementData> instantiateEntry() override;
        
        void regStats() override;
};

} // namespace ReplacementPolicy


/*ReplacementPolicy::BitPLRU*
BitPLRURPParams::create () const {
	return new ReplacementPolicy::BitPLRU(this);
}*/


#endif // __MEM_CACHE_REPLACEMENT_POLICIES_BIT_PLRU_RP_HH__
