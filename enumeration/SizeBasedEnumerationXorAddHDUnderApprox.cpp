
#include "SizeBasedEnumerationXorAddHDUnderApprox.h"
#include "SizeBasedEnumeratorHDUnderApprox.h"

bool SizeBasedEnumerationXorAddHDUnderApprox::getViolation(ASTNode *newProg) {
    if (newProg->getNodeType().getType() != Helper::Type::BitVec64) return false;
    return SizeBasedEnumeratorHDUnderApprox::getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}
