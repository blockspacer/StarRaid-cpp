#include "gfxSystem.hpp"

/**
 * creates all the grafic sources
 */
gfxSystem::gfxSystem() {
    flagLoaded=0;

    sfNone = new gfxImage;


    //ressouces
    sfRadarSpotNeutralSmall = new gfxImage;

    sfMouseSpot = new  gfxImage;

    sfMessageWindow = new gfxImage;

    sfStarBig = new gfxImage;
    sfStarMiddle = new gfxImage;
    sfStarSmall = new gfxImage;

    sfMenue= new gfxImage;
    sfMenueBG= new gfxImage;
    sfMenueLogo = new gfxImage;
    sfMenueActive = new gfxImage;
    sfMenuePassive = new gfxImage;
    sfMenueBottom = new gfxImage;
    sfMenueBorder = new gfxImage;

    sfWinDecoBrownMenue = new gfxImage;
    sfWinDecoBrownResize = new gfxImage;
    sfWinDecoBrownBackground = new gfxImage;
    sfWinDecoBrownTopLeft = new gfxImage;
    sfWinDecoBrownTopMiddle = new gfxImage;
    sfWinDecoBrownTopRight = new gfxImage;
    sfWinDecoBrownBottomLeft = new gfxImage;
    sfWinDecoBrownBottomMiddle = new gfxImage;
    sfWinDecoBrownBottomRight = new gfxImage;
    sfWinDecoBrownMiddleLeft = new gfxImage;
    sfWinDecoBrownMiddleRight = new gfxImage;
    sfWinDecoBrownTextInputLeft = new gfxImage;
    sfWinDecoBrownTextInputMiddle = new gfxImage;
    sfWinDecoBrownTextInputRight = new gfxImage;

    fontArial12 = new gfxFont;
    fontArial18 = new gfxFont;
}

/**
 * destroys all the grafic sources
 */
gfxSystem::~gfxSystem() {
    delete sfNone;

    delete sfRadarSpotNeutralSmall;

    delete sfMessageWindow;

    delete sfMouseSpot;

    delete sfStarBig;
    delete sfStarMiddle;
    delete sfStarSmall;

    delete sfMenue;
    delete sfMenueBG;
    delete sfMenueLogo;
    delete sfMenueActive;
    delete sfMenuePassive;
    delete sfMenueBottom;
    delete sfMenueBorder;

    delete sfWinDecoBrownMenue;
    delete sfWinDecoBrownResize;
    delete sfWinDecoBrownBackground;
    delete sfWinDecoBrownTopLeft;
    delete sfWinDecoBrownTopMiddle;
    delete sfWinDecoBrownTopRight;
    delete sfWinDecoBrownBottomLeft;
    delete sfWinDecoBrownBottomMiddle;
    delete sfWinDecoBrownBottomRight;
    delete sfWinDecoBrownMiddleLeft;
    delete sfWinDecoBrownMiddleRight;
    delete sfWinDecoBrownTextInputLeft;
    delete sfWinDecoBrownTextInputMiddle;
    delete sfWinDecoBrownTextInputRight;

    delete fontArial12;
    delete fontArial18;
}




gfxImage* gfxSystem::getSurface(string pKey) {
    ///TODO: gfxPoolIterator?
    gfxPoolIterator i = gfxPool.find(pKey);
    if(i != gfxPool.end()) {
        return i->second;
    }
    fontArial12->drawtext(sfNone->getSurface(0), 2, 2, pKey );
    return sfNone;
}


/**
 * Here will the menue be puzzelt together, depending on the resolution and a few other
 * variables
 *   @param pKey The Surface out of the pool that should be displayed
 */
void gfxSystem::addToPool(string pName, string pFileName, int pRotate) {

    //gfxImage tmpGFX;
    gfxImage *tmpGFX = new gfxImage;

    if(pRotate==0) tmpGFX->load(pFileName);
    else tmpGFX->loadMultiple(pFileName, pRotate);


    gfxPool.insert(gfxPoolPair(pName, tmpGFX));
}


/**
 * Here will the menue be puzzelt together, depending on the resolution and a few other
 * variables
 *   @param resX The resolution width
 *   @param resY The resolution height
 */
