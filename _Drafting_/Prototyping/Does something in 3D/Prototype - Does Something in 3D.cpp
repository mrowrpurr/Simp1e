#include <QApplication>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>

int main(int argc, char** argv) {
    QApplication           app(argc, argv);
    Qt3DExtras::Qt3DWindow view;

    // Create a root entity
    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity;

    // Create a material
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setDiffuse(QColor(QRgb(0xff00ff)));

    // Create a cube mesh
    Qt3DExtras::QCuboidMesh* cubeMesh = new Qt3DExtras::QCuboidMesh(rootEntity);

    // Create a transform
    Qt3DCore::QTransform* transform = new Qt3DCore::QTransform(rootEntity);
    transform->setScale(2.0f);

    // Create an entity for the cube
    Qt3DCore::QEntity* cubeEntity = new Qt3DCore::QEntity(rootEntity);
    cubeEntity->addComponent(cubeMesh);
    cubeEntity->addComponent(material);
    cubeEntity->addComponent(transform);

    // Set up lighting
    Qt3DCore::QEntity*       lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight* light       = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform* lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(60, 0, 100));
    lightEntity->addComponent(lightTransform);

    // Set up camera
    Qt3DRender::QCamera* camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
    Qt3DExtras::QOrbitCameraController* camController =
        new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed(50.f);
    camController->setLookSpeed(180.f);
    camController->setCamera(camera);

    // Set root object of the scene
    view.setRootEntity(rootEntity);

    // Show window
    view.show();

    return app.exec();
}
