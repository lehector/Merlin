
#include "MulBottomUpDeduction.h"

#include <boost/integer/mod_inverse.hpp>

#include "boost/log/trivial.hpp"
#include "../enumeration/SizeBasedEnumeratorMulUnderApprox.h"


int64_t extended_euclidian(int64_t a, int64_t m, int64_t x, int64_t y) {
    if(((uint64_t) a) <= 1) return x;
    int64_t q = ((uint64_t) a) / ((uint64_t) m);
    int64_t mp = a - (q * m);
    int64_t yp = x - (q * y);
    return extended_euclidian(m, mp, y, yp);
}

uint64_t unsigned_div_2p64(int64_t d) {
    if(d == INT64_MIN) return 2;
    if(d < 0) return 1;
    int64_t q = (((uint64_t) INT64_MIN) / ((uint64_t) d)) << 1;
    int64_t r = 0 - q * d;
    if(((uint64_t) r) >= ((uint64_t) d)) {
        return q + 1;
    }
    return q;

}

int64_t mul_inv_64(int64_t a) {
    int64_t first_q = unsigned_div_2p64(a);
    int64_t first_m = -(first_q * a);
    int64_t first_y = -first_q;
    return extended_euclidian(a, first_m, 1, first_y);
}

int64_t mul_inv(int64_t a, int64_t m)
{
    int64_t x = boost::integer::mod_inverse(a, m);
    return x;
}

std::vector<uint64_t> getFactors(const uint64_t val, const uint64_t target, int numTrailingZerosVal, std::atomic<bool>* stop) {
    int64_t mulInverse;
    if(numTrailingZerosVal > 0) {
        mulInverse = mul_inv(val >> numTrailingZerosVal, 0x8000000000000000 >> numTrailingZerosVal);
    }
    else {
        mulInverse = mul_inv_64(val);
    }
    uint64_t next;
    if(numTrailingZerosVal > 0) {
        next = ((target >> numTrailingZerosVal) * mulInverse) % (0x8000000000000000 >> (numTrailingZerosVal - 1));
    }
    else {
        next = (target >> numTrailingZerosVal) * mulInverse;
    }
    std::vector<uint64_t> out;
    if(numTrailingZerosVal > 0) {
        for(int i = 0; i < (2 << (numTrailingZerosVal - 1)); i++) {
            if (stop != nullptr && stop->load()) break;
            out.push_back(next + i * (0x8000000000000000 >> (numTrailingZerosVal - 1)));
        }
    }
    else {
        out.push_back(next);
    }
    return out;
}


std::optional<ASTNode *> MulBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if (newProg->getNodeType().getType() == Helper::Type::BitVec64 && !SizeBasedEnumeratorMulUnderApprox::checkPreCondition(newProg->getValues(), expectedOutputs)) {
        const std::vector<variant> &progValues = newProg->getValues();
        int numContexts = progValues.size();
        int min = std::min(numContexts, config->getFirstNValuesCheck());
        std::vector<std::pair<uint64_t, uint64_t> > valTargetPairs;
        valTargetPairs.reserve(min);
        for (int i = 0; i < min; i++) {
            valTargetPairs.emplace_back(std::get<uint64_t>(progValues.at(progValues.size() - 1 - i)),
                                        expectedOutputsVals.at(expectedOutputsVals.size() - 1 - i));
        }
        int trailingZerosFirstContextVal = Helper::countTrailingZeros(valTargetPairs.at(0).first);
        firstContextValToProgsMap[valTargetPairs.at(0).first].push_back(newProg);
        return getProgramUsingMath(newProg, valTargetPairs, trailingZerosFirstContextVal);
    }
    return std::nullopt;
}

MulBottomUpDeduction::MulBottomUpDeduction(const std::vector<std::map<std::string, variant> > &contexts,
                                           const std::vector<uint64_t> &expected_outputs_vals,
                                           const std::vector<variant> &expected_outputs, const std::shared_ptr<VocabFactory> &vocabFactory,
                                           std::atomic<bool>* stop)
    : BottomUpDeductionBV(contexts, expected_outputs, expected_outputs_vals), stop(stop) {
    config = Config::GetInstance();
    for(const std::shared_ptr<VocabMaker> &maker: vocabFactory->getNonLeaves()) {
        if(maker->getHead() == "bvmul") {
            mulMaker = maker;
            noMaker = false;
        }
    }
}

void MulBottomUpDeduction::clear() {
    firstContextValToProgsMap.clear();
}


std::optional<ASTNode*> MulBottomUpDeduction::getProgramUsingMath(
    ASTNode*newProg, const std::vector<std::pair<uint64_t, uint64_t>>& valTargetPairs, int trailingZerosFirstContextVal) {
    if(trailingZerosFirstContextVal > 63) return std::nullopt;
    std::vector<uint64_t> viableFactors = getViableFactorsMath(valTargetPairs.at(0).first, valTargetPairs.at(0).second, trailingZerosFirstContextVal, stop);
    std::vector<ASTNode* > progs = getViablePrograms(viableFactors, stop);
    for(ASTNode*prog: progs) {
        std::optional<ASTNode* > out = checkProg(newProg, valTargetPairs, prog);
        if(out.has_value()) return out;
    }
    return std::nullopt;
}


std::vector<uint64_t> MulBottomUpDeduction::getViableFactorsMath(uint64_t firstContextVal,
    uint64_t firstContextTarget, int trailing_zeros_first_context_val, std::atomic<bool>* stop) {
    if(0x8000000000000000 >> trailing_zeros_first_context_val == 1) {
        return {0};
    }
    return getFactors(firstContextVal, firstContextTarget, trailing_zeros_first_context_val, stop);
}

std::vector<ASTNode*> MulBottomUpDeduction::getViablePrograms(const std::vector<uint64_t>& viableFactors, std::atomic<bool>* stop) {
    std::vector<ASTNode*> ret;
    for(uint64_t factor : viableFactors) {
        if (stop != nullptr && stop->load()) break;
        auto it = firstContextValToProgsMap.find(factor);
        if(it != firstContextValToProgsMap.end()) {
            ret.insert(ret.end(), it->second.begin(), it->second.end());
        }
    }
    return ret;
}

std::optional<ASTNode*> MulBottomUpDeduction::checkProg(ASTNode*newProg, const std::vector<std::pair<uint64_t, uint64_t>>& valTargetPairs, ASTNode*prog) const {
    const std::vector<variant> &progValues = prog->getValues();
    for(int i = 0; i < valTargetPairs.size(); i++) {
        uint64_t contextCandidate = std::get<uint64_t>(progValues.at(progValues.size() - 1 - i));
        if ((valTargetPairs.at(i).first * contextCandidate) != valTargetPairs.at(i).second) {
            return std::nullopt;
        }
    }
    //BOOST_LOG_TRIVIAL(info) << "found prog that works on last " << valTargetPairs.size() << " values with mul";
    ASTNode* mulProg = mulMaker->apply({newProg, prog}, contexts);
    mulProg->setMaker(mulMaker);
    if (mulProg->getValues() == expectedOutputs) {
        return mulProg;
    }
    delete mulProg;
    return std::nullopt;
}