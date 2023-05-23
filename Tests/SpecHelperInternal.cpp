#include "SpecHelperInternal.h"

namespace HacksPendingSpecsCppLibraryCompletion {
    std::vector<std::function<void()>> SpecDefinitionBodies;

    CollectSpecDefinitionBody::CollectSpecDefinitionBody(std::function<void()> body) {
        SpecDefinitionBodies.push_back(body);
    }
}
