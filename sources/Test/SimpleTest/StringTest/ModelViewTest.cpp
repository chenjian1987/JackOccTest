#include "ModelViewTest.h"
#include "ModelView.h"
#include "Document.h"


using namespace JackC;






void ModelViewTest::TestCreateDefaultModelView()
{
    RuntimeId id(10);
    std::wstring newDocumentName = L"Jack Doc";


    Document* pDoc = new Document(id, newDocumentName);

    ModelView* pModelView = ModelView::Create(pDoc, L"三维视图", BuiltInViewType::ThreeDimensional, Vector3d(1000, 0, -1.0), Vector3d::Zero, Vector3d(1.0, 0, 100), Vector3d(0, 1.0, 0));
    ModelView* pModelView2 = ModelView::Create(pDoc, L"三维视图", BuiltInViewType::ThreeDimensional, Vector3d(1000, 0, -1.0), Vector3d::Zero, Vector3d(1.0, 0, 100), Vector3d(0, 1.0, 0));



}

