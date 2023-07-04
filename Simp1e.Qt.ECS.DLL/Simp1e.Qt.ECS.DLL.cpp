#include <_Log_.h>
_LogToFile_("Simp1e.Qt.ECS.log");

#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEngineManagerService.h>
#include <Simp1e/QtGuiSystem.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>

#include <QApplication>
#include <QTimer>
#include <chrono>
#include <memory>

using namespace Simp1e;

int          argc = 0;
QApplication app{argc, nullptr};

constexpr auto* ENVIRONMENT_NAME = "Default";

IEngine*                                    _environment;
std::unique_ptr<SystemPointerManagerClient> systemManager;
std::unique_ptr<EntityPointerManagerClient> entityManager;

QTimer                                                      mainLoopTimer;
int                                                         mainLoopPerMillisecond = 1000;  // 16;
std::chrono::time_point<std::chrono::high_resolution_clock> mainLoopLastTime =
    std::chrono::high_resolution_clock::now();

void GameLoop() {
    _Log_("GameLoop");

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime   = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mainLoopLastTime).count();
    mainLoopLastTime = currentTime;
    auto seconds     = deltaTime / 1000000.0;

    _environment->GetSystemManager()->Update(_environment, seconds);

    _Log_("GameLoop end");
}

void SetupSystems(IEngine* environment) { systemManager->Add<QtGuiSystem>(environment); }

void Initialize(IEngine* environment) {
    _environment  = environment;
    systemManager = std::make_unique<SystemPointerManagerClient>(environment->GetSystemManager());
    entityManager = std::make_unique<EntityPointerManagerClient>(environment->GetEntityManager());
    SetupSystems(environment);
}

OnSimp1eLoad {
    _Log_("Init");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEngineManagerService>())
        if (auto* environmentManager = environmentManagerService->GetEngineManager())
            if (auto* environment = environmentManager->GetEngine(ENVIRONMENT_NAME)) Initialize(environment);
}

OnSimp1eStart {
    _Log_("Qt ECS GUI start");

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(mainLoopPerMillisecond);

    _Log_("Run Qt application");
    app.setStyle("Fusion");
    app.exec();

    _Log_("Qt ECS GUI end");
}
