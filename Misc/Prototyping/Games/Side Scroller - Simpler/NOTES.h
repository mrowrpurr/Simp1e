// TODO rename all of my Qt* files and classes to QT* so it doesn't collide with Qt's classes

// bool viewportEvent(QEvent* event) override {
//     switch (event->type()) {
//         case QEvent::TouchBegin: {
//             QTouchEvent*                   touchEvent  = static_cast<QTouchEvent*>(event);
//             QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
//             _touchTimer.restart();
//             _recentTouchPos = _currentTouchPos = touchPoints.first().pressPosition();
//         }
// Size GetGameSize(int labelHeight = 0) {
//     qDebug() << "GetGameSize() label height: " << labelHeight;
//     // #ifdef Q_OS_ANDROID
//     auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
//     return {
//         static_cast<sreal>(screenGeometry.width()),
//         static_cast<sreal>(screenGeometry.height() - labelHeight)};
//     // #else
//     //     return {1600, 800};
//     // #endif
// }
QObject::connect(screen, &QScreen::geometryChanged, &window, [&]() {

});
auto* screen = QGuiApplication::primaryScreen();