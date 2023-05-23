#include "DI.h"
#include "_SpecHelper.h"

Describe("Does this work in the DI Spec?") {
    it("should maybe work here in DI Spec?", []() {
        AnythingStructFindMe anything;
        AssertThat("maybe", Equals("maybe???"));
    });
}