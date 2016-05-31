#pragma once

#include "GameState.h"

#define STOP		0
#define MOVE_STRATE 1
#define MOVE_BACK	2
#define MOVE_LEFT	3
#define MOVE_RIGHT	4

#define RUNNING		10
#define ROTATING	11

class PlayState : public GameState
{
public:
  void enter(void);
  void exit(void);

  void pause(void);
  void resume(void);

  bool frameStarted(GameManager* game, const Ogre::FrameEvent& evt);
  bool frameEnded(GameManager* game, const Ogre::FrameEvent& evt);

  bool mouseMoved(GameManager* game, const OIS::MouseEvent &e) ;
  bool mousePressed(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id );
  bool mouseReleased(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool keyPressed(GameManager* game, const OIS::KeyEvent &e);
  bool keyReleased(GameManager* game, const OIS::KeyEvent &e);

  static PlayState* getInstance() { return &mPlayState; }

private:

  void _setLights(void);
  void _drawGroundPlane(void);
  void _drawGridPlane(void);


  static PlayState mPlayState;

  int mCharacterDirection;
  int mCharacterState;
  float mCharacterAccel;
  float mCharacterVelocity;

  Ogre::Vector3 mDirection;
  Ogre::Quaternion mSrcQuat, mDestQuat;

  Ogre::Root *mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr;
  Ogre::Camera* mCamera;

  Ogre::Light *mLightP, *mLightD, *mLightS;

  Ogre::SceneNode* mCharacterRoot;
  Ogre::SceneNode* mCharacterYaw;
  Ogre::SceneNode* mCameraHolder;
  Ogre::SceneNode* mCameraYaw;
  Ogre::SceneNode* mCameraPitch;

  Ogre::Entity* mCharacterEntity;

  Ogre::AnimationState* mAnimationState;

  Ogre::Overlay*           mInformationOverlay;

  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;

};