void gfxSystem::createMenue(int resX, int resY) {

    int i;
    SDL_Rect tmpRect;

    for(i=0;i<=resY;i++) {
        tmpRect = newSDL_Rect(0,i,0,0);
        SDL_BlitSurface(sfMenueBG->getSurface(0), NULL, sfMenue->getSurface(0), &tmpRect );
    }

    SDL_BlitSurface(sfMenueLogo->getSurface(0), NULL, sfMenue->getSurface(0), NULL );
    for(i=0;i<=7;i++) {
        tmpRect = newSDL_Rect(0,((i*30)+128),0,0);
        SDL_BlitSurface(sfMenuePassive->getSurface(0), NULL, sfMenue->getSurface(0), &tmpRect );
    }

    tmpRect = newSDL_Rect(0,((8*30)+ 128),0,0);
    SDL_BlitSurface(sfMenueBorder->getSurface(0), NULL, sfMenue->getSurface(0), &tmpRect );

    tmpRect = newSDL_Rect(0,(resY-16),0,0);
    SDL_BlitSurface(sfMenueBottom->getSurface(0), NULL, sfMenue->getSurface(0), &tmpRect );

}

/**
 * Creats the image for the message window, depending on its size and the resolution
 *   @param resX The resolution width
 *   @param resY The resolution height
 *   @param msgWinW The width of the message window
 *   @param msgWinH The height of the message window
 */
void gfxSystem::createMsgWindow(int resX, int resY, int msgWinW, int msgWinH) {

    int i,e;
    SDL_Rect tmpRect;

    int borderTop, borderBottom, borderLeft, borderRight;
    borderTop=sfWinDecoBrownTopMiddle->heigth,
    borderBottom=sfWinDecoBrownBottomMiddle->heigth;
    borderLeft=sfWinDecoBrownMiddleLeft->width,
    borderRight=sfWinDecoBrownMiddleRight->width;

    for(i=borderLeft;i<=(msgWinW-borderRight);i++) {
        tmpRect = newSDL_Rect(i,0,0,0);
        SDL_BlitSurface(sfWinDecoBrownTopMiddle->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );
    }

    for(i=borderTop;i<=(msgWinH);i++) {
        tmpRect = newSDL_Rect(0,i,0,0);
        SDL_BlitSurface(sfWinDecoBrownMiddleLeft->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );
    }

    for(i=borderTop;i<=(msgWinH);i++) {
        tmpRect = newSDL_Rect(msgWinW-borderRight,i,0,0);
        SDL_BlitSurface(sfWinDecoBrownMiddleRight->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );
    }

    SDL_BlitSurface(sfWinDecoBrownTopLeft->getSurface(0), NULL, sfMessageWindow->getSurface(0), NULL );

    tmpRect = newSDL_Rect(msgWinW-borderRight,0,0,0);
    SDL_BlitSurface(sfWinDecoBrownTopRight->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );

    for(i=borderLeft;i<=(msgWinW-borderRight);i++) {
        for(e=borderTop;e<=(msgWinH-borderBottom);e++) {
            tmpRect = newSDL_Rect(i,e,0,0);
            SDL_BlitSurface(sfWinDecoBrownBackground->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );
        }
    }
    for(i=borderLeft;i<=(msgWinW-borderRight);i++) {
        tmpRect = newSDL_Rect(i,msgWinH-borderBottom,0,0);
        SDL_BlitSurface(sfWinDecoBrownTextInputMiddle->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );
    }

    tmpRect = newSDL_Rect(4,msgWinH-borderBottom,0,0);
    SDL_BlitSurface(sfWinDecoBrownTextInputLeft->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );

    tmpRect = newSDL_Rect(msgWinW-borderRight-4,msgWinH-borderBottom,0,0);
    SDL_BlitSurface(sfWinDecoBrownTextInputRight->getSurface(0), NULL, sfMessageWindow->getSurface(0), &tmpRect );



}



