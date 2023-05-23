#include "DI.h"
#include "SpecHelper.h"

Describe("SECOND Does this work in the DI Spec?") {
    it("SECOND should maybe work here in DI Spec?", []() {
        AnythingStructFindMe anything;
        AssertThat("maybe", Equals("maybe???"));
    });
}