#include "PlayState.h"
#include "TitleState.h"

using namespace Ogre;

PlayState PlayState::mPlayState;


void PlayState::enter(void)
{
  mRoot = Root::getSingletonPtr(); 
  mRoot->getAutoCreatedWindow()->resetStatistics();

  mSceneMgr = mRoot->getSceneManager("main");
  mCamera = mSceneMgr->getCamera("main");
  mCamera->setPosition(Ogre::Vector3::ZERO);

  _drawGridPlane();
  _setLights();
  _drawGroundPlane();

  mInformationOverlay = OverlayManager::getSingleton().getByName("Overlay/Information");
  mInformationOverlay->show(); 

  mCharacterRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode("lancerEvolutionRoot");
  mCharacterRoot->setPosition(0.0f, 0.0f, 2000.0f);

  mCharacterYaw = mCharacterRoot->createChildSceneNode("lancerEvolutionYaw");
  mCharacterYaw->yaw(Degree(90.0f));

  mCameraYaw = mCharacterRoot->createChildSceneNode("CameraYaw", Vector3(0.0f, 120.0f, 0.0f));
 // mCameraYaw->yaw(Degree(90.0f));
  mCameraPitch = mCameraYaw->createChildSceneNode("CameraPitch");
  mCameraHolder = mCameraPitch->createChildSceneNode("CameraHolder", Vector3(0.0f, 0.0f, 500.0f));

  mCharacterEntity = mSceneMgr->createEntity("lancerEvolution", "lancer_evolutionBase.mesh");
  mCharacterYaw->scale(1000.0f, 1000.0f, 1000.0f);
  mCharacterYaw->attachObject(mCharacterEntity);
  mCharacterEntity->setCastShadows(true);

  mCharacterAccel = 0.0f;
  mCharacterDirection = STOP;
  mCharacterState = RUNNING;
  mCharacterVelocity = 111.0f;

  mCameraHolder->attachObject(mCamera);
  mCamera->lookAt(mCameraYaw->getPosition());

}

void PlayState::exit(void)
{
	mSceneMgr->clearScene();
	mInformationOverlay->hide();
}

void PlayState::pause(void)
{
}

void PlayState::resume(void)
{
}

bool PlayState::frameStarted(GameManager* game, const FrameEvent& evt)
{
	if (STOP != mCharacterDirection)
	{
		if (mCharacterRoot->getPosition().z < 5000.0f || mCharacterRoot->getPosition().z > -5000.0f)
		{
			if (mCharacterRoot->getPosition().z > 5000.0f)
				mCharacterRoot->setPosition(mCharacterRoot->getPosition().x,
					mCharacterRoot->getPosition().y, 5000.0f);
			else if (mCharacterRoot->getPosition().z<-5000.0f)
				mCharacterRoot->setPosition(mCharacterRoot->getPosition().x,
					mCharacterRoot->getPosition().y, -5000.0f);

			if (RUNNING == mCharacterState)
			{
				if (MOVE_STRATE == mCharacterDirection)
				{
					if (mCharacterAccel < 50.0f)
						mCharacterAccel += 0.01f;
					else if (mCharacterAccel > 50.0f)
						mCharacterAccel = 50.0f;
				}
				else if (MOVE_BACK == mCharacterDirection)
				{
					if (mCharacterAccel > 0.0f)
						mCharacterAccel -= 0.01f;
					else if (mCharacterAccel < 0.0f)
						mCharacterAccel = 0.0f;

				}
			}
			else if (ROTATING == mCharacterState)
			{
				if (MOVE_LEFT == mCharacterDirection)
				{
					mCharacterRoot->yaw(Degree(0.5f));
				}
				else if (MOVE_RIGHT == mCharacterDirection)
				{
					mCharacterRoot->yaw(Degree(-0.5f));
				}
				mCharacterState = RUNNING;
			}

			mCharacterRoot->translate(0.0f, 0.0f,
				-mCharacterDirection * mCharacterVelocity *mCharacterAccel *evt.timeSinceLastFrame);
		}
	}

	
  return true;
}

bool PlayState::frameEnded(GameManager* game, const FrameEvent& evt)
{
  static Ogre::DisplayString currFps = L"현재 FPS: "; 
  static Ogre::DisplayString avgFps = L"평균 FPS: ";
  static Ogre::DisplayString bestFps = L"최고 FPS: ";
  static Ogre::DisplayString worstFps = L"최저 FPS: ";

  OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("AverageFps");
  OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("CurrFps");
  OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("BestFps");
  OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("WorstFps");

  const RenderTarget::FrameStats& stats = mRoot->getAutoCreatedWindow()->getStatistics();

  guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
  guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
  guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS));
  guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS));

  return true;
}


bool PlayState::keyReleased(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_A:
		break;

	case OIS::KC_D:
		break;
	}
  return true;
}

bool PlayState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
		case OIS::KC_ESCAPE:
			game->changeState(TitleState::getInstance());
			break;

		case OIS::KC_W:
			mCharacterState = RUNNING;
			mCharacterDirection = MOVE_STRATE;
			break;

		case OIS::KC_S:
			mCharacterState = RUNNING;
			mCharacterDirection = MOVE_BACK;
			break;

		case OIS::KC_A:
			mCharacterState = ROTATING;
			mCharacterDirection = MOVE_LEFT;
			break;

		case OIS::KC_D:
			mCharacterState = ROTATING;
			mCharacterDirection = MOVE_RIGHT;
			break;
	}

  return true;
}

bool PlayState::mousePressed(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool PlayState::mouseReleased(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}


bool PlayState::mouseMoved(GameManager* game, const OIS::MouseEvent &e)
{ 
  return true;
}



void PlayState::_setLights(void)
{
  mSceneMgr->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));
  mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

  mLightD = mSceneMgr->createLight("LightD");
  mLightD->setType(Light::LT_DIRECTIONAL);
  mLightD->setDirection( Vector3( 1, -2.0f, -1 ) );
  mLightD->setVisible(true);
}

void PlayState::_drawGroundPlane(void)
{
}

void PlayState::_drawGridPlane(void)
{
  // 좌표축 표시
  Ogre::Entity* mAxesEntity = mSceneMgr->createEntity("Axes", "axes.mesh");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("AxesNode",Ogre::Vector3(0,0,0))->attachObject(mAxesEntity);
  mSceneMgr->getSceneNode("AxesNode")->setScale(5, 5, 5);

  Ogre::ManualObject* gridPlane =  mSceneMgr->createManualObject("GridPlane"); 
  Ogre::SceneNode* gridPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("GridPlaneNode"); 

  Ogre::MaterialPtr gridPlaneMaterial = Ogre::MaterialManager::getSingleton().create("GridPlanMaterial","General"); 
  gridPlaneMaterial->setReceiveShadows(false); 
  gridPlaneMaterial->getTechnique(0)->setLightingEnabled(true); 
  gridPlaneMaterial->getTechnique(0)->getPass(0)->setDiffuse(1,1,1,0); 
  gridPlaneMaterial->getTechnique(0)->getPass(0)->setAmbient(1,1,1); 
  gridPlaneMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1); 

  gridPlane->begin("GridPlaneMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
  for(int i=0; i<101; i++)
  {
    gridPlane->position(-5000.0f, 0.0f, 5000.0f-i*50);
    gridPlane->position(5000.0f, 0.0f, 5000.0f-i*50);

    gridPlane->position(-5000.f+i*50, 0.f, 5000.0f);
    gridPlane->position(-5000.f+i*50, 0.f, -5000.f);
  }

  gridPlane->end(); 

  gridPlaneNode->attachObject(gridPlane);
}