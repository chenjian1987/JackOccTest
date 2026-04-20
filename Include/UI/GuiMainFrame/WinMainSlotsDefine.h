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
    void WIN::onActionTestSimpleSewing() \
    { \
        HOMEPAGE->OP->TestSimpleSewing(); \
    } \
    void WIN::onActionTestSimpleShapeUpgrade() \
    { \
        HOMEPAGE->OP->TestSimpleShapeUpgrade(); \
    } \
    \
    \
    \
    \
    \
    \
    \
    void WIN::onActionMatrixTranslate() \
    { \
        HOMEPAGE->OP->TestMatrixTranslate(); \
    } \
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
    void WIN::onActionTopCoreView() \
    { \
        HOMEPAGE->OP->TestTopCoreView(); \
    } \
    void WIN::onActionBottomCoreView() \
    { \
        HOMEPAGE->OP->TestBottomCoreView(); \
    } \
    \
    \
    void WIN::onActionImportIfc() \
    { \
        HOMEPAGE->OP->ImportIfc(); \
    } \
    void WIN::onActionPointLineRelation() \
    { \
        HOMEPAGE->OP->PointLineRelation(); \
    } \
    void WIN::onActionPointOnFace() \
    { \
        HOMEPAGE->OP->PointOnFace(); \
    } \
    void WIN::onActionLine2Line() \
    { \
        HOMEPAGE->OP->NearestPointsLine2Line(); \
    } \
    void WIN::onActionLine2BSplineCurve() \
    { \
        HOMEPAGE->OP->NearestPointsLine2BSplineCurve(); \
    } \
    void WIN::onActionSegPlaneIntersection() \
    { \
        HOMEPAGE->OP->SegPlaneIntersection(); \
    } \
    void WIN::onActionArcBSplineIntersect() \
    { \
        HOMEPAGE->OP->ArcBSplineIntersect(); \
    } \
    void WIN::onActionArcFaceIntersectBSplineFace() \
    { \
         HOMEPAGE->OP->ArcFaceIntersectBSplineFace(); \
    } \
    \
    \
    void WIN::onActionHelloOCAF() \
    { \
         HOMEPAGE->OP->HelloOCAF(); \
    } \
    \
    \
    void WIN::onActionBsplineCurveTest1() \
    { \
        HOMEPAGE->OP->BsplineCurveTest1(); \
    } \
    \
    \
    void WIN::onActionBsplineCurveKnotMultCompareTest() \
    { \
        HOMEPAGE->OP->BsplineCurveKnotMultCompareTest(); \
    } \
    void WIN::onActionBsplineCurveLocalControlCompareTest() \
    { \
    HOMEPAGE->OP->BsplineCurveLocalControlCompareTest(); \
    }\
    void WIN::onActionBsplineCurvePointEvalCompareTest() \
    { \
    HOMEPAGE->OP->BsplineCurvePointEvalCompareTest(); \
    }\
    void WIN::onActionBsplineDataValidityCompareTest() \
    { \
    HOMEPAGE->OP->BsplineDataValidityCompareTest(); \
    }\
    void WIN::onActionBsplineCurveActivePoleRangeTest() \
    { \
        HOMEPAGE->OP->BsplineCurveActivePoleRangeTest(); \
    } 
