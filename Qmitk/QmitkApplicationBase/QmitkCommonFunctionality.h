// std
#include <string>

// itk includes
#include <itkMinimumMaximumImageCalculator.h>

// mitk includes
#include <mitkBoolProperty.h>
#include <mitkProperties.h>
#include <mitkStringProperty.h>
#include <mitkDataTreeNode.h>
#include <mitkSurfaceData.h>

#include <MeshUtil.h>

class CommonFunctionality
{

public:

  /** \brief compute min and max 
  */
  template < typename TImageType >
    static void MinMax(typename TImageType::Pointer image, float &min, float &max)
  {
    typedef itk::MinimumMaximumImageCalculator<TImageType> MinMaxCalcType;
    MinMaxCalcType::Pointer minmax = MinMaxCalcType::New();
    minmax->SetImage( image );
    minmax->ComputeMinimum();
    minmax->ComputeMaximum();

    min = (float) minmax->GetMinimum();
    max = (float) minmax->GetMaximum();
  }


  /** 
  * \brief converts the itk image to mitk image, creates a datatreenode and adds the node to 
  * the referenced datatree
  */
  template < typename TImageType >
    static void AddItkImageToDataTree(typename TImageType::Pointer itkImage, mitk::DataTreeIterator* iterator, std::string str)
  {
    mitk::DataTreeIterator* it=iterator->clone();

    mitk::Image::Pointer image = mitk::Image::New();
    image->InitializeByItk(itkImage.GetPointer());
    image->SetVolume(itkImage->GetBufferPointer());

    mitk::DataTreeNode::Pointer node = NULL;
    mitk::DataTreeIterator* subTree = ((mitk::DataTree *) it->getTree())->GetNext("fileName", new mitk::StringProperty( str.c_str() ));

    if (subTree == NULL || subTree->get() == NULL)
    {
      node=mitk::DataTreeNode::New();
      mitk::StringProperty::Pointer nameProp = new mitk::StringProperty(str.c_str());
      node->SetProperty("fileName",nameProp);
      it->add(node);
    }
    else
    {
      node = subTree->get();
    }
    node->SetData(image);

    mitk::LevelWindowProperty::Pointer levWinProp = new mitk::LevelWindowProperty();
    mitk::LevelWindow levelWindow;
    levelWindow.SetAuto( image->GetPic() );
    levWinProp->SetLevelWindow(levelWindow);
    node->GetPropertyList()->SetProperty("levelwindow",levWinProp);

    float extrema[2];
    extrema[0] = 0;
    extrema[1] = 4096;

    CommonFunctionality::MinMax<TImageType>(itkImage,extrema[0],extrema[1]);
    if (extrema[0] == 0 && extrema[1] ==1)
    {
      mitk::BoolProperty::Pointer binProp = new mitk::BoolProperty(true);
      node->GetPropertyList()->SetProperty("binary",binProp);
    }
    delete it;
  }


  /** 
  * \brief converts the itk image to mitk image, creates a datatreenode and adds the node to 
  * the referenced datatree
  */
  template < typename TMeshType >
    static void AddItkMeshToDataTree(typename TMeshType::Pointer itkMesh, mitk::DataTreeIterator* iterator, std::string str)
  {
    mitk::DataTreeIterator* it=iterator->clone();

    mitk::DataTreeNode::Pointer node = NULL;
    mitk::DataTreeIterator* subTree = ((mitk::DataTree *) it->getTree())->GetNext("fileName", new mitk::StringProperty( str.c_str() ));

    if (subTree == NULL || subTree->get() == NULL )
    {
      node=mitk::DataTreeNode::New();
      mitk::StringProperty::Pointer nameProp = new mitk::StringProperty(str.c_str());
      node->SetProperty("fileName",nameProp);
      it->add(node);
    }
    else
    {
      node = subTree->get();
    }

    mitk::SurfaceData::Pointer surface = mitk::SurfaceData::New();
    vtkPolyData* polys = MeshUtil<TMeshType>::meshToPolyData( itkMesh );
    surface->SetVtkPolyData(polys);
    node->SetData( surface );
    node->SetProperty("layer", new mitk::IntProperty(1));
    node->SetVisibility(true,NULL);

    float meshColor[3] = {.5f,.5f,.5f};
    node->SetColor(meshColor,  NULL );
    node->SetVisibility(true, NULL );

    delete it;
  }
  /** 
  * \brief converts the itk image to mitk image, creates a datatreenode and adds the node to 
  * the referenced datatree
  */
    static void AddVtkMeshToDataTree(vtkPolyData* polys, mitk::DataTreeIterator* iterator, std::string str)
  {
    mitk::DataTreeIterator* it=iterator->clone();

    mitk::DataTreeNode::Pointer node = NULL;
    mitk::DataTreeIterator* subTree = ((mitk::DataTree *) it->getTree())->GetNext("fileName", new mitk::StringProperty( str.c_str() ));

    if (subTree == NULL || subTree->get() == NULL )
    {
      node=mitk::DataTreeNode::New();
      mitk::StringProperty::Pointer nameProp = new mitk::StringProperty(str.c_str());
      node->SetProperty("fileName",nameProp);
      it->add(node);
    }
    else
    {
      node = subTree->get();
    }

    mitk::SurfaceData::Pointer surface = mitk::SurfaceData::New();
    surface->SetVtkPolyData(polys);
    node->SetData( surface );
    node->SetProperty("layer", new mitk::IntProperty(1));
    node->SetVisibility(true,NULL);

    float meshColor[3] = {.5f,.5f,.5f};
    node->SetColor(meshColor,  NULL );
    node->SetVisibility(true, NULL );

    delete it;
  }
};
