#include <Specs/BDD.h>
#include <Specs/ExceptionHandlers/SnowhouseExceptionHandler.h>
#include <Specs/Setup.h>

#include "_SpecHelper.h"

//
#include "__SpecHeaderIncludes.h"
//

int main(int argc, char** argv) {
    Specs::CLI cli;
    Specs::Setup(cli);
    Specs::BDD(cli);

    cli.GetApplication()->AddExceptionHandler<Specs::ExceptionHandlers::SnowhouseExceptionHandler>(
    );

    for (auto& specDefinitionBody : HacksPendingSpecsCppLibraryCompletion::SpecDefinitionBodies) {
        specDefinitionBody();
    }

    cli.Run(argc, argv);
}