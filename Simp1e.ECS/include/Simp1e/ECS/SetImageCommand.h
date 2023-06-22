#pragma once

#include <string>

#include "CommandTypeMacro.h"
#include "Entity.h"
#include "Game.h"
#include "ImageComponent.h"

namespace Simp1e::ECS {

    class SetImageCommand {
        Entity      _entity;
        std::string _imageFilePath;

    public:
        SIMP1E_ECS_COMMAND("SetImage")

        SetImageCommand() = default;
        SetImageCommand(const Entity& entity, const std::string& imageFilePath)
            : _entity(entity), _imageFilePath(imageFilePath) {}

        void Execute(Game& game) {
            if (auto* imageComponent = game.Entities().GetComponent<ImageComponent>(_entity))
                imageComponent->SetImagePath(_imageFilePath);
        }
    };
}
