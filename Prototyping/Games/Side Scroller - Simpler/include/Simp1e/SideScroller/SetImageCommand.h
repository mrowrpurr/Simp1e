#pragma onec

#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/QT/Conversions/ToQString.h>
#include <Simp1e/QT/ECS/QTImageComponent.h>

#include <string>


namespace Simp1e {

    class SetImageCommand {
        QTImageComponent* _imageComponent;
        std::string       _imageFilePath;

    public:
        SIMP1E_ECS_COMMAND("SetImage")

        SetImageCommand(QTImageComponent* imageComponent, const std::string& imageFilePath)
            : _imageComponent(imageComponent), _imageFilePath(imageFilePath) {}

        void Execute() { _imageComponent->SetPixmap(ToQString(_imageFilePath)); }
    };
}
