#pragma once


#define MAINWINDOW_ACTION_SLATS \
    void onActionSimpleBuildAndTranslate(); \
    void onActionSimpleTopExp(); \
    void onActionRotationSimpleShape(); \
    void onActionMirrorSimpleShape(); \
    void onActionCreateSimpleBSplineCurve(); \
    void onActionCreateRectangle(); \
    void onActionLeftCoreView(); \
    void onActionRightCoreView(); \
    void onActionFrontCoreView(); \
    void onActionBackCoreView();



#define MAINWINDOW_ACTION_SLOTS_FUN(WIN,HOMEPAGE,OP) \
    void WIN::onActionSimpleBuildAndTranslate() \
    { \
        HOMEPAGE->OP->TestSimpleBuildAndTranslate(); \
    } \
    \
    void WIN::onActionSimpleTopExp() \
    { \
        HOMEPAGE->OP->TestSimpleTopExp(); \
    } \
    \
     void WIN::onActionEditShape() \
    { \
        HOMEPAGE->OP->TestEditShape(); \
    } \
     void WIN::onActionBuildBaseMode() \
    { \
        HOMEPAGE->OP->TestBuildBaseMode(); \
    } \
    \
    \
    \
    void WIN::onActionCreateSimpleBSplineCurve() \
    { \
        HOMEPAGE->OP->TestCreateSimpleBSplineCurve(); \
    } \
    \
    void WIN::onActionCreateRectangle() \
    { \
        HOMEPAGE->OP->TestCreateRectangle(); \
    } \
    \
    \
    \
    void WIN::onActionLeftCoreView() \
    { \
        HOMEPAGE->OP->TestLeftCoreView(); \
    } \
    \
    void WIN::onActionRightCoreView() \
    { \
        HOMEPAGE->OP->TestRightCoreView(); \
    } \
    \
    void WIN::onActionFrontCoreView() \
    { \
        HOMEPAGE->OP->TestFrontCoreView(); \
    } \
    \
    void WIN::onActionBackCoreView() \
    { \
        HOMEPAGE->OP->TestBackCoreView(); \
    } \
    \
    \
    \
    void WIN::onActionImportIfc() \
    { \
        HOMEPAGE->OP->ImportIfc(); \
    } 