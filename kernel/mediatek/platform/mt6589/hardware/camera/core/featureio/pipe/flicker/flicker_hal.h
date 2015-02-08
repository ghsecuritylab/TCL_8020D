
#ifndef _FLICKER_HAL_H_
#define _FLICKER_HAL_H_

#include "flicker_hal_base.h"
//#include "af_param.h"
#include "imem_drv.h"

using namespace android;

typedef struct FLKThreSetting_S
{
	MUINT32 u4FlickerPoss1; 		// impossible flicker
	MUINT32 u4FlickerPoss2; 		// maybe flicker exist
	MUINT32 u4FlickerFreq1; 		// flicker frequency detect
	MUINT32 u4FlickerFreq2; 		// flicker frequency detect
	MUINT32 u4ConfidenceLevel1;   // flicker confidence level
	MUINT32 u4ConfidenceLevel2;   // flicker confidence level
	MUINT32 u4ConfidenceLevel3;   // flicker confidence level
}FLKThreSetting_T;

  //move to ae_param.h
typedef struct FLKWinCFG_S
{
	MINT32 m_uImageW;
	MINT32 m_uImageH;
	MINT32 m_u4OffsetX;
	MINT32 m_u4OffsetY;
	MINT32 m_u4NumX;
	MINT32 m_u4NumY;
	MINT32 m_u4SizeX;
	MINT32 m_u4SizeY;
}FLKWinCFG_T;

using namespace NS3A;



class FlickerHal : public FlickerHalBase {
public:
    static FlickerHalBase* getInstance();
    virtual MVOID      destroyInstance();
    virtual MINT32 	open(MINT32 i4SensorDev);
    virtual MVOID 	close();


    virtual MINT32 createBuf();
    virtual MVOID releaseBuf();

private:
    FlickerHal();
    virtual ~FlickerHal();

 //   virtual MVOID  setFlickerThresholdParams(NSCamCustom::FlickerThresholdSetting_T  *strFlickerThres);
	virtual MVOID	setFlickerThresholdParams(FLKThreSetting_T  *strFlickerThres);
    virtual MINT32 setFlickerDrv(MBOOL flicker_en);
    virtual MINT32 setFlickerWinConfig(FLKWinCFG_T* ptFlkWinCfg);
    virtual MINT32 setFlickerDMAConfig(unsigned long flicker_DMA_address ,MINT32 DMASize );
    virtual MUINT32 GetFlicker_CurrentDMA();
	static MVOID* uninitThread(MVOID* arg);

public:
    virtual MINT32 init(MINT32 i4SensorDev);
    virtual MINT32 uninit();
    virtual MINT32  enableFlickerDetection(MBOOL bEnableFlicker);
    virtual MINT32 analyzeFlickerFrequency(MINT32 i4LMVcnt, MINT32 *i4LMV_x, MINT32 *i4LMV_y, MINT64 *i4vAFstat);
    virtual MINT32 setWindowInfo();
    virtual MINT32 getFlickerStatus(MINT32 *a_flickerStatus);
//	virtual MBOOL sendCommand(ECmd_T eCmd);
	virtual MBOOL sendCommand(FLKCmd_T eCmd,void* pi4Arg);

private:
    volatile int mUsers;
	pthread_t       mUninitThread;
    mutable Mutex mLock;
   // NSIspSysram::IspSysramDrv* m_pFlickerSysram;
    SensorDrv *m_pSensorDrv;
    IspDrv *m_pIspDrv;
    isp_reg_t *m_pIspRegMap;


    MBOOL  m_bFlickerModeChange;
	MBOOL  m_bFlickerEnable;
    MBOOL m_bFlickerEnablebit;
    MUINT32 m_u4FlickerWidth;
    MUINT32 m_u4FlickerHeight;
    MUINT32 m_u4SensorPixelClkFreq;
    MINT32 m_u4FreqFrame;
    MINT32 m_u4Freq000;
    MINT32 m_u4Freq100;
    MINT32 m_u4Freq120;
    MINT32 m_vAMDF[8];
    MUINT32 m_u4PixelsInLine;
    FLICKER_STATUS m_flickerStatus ;
    LMV_STATUS m_EIS_LMV_Flag;
    MINT32 *m_pVectorAddress1;
    MINT8 m_iDataBank;
    MINT32 *m_pVectorData1;
    MINT32 *m_pVectorData2;
    MINT32 m_u4FlickerFreq;
    IMemDrv*  mpIMemDrv;
	IMEM_BUF_INFO flkbufInfo[2];
	FLKWinCFG_T strFlkWinCfg;
	MINT32 FLK_DMA_Size;
	MINT32 m_SensorDev;
	MINT32 m_FlickerMotionErrCount;
	MINT32 m_FlickerMotionErrCount2;
    MBOOL  m_bPause;

private:
	EisHalBase* mpEisHal;
	MBOOL updateEISInfo();
	MBOOL updateAAAInfo();
	MBOOL Updated();
	MBOOL getFlickerThresPara(NSCamCustom::eFlickerDetectSpeed idx ,FLKThreSetting_T *ptFlickerThreshold);

#if 1
protected:	////	Attributes.
		//
		MINT32			mi4DetectedResult;

		/*
		 *	AF Windows Status.
		 *
		 *	FIXME:
		 *		AF_WIN_NUM is defined in af_param.h
		 *		NUM should be queried from af_param.h.
		 */
		enum { eAFWinNum = 9/* AF_WIN_NUM*/ };
		//enum { eAFWinNum = 1 };
		MINT64 mai4AFWin[eAFWinNum];
		//
		//	EIS GMV X/Y
#if 1
MINT32			mai4GMV_X;
MINT32			mai4GMV_Y;

#else
		enum { eMaxGMVNum = 1 };
		MINT32			mai4GMV_X[eMaxGMVNum];
		MINT32			mai4GMV_Y[eMaxGMVNum];
#endif

#endif
};

#endif