/**
 * Long description
 *   @note Try to keep the only point where SDL_GFX is applyed then phalanxing
 *   @param resX The resolution width
 *   @param resY The resolution height
 */
 void gfxSystem::init(int resX, int resY) {

    //**** set temporary variables
    int msgWinW,msgWinH,msgWinX,msgWinY;
    msgWinW=200;
    msgWinH=40+(10*15); // 10 messages
    msgWinX=200;
    msgWinY=resY-msgWinH+10; // the last 10 set the bottom decoration out of the screen



    //**** loead the grafics only once
    if(flagLoaded==0) {

        fontArial12->load("GFX/times.ttf",12, 255,255,255,0);
        fontArial18->load("GFX/times.ttf",18, 155,155,155,0);

        addToPool("sfCursor", "GFX/sfCursor.bmp", 0);
        addToPool("sfRadar", "GFX/sfRadar.bmp", 0);

        ///TODO: move stuff to the pool
        sfRadarSpotNeutralSmall->load("GFX/sfRadarSpotNeutralSmall.bmp");

        sfMouseSpot->load("GFX/sfMouseSpot.bmp");

        sfStarBig->load("GFX/bg/mapStarBig.bmp");
        sfStarMiddle->load("GFX/bg/mapStarMiddle.bmp");
        sfStarSmall->load("GFX/bg/mapStarSmall.bmp");

        sfMenueBG->load("GFX/sfMenueBG.bmp");
        sfMenueLogo->load("GFX/sfMenueLogo.bmp");
        sfMenueActive->load("GFX/sfMenueActive.bmp");
        sfMenuePassive->load("GFX/sfMenuePassive.bmp");
        sfMenueBottom->load("GFX/sfMenueBottom.bmp");
        sfMenueBorder->load("GFX/sfMenueBorder.bmp");

        sfWinDecoBrownMenue->load("GFX/windowDecorations/brown/sfMenue.bmp");
        sfWinDecoBrownResize->load("GFX/windowDecorations/brown/sfResize.bmp");
        sfWinDecoBrownBackground->load("GFX/windowDecorations/brown/sfBackground.bmp");
        sfWinDecoBrownTopLeft->load("GFX/windowDecorations/brown/sfTopLeft.bmp");
        sfWinDecoBrownTopMiddle->load("GFX/windowDecorations/brown/sfTopMiddle.bmp");
        sfWinDecoBrownTopRight->load("GFX/windowDecorations/brown/sfTopRight.bmp");
        sfWinDecoBrownBottomLeft->load("GFX/windowDecorations/brown/sfBottomLeft.bmp");
        sfWinDecoBrownBottomMiddle->load("GFX/windowDecorations/brown/sfBottomMiddle.bmp");
        sfWinDecoBrownBottomRight->load("GFX/windowDecorations/brown/sfBottomRight.bmp");
        sfWinDecoBrownMiddleLeft->load("GFX/windowDecorations/brown/sfMiddleLeft.bmp");
        sfWinDecoBrownMiddleRight->load("GFX/windowDecorations/brown/sfMiddleRight.bmp");
        sfWinDecoBrownTextInputLeft->load("GFX/windowDecorations/brown/sfTextInputLeft.bmp");
        sfWinDecoBrownTextInputMiddle->load("GFX/windowDecorations/brown/sfTextInputMiddle.bmp");
        sfWinDecoBrownTextInputRight->load("GFX/windowDecorations/brown/sfTextInputRight.bmp");

        // ships
        addToPool("aCC1", "GFX/ships/aCC1.bmp", 36);
        addToPool("aCD1", "GFX/ships/aCD1.bmp", 36);
        addToPool("aCH1", "GFX/ships/aCH1.bmp", 36);
        addToPool("aCZ1", "GFX/ships/aCZ1.bmp", 36);

        // marks
        addToPool("sfSelect50", "GFX/sfSelect50.bmp",36);
        addToPool("sfSelect100", "GFX/sfSelect100.bmp",36);
        addToPool("sfSelect200", "GFX/sfSelect200.bmp",36);

        // actions
        addToPool("actionFrameOn", "GFX/action/frame/frameOn.bmp", 0);
        addToPool("actionFrameOff", "GFX/action/frame/frameOff.bmp", 0);
        addToPool("actionFrameLoading", "GFX/action/frame/frameLoading.bmp", 0);
        addToPool("actionLoading", "GFX/action/frame/loading.bmp", 0);
        addToPool("actionReady", "GFX/action/frame/ready.bmp", 0);

        addToPool("actionComEmp", "GFX/action/comEmp.bmp", 0);
        addToPool("actionComScan", "GFX/action/comScan.bmp", 0);
        addToPool("actionGunSingle", "GFX/action/gunSingle.bmp", 0);
        addToPool("actionGunMulti", "GFX/action/gunMulti.bmp", 0);
        addToPool("actionLaserSingle", "GFX/action/laserSingle.bmp", 0);
        addToPool("actionLaserMulti", "GFX/action/laserMulti.bmp", 0);


        sfNone->load("GFX/sfNONE.bmp");

        flagLoaded=1;


        ///TODO: move out of the loading once to resize, BUG in there
        //**** Create the message window
        sfMessageWindow->create(msgWinW,msgWinH);
        createMsgWindow(resX, resY, msgWinW,msgWinH);

    }


    //**** Create the Menue
    sfMenue->create(35,resY);
    createMenue(resX, resY);

    //**** set the default positions
    getSurface("sfRadar")->setDefaultPositions(resX-getSurface("sfRadar")->width, 0);
    sfMenue->setDefaultPositions(0,0);
    sfMessageWindow->setDefaultPositions(msgWinX,msgWinY);

}

