#include <Simp1e/DI.h>

#include "_SpecHelper.h"

Describe("Something") {
    it("can run a test", []() {
        AnythingStructFindMe anything;
        AssertThat("hello", Equals("not hello"));
    });
}
